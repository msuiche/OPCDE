/* 
 * XXX I accidentally committed this file.. so maybe ignore.. but
 * leaving it because it might be useful to read for someone.
 */

/*
 * NaiveSensitiveDataLeak
 * 
 * It makes use of the TargetCallSitesPass as a way to get uses and
 * target values of interest (sinks, sources, and the related data);
 * it runs prior to this pass.
 *
 * It is using the StoreCollector a poor means of tracking 
 * memory load/store use. That could be a pass but it isn't. There
 * are much better ways for handling this... Andersen's AA or
 * MemoryDependenceAnalysis. The former is a bit more memory intensive
 * and agressive, while the latter is ``lazy''.
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
#include "StoreCollector.h"

void
NaiveSensitiveDataLeak::getAnalysisUsage(AnalysisUsage &AU) const
{
	AU.addRequired<TargetCallSitesPass>();	
	AU.preservesCFG();
}

bool
NaiveSensitiveDataLeak::runOnModule(Module &M)
{
	errs() << "Running naive sensitive data leak pass\n";
    TargetCallSitesPass &p = getAnalysis<TargetCallSitesPass>();


	if (p.src_empty()) {
		return false;
	}	
	if (p.snk_empty()) {
		return false;
	}

	StoreCollector *store = new StoreCollector();

	// For each sink value available, we must attempt to trace it to a source
	for (auto snkIt = p.snk_begin(); snkIt != p.snk_end(); ++snkIt) {
		TargetCallSite *snkSite = &*snkIt->get();
		Value *leakData = snkSite->getTarget();	

#if 0
		/* Lazily refresh the StoreInst holder */
		if (snkSite->getCaller() != store->getFunction()) {
			store->collect(snkSite->getCaller());
		}
#endif

		for (auto srcIt = p.src_begin(); srcIt != p.src_end(); ++srcIt) {
			TargetCallSite *srcSite = &*srcIt->get();
			Value *originalSourceData = srcSite->getTarget();
			Value *sourceData = originalSourceData;

			while (true) {
				if (isa<CallInst>(leakData) || isa<InvokeInst>(leakData)) {
					if (leakData == sourceData) {
						printResult(srcSite, snkSite);
					}
					/*
					 * Does not handle propagators at this point :(
					 * so any call is either a source or dead end.
					 */	
					break;
				}

				if (isa<Constant>(leakData)) {
					break;
				}
				if (isa<AllocaInst>(leakData)) {
					break;
				}

				if (CastInst *ci = dyn_cast<CastInst>(leakData)) {
					User *cu = cast<User>(ci);
					leakData = cu->getOperand(0);
					continue;
				}	
				if (LoadInst *li = dyn_cast<LoadInst>(leakData)) {
					Value *memLoc = li->getPointerOperand();
					leakData = store->find(memLoc);
					assert(leakData != NULL && "memLoc not in storeCollect");
					continue;
				}
				if (GetElementPtrInst *gp =
				  dyn_cast<GetElementPtrInst>(leakData)) {
					leakData = gp->getPointerOperand();
					continue;
				}
				if (isa<Argument>(leakData)) {
					errs() << "Data leaked escaped function analysis, unknown result.\n";
					break;
				}
				errs() << "Unhandled:\n    ";
				leakData->dump();
				assert(0 == 1);
			}
		}
	}
	delete store;
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
