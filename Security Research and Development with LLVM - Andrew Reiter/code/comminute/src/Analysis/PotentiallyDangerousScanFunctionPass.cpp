/*
 *
 * PotentiallyDangerousScanFunctionPass
 *
 * This example implements the scan as a function pass and
 * then iterate through the instructions for calls. This could
 * also be done as a visitor.
 *
 */
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/ValueHandle.h"
#include "llvm/Pass.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/CallSite.h"
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

#include "PotentiallyDangerousScanFunctionPass.h"

void
PotentiallyDangerousScanFunctionPass::getAnalysisUsage(AnalysisUsage &AU) const
{
}

void
PotentiallyDangerousScanFunctionPass::setConfigFilePath(std::string s)
{

	_cfgFilePath = s; 
	pdFunctions.clear();
	lookupPDFunctions = true;
}


bool
PotentiallyDangerousScanFunctionPass::runOnFunction(Function &f)
{
	std::string caller = "unnamed_func";

	if (lookupPDFunctions) {
		Json::Value fnDict;
		std::ifstream cfgFileStream;

		cfgFileStream.open(this->getConfigFilePath());
		cfgFileStream >> fnDict;
		cfgFileStream.close();
		Json::Value fnList = fnDict["functions"];
		assert(fnList.isArray() && "fnList was not an array");
		Json::ArrayIndex aLen = fnList.size();
		Module *m = f.getParent();
		for (Json::ArrayIndex ai = 0; ai < aLen; ai++) {
			Function *pdFn = m->getFunction(fnList[ai].asString());
			if (pdFn != NULL) {
				pdFunctions.push_back(pdFn);
			}
		}
		lookupPDFunctions = false;
	}
	// Nothing to find.
	if (pdFunctions.empty()) {
		return false;
	}

	if (f.hasName()) {
		caller = f.getName().str();
	}
	for (auto ii = inst_begin(f); ii != inst_end(f); ++ii) {
		Instruction *in = &*ii;
		if (isa<CallInst>(in) || isa<InvokeInst>(in)) {
			CallSite cs(in);
			Function *called = cs.getCalledFunction();
			for (auto pdi = pdFunctions.begin(); pdi != pdFunctions.end(); ++pdi) {
				Function *pdf = *pdi;
				if (called == pdf) {
					std::cout << "   " << caller << " called " << pdf->getName().str() << "\n";
					break;
				}
			}
		}
	}
	return false;
}

char PotentiallyDangerousScanFunctionPass::ID = 1;
static RegisterPass<PotentiallyDangerousScanFunctionPass> XX("pot-danger-function-pas", "Potentially Dangerous Call (CWE 676) func pass");
