/*
 * PotentiallyDangerousScanUserMethodUserMethod
 *
 * This example is implemented looking at User list associated
 * with p.d. function.
 *
 * Similar to PotentiallyDangerousScanUserMethod and
 * PotentiallyDangerousScanUserMethodFunctionPass 
 *
 */
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/ValueHandle.h"
#include "llvm/Pass.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/CallSite.h"
#include "llvm/Analysis/CallGraph.h"

#include <map>
#include <string>
#include <iostream>
#include <fstream>

#include <json/config.h>
#include <json/value.h>
#include <json/reader.h>

using namespace llvm;

#include "PotentiallyDangerousScanUserMethod.h"

void
PotentiallyDangerousScanUserMethod::getAnalysisUsage(AnalysisUsage &AU) const
{
}


bool
PotentiallyDangerousScanUserMethod::runOnModule(Module &M)
{
	Json::Value fnDict;
	std::ifstream cfgFileStream;

	cfgFileStream.open(this->getConfigFilePath());
	cfgFileStream >> fnDict;
	cfgFileStream.close();
	Json::Value fnList = fnDict["functions"];
	assert(fnList.isArray() && "fnList was not an array");
	Json::ArrayIndex aLen = fnList.size();
	for (Json::ArrayIndex ai = 0; ai < aLen; ai++) {
		Function *f = M.getFunction(fnList[ai].asString());
		if (f == NULL) {
			continue;
		}
		for (auto fi = f->user_begin(); fi != f->user_end(); ++fi) {
			User *u = *fi;
			if (isa<CallInst>(u) || isa<InvokeInst>(u)) {

				/* CallSite is a nice container for call and invoke */
				CallSite cs(u);
				Function *caller = cs.getCaller();
				std::string callerName = "unnamed_func";
				if (caller->hasName()) {
					callerName = caller->getName().str();
				}
				std::cout << "   " << callerName << " calls "
				  << fnList[ai].asString() << "\n";
			}
		}
	}
	return false;
}

char PotentiallyDangerousScanUserMethod::ID = 1;
static RegisterPass<PotentiallyDangerousScanUserMethod> XX("pot-danger-user-method", "Potentially Dangerous Call (CWE 676) done with User list");
