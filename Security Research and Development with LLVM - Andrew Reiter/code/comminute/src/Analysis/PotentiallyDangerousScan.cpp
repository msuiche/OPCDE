/*
 *
 * PotentiallyDangerousScan
 *
 * This is yet another naive scan :-). This is just 
 * checking for CWE 676 which is just the use of potentially
 * dangerous functions.
 *
 * There are, again, multiple ways that one might do this
 * check in LLVM (ie, ignoring objdump | grep :)). You
 * could use a visitor, instruction iteration, or even 
 * the User checking of the functions in question.
 * Each of those are valid, but here we perform this
 * by relying on the CallGraph pass being run prior
 * to this pass. We then just go through those results
 * and perform the analysis. So this code illustrates
 * pass dependency (via getAnalysisUsage()) and using
 * the CallgraphPass as a source of data.  
 *
 */
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/ValueHandle.h"
#include "llvm/Pass.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Analysis/CallGraph.h"

#include <map>
#include <string>
#include <iostream>
#include <fstream>

#include <json/config.h>
#include <json/value.h>
#include <json/reader.h>

using namespace llvm;

#include "PotentiallyDangerousScan.h"

/*
 * This informs the pass manager that prior to running this pass, the
 * CallGraphWrapperPass should be run. This helps in ordering passes so
 * you can have a reasonable expectation of state of the IR (or other)
 * upon entry to your runOn*() function.
 *
 */
void
PotentiallyDangerousScan::getAnalysisUsage(AnalysisUsage &AU) const
{
	AU.addRequired<CallGraphWrapperPass>();
}


bool
PotentiallyDangerousScan::runOnModule(Module &M)
{
	Json::Value fnDict;
	std::ifstream cfgFileStream;
	std::vector<Function *> pdFunctions;

	/*
	 * Make use of the libjsoncpp to ingest a json config file.
	 * This file houses the set of functions that one might consider
	 * to fall under CWE 676. Then see if any of them exist in this
	 * module.
	 *
	 * I am being trusting of the configs.. 
	 */
	cfgFileStream.open(this->getConfigFilePath());
	cfgFileStream >> fnDict;
	cfgFileStream.close();
	Json::Value fnList = fnDict["functions"];
	assert(fnList.isArray() && "fnList was not an array");
	Json::ArrayIndex aLen = fnList.size();
	for (Json::ArrayIndex ai = 0; ai < aLen; ai++) {
		Function *f = M.getFunction(fnList[ai].asString());
		if (f != NULL) {
			pdFunctions.push_back(f);
		}
	}

	/*
	 * A great thing about the pass design is that you can share information 
	 * between them. Here we are getting the call graph as previously built
	 * by the call graph pass. If you see the getAnalysisUsage() function at 
	 * the bottom of this file, you will note the dependency on that pass
	 * being run prior to this one.
	 *
	 */
	CallGraphWrapperPass &cgPass = getAnalysis<CallGraphWrapperPass>();

	/* store results in pairs of caller and callee */
	typedef std::pair<Function *, Function *> PDRes;
	std::vector<PDRes> results;

	/*
	 * The container we go through is a map with key Function * and value of
	 * CallGraphNode list. The key is the caller and value the callees.
	 * 
	 * The first entry this map is NULL caller and contains the set of all functions.
	 *
	 */
	CallGraph &cg = cgPass.getCallGraph();
	for (auto cgIt = cg.begin(); cgIt != cg.end(); ++cgIt) {
		/*
		 * Calling function
		 */
		Function *caller = const_cast<Function *>(cgIt->first);
		if (caller == NULL) {
			continue;
		}

		CallGraphNode &callees = *cgIt->second;
		if (callees.size() == 0) {
			continue;
		}

		/*
		 * We iterate through a vector of CallRecords, which is:
		 *  typedef std::pair<WeakVH, CallGraphNode *> CallRecord;
		 * in which each CGN is a called function.
		 *
		 */ 
		for (const auto &crIt : callees) {
			if (Function *callee = crIt.second->getFunction()) {

				/* determine if the callee is something we consider dangerous */
				for (auto pdIt = pdFunctions.begin(); pdIt != pdFunctions.end(); ++pdIt) {
					Function *pdFn = *pdIt;
					if (pdFn == callee) {
						/* Save a result since matched a pd function */
						auto r = std::make_pair(caller, callee);
						results.push_back(r);
						break;
					}
				}
			}
		}
	}

	errs() << "Results for potentially dangerous function call usage:\n";
	for (auto rIt = results.begin(); rIt != results.end(); ++rIt) {
		Function *caller = rIt->first;
		Function *callee = rIt->second;
		errs() << "   " << caller->getName().str() << " calls " << \
		  callee->getName().str() << "\n";
	}

	return false;
}

char PotentiallyDangerousScan::ID = 1;
static RegisterPass<PotentiallyDangerousScan> XX("pot-danger", "Potentially Dangerous Call (CWE 676)");
