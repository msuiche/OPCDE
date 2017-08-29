/*
 * This is a dependency for a few of the passes. Collects
 * call sites and organizes them by TargetCallType.
 *
 */
 
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/CallSite.h"
#include "llvm/Support/raw_ostream.h"

#include <string>
#include <iostream>
#include <fstream>

#include <json/config.h>
#include <json/value.h>
#include <json/reader.h>

using namespace llvm;

#include "TargetCallSitesPass.h"
#include "../Transform/FunctionExternalizer.h"

void
TargetCallSitesPass::getAnalysisUsage(AnalysisUsage &AU) const
{
}

void
TargetCallSitesPass::parseConfig(std::string configFilePath, TargetCallType tct,
  Module *M)
{
	Json::Value dict;
	std::ifstream strm;

	// XXX quite trusting 
	strm.open(configFilePath);
	strm >> dict;
	strm.close();
	Json::Value::Members mems = dict.getMemberNames();
	for (auto memIt = mems.begin(); memIt != mems.end(); ++memIt) {
		std::string fnName = *memIt;
		
		int argIdx = dict[fnName].asInt();
		assert(argIdx >= -1 && "Argument index should be >= -1");

		/*
		 * Determine if the name given for the function in the config is
		 * a name of a function in this module.
		 */
		Function *fp = M->getFunction(fnName);
		if (fp == NULL) {
			continue;
		}

		/*
		 * See if argument counts match up
		 * If we were cool, we would check arg types if we detected
		 * no name mangling.
		 * 
		 * The difference between sink and source cases is that at some point
		 * want to not just handle return value of source but allow for in/out
		 * or out arguments to be tainted.
		 */
		if (argIdx != -1 && \
		  (fp->arg_size() == 0 || fp->arg_size() <= (unsigned)argIdx)) {
				continue;
		} 

		/*
		 * Check this Function's User list. If there is no instruction using
		 * this function, then we have nothing to check for it.
		 */
		if (fp->user_empty() == true) {
			continue;
		}

		/* Ok, so we have name, argument, function, and a non-empty user list */
		for (auto userIt = fp->user_begin(); userIt != fp->user_end();
		  ++userIt) {
			User *targUser = *userIt;

			/*
			 * Not handling functions passed call backs or entry pts based on
			 * RT environment, so just get CallSites. Then make sure the called
			 * function is the targeted function (from config file)
			 */
			if (!isa<CallInst>(targUser) && !isa<InvokeInst>(targUser)) {
				continue;
			}
			Instruction *targInst = cast<Instruction>(targUser);
			std::unique_ptr<TargetCallSite> tcs(new TargetCallSite(targInst,
			  argIdx));
			targetCallMap[tct].push_back(std::move(tcs));
		}
	}
}

bool
TargetCallSitesPass::runOnModule(Module &M)
{
	errs() << "Running target call sites pass.\n";

	for (auto k = targetConfigMap.begin(); k != targetConfigMap.end(); ++k) {
		TargetCallType t = k->first;
		std::string p = k->second;
		parseConfig(p, t, &M);
	}
	return false;
}
char TargetCallSitesPass::ID = 0;
static RegisterPass<TargetCallSitesPass> XX("target-call-sites", "");
