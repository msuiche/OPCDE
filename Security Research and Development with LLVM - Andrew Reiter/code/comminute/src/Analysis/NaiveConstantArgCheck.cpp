/*
 * NaiveConstantArgCheck
 * 
 * There are some functions that you do not want constant values
 * as arguments... some such are seed functions to (P)RNGs.
 * This attempts some basic detection of such cases, but is quite
 * naive
 *
 * Note:
 *  Things would be better if this used the TargetCallSitesPass.
 *  Mostly wanted to show using instruction iteration in an 
 *  example. There are many improvements to be made.. C'est la vie.
 *
 */

#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/CallSite.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/DebugLoc.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LazyCallGraph.h"

#include <string>
#include <iostream>
#include <fstream>

#include <json/config.h>
#include <json/value.h>
#include <json/reader.h>

using namespace llvm;

#include "NaiveConstantArgCheck.h"

void
NaiveConstantArgCheck::getAnalysisUsage(AnalysisUsage &AU) const
{
}

bool
NaiveConstantArgCheck::runOnModule(Module &M)
{
	errs() << "Running naive constant argument check pass\n";

	Json::Value caDict;
	std::ifstream cfgFileStream;
	std::map<std::string, std::pair<Function *, unsigned>> _existingToCheck;

	cfgFileStream.open(this->getConfigFilePath());
	cfgFileStream >> caDict;
	cfgFileStream.close();

	Json::Value::Members mems = caDict.getMemberNames();
	for (auto memberIt = mems.begin(); memberIt != mems.end(); ++memberIt) {
		std::string fnName = *memberIt;
		unsigned fnArgIdx = caDict[fnName].asUInt();

		// Lookup Function by name in this module
		Function *f = M.getFunction(fnName);
		if (f == NULL) {
			continue;
		}

		// If arg count for target fn and this fn don't match, don't add it. 
		if (f->arg_size() == 0 || f->arg_size() <= fnArgIdx) {
			continue;
		}
		_existingToCheck[fnName] = std::make_pair(f, fnArgIdx);
	}

	/*
	 * Go through every function (cept some)
	 * Go through every call instruction 
	 * Determine if the called function is a sink function
	 * Naively check if the argument in question is constant (to that function)
	 * If so, add to result set.
	 * See User in naive sensitive data leak for another way to do things.
	 *
	 */
	for (auto &f : M) {
		Function *parentFunction = &f;
	
		/*
		 * Skip analyzing any functions we check the calling of.
		 */	
		auto etcIt = _existingToCheck.begin();
		for ( ; etcIt != _existingToCheck.end(); ++etcIt) {
			auto fi = etcIt->second;
			Function *ignoreMe = fi.first;
			if (ignoreMe == parentFunction) {
				break;
			}
		}
		if (etcIt != _existingToCheck.end()) {
			continue;
		}

		/*
		 * Iterate through the instructions that compose the function.
		 *
		 * Instead of going through instructions to find CallInst, etc,
		 * we could use an instruction visitor. For such an example, see the
		 * visitor code found in the intflip base above this directory.
		 * 
		 * Further, as per two comments above and code in another file, could 
		 * just utilize the User list associated with the Function.
		 */
		for (inst_iterator iIt = inst_begin(f); iIt != inst_end(f); ++iIt) {
			/*
			 * Check if the current instruction is a call instruction. If not,
			 * skip to the next instruction.
			 *
			 * Some might use:
			 *     if (CallInst *ci = dyn_cast<CallInst>(&fInst) {
		 	 *	   } else if (InvokeInst *ii ....)
			 * instead...
			 *
			 */
			Instruction *fInst = &*iIt;
			if (!isa<CallInst>(fInst) && !isa<InvokeInst>(fInst)) {
				continue;
			}
			DILocation *lineInfo = fInst->getDebugLoc().get();
			CallSite cs(fInst);
			Function *calledFunction = cs.getCalledFunction();

			auto fIt = _existingToCheck.begin();
			for (; fIt != _existingToCheck.end(); ++fIt) {
				auto j = fIt->second;
				Function *badFunc = j.first;
				unsigned idx = j.second;

				if (badFunc == calledFunction) {

					// We just assume there needs to be enough arguments
					// Improvement would check function signature or something.
					unsigned nOps = cs.getNumArgOperands();
					if (nOps <= idx) {
						continue;
					}

					// Get argument by index.
					Value *arg = cs.getArgOperand(idx);

					// and determine if this argument is constant
					if (!isa<Constant>(arg)) {
						continue;
					}

					/*
					 * Since the arg was constant and was to a fn of interest,
					 * then save off this info as a finding to display.
					 *
					 */
					NaiveConstantArgCheckResult n(parentFunction,
					  calledFunction, arg, idx, lineInfo);
					n.printResult();
				}
			}
		}
	}
	return false;
}

void
NaiveConstantArgCheckResult::printResult()
{
	bool hl = hasLocation();
	bool hn = caller->hasName();	

	if (hn && hl) {
		unsigned line = loc->getLine();
		StringRef file = loc->getFilename();
		StringRef fdir = loc->getDirectory();
		std::cout << "   !" << caller->getName().str() << " calls "
		  << callee->getName().str() << " where arg index: "
		  << argIndex << " is a constant\n";
		std::cout << "       " << file.str() << ":" << line << "\n";
	} else if (hn && !hl) {
		std::cout << "   !" << caller->getName().str() << " calls "
		  << callee->getName().str() << " with argument "
		  << argIndex << " of constant value: \n      ";
		argument->dump();	
	}
}

char NaiveConstantArgCheck::ID = 0;
static RegisterPass<NaiveConstantArgCheck> XX("naive-con-arg-check", "Basic constant arg check");
