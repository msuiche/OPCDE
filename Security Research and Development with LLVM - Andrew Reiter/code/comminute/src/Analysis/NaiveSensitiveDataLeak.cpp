/*
 * NaiveSensitiveDataLeak
 * 
 * It makes use of the TargetCallSitesPass as a way to get uses and
 * target values of interest (sinks, sources, and the related data);
 * it runs prior to this pass.
 *
 * There is no handling of special entry points or callbacks that are
 * tainted a priori.
 */
 
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/CallSite.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/DebugLoc.h"
#include "llvm/Support/raw_ostream.h"

#include <string>

using namespace llvm;

#include "TargetCallSitesPass.h"
#include "NaiveSensitiveDataLeak.h"

void
NaiveSensitiveDataLeak::getAnalysisUsage(AnalysisUsage &AU) const
{
	AU.addRequired<TargetCallSitesPass>();	
	AU.setPreservesCFG();
}

bool
NaiveSensitiveDataLeak::runOnModule(Module &M)
{
	errs() << "Running naive sensitive data leak pass\n";
	/*
	 * We can use the upstream analysis from the TargetCallSitesPass.
	 * Always nice use already available tools.
	 */
	TargetCallSitesPass &p = getAnalysis<TargetCallSitesPass>();


	if (p.src_empty()) {
		return false;
	}	
	if (p.snk_empty()) {
		return false;
	}

	// For each sink value available, we must attempt to trace it to a source
	for (auto snkIt = p.snk_begin(); snkIt != p.snk_end(); ++snkIt) {
		TargetCallSite *snkSite = &*snkIt->get();
		Value *leakData = snkSite->getTarget();	
		auto srcIt = p.src_end();
		--srcIt;
		bool brk_back = false;
		for (; brk_back == false; --srcIt) {
			if (srcIt == p.src_begin()) {
				brk_back = true;
			}
			TargetCallSite *srcSite = &*srcIt->get();
			Value *originalSourceData = srcSite->getTarget();
			Value *sourceData = originalSourceData;
			if (isa<CallInst>(leakData) || isa<InvokeInst>(leakData)) {
				if (leakData == sourceData) {
					printResult(srcSite, snkSite);
					break;
				}
			}
		}
	}
	return false;	

}

void
NaiveSensitiveDataLeak::printResult(TargetCallSite *srcSite,
  TargetCallSite *snkSite)
{
	Instruction *snkIn = snkSite->getInstruction();
	Instruction *srcIn = srcSite->getInstruction();

	DILocation *snkLoc = snkIn->getDebugLoc().get();
	DILocation *srcLoc = srcIn->getDebugLoc().get();
				
	errs() << "  ! sensitive data leak \n";
	errs() << "     " << snkSite->getCaller()->getName()
	  << " calls " << snkSite->getCalled()->getName()
	  << " where arg idx #" << snkSite->getArgIndex()
	  << " is tainted sensitive. file: " << snkLoc->getFilename() 
	  << " line: " << snkLoc->getLine() << "\n"; 
	errs() << "        source: " << srcSite->getCaller()->getName() 
	  << " calls " << srcSite->getCalled()->getName() 
	  << " at line: "<< srcLoc->getLine()  << " of file: "
	  << srcLoc->getFilename() << "\n";
}

char NaiveSensitiveDataLeak::ID = 0;
static RegisterPass<NaiveSensitiveDataLeak> XX("naive-sensitive-leak",
  "Naive sensitive data leak");
