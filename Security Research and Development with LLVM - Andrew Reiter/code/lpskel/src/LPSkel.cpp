#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#include "LPSkel.h"

void
LPSkel::getAnalysisUsage(AnalysisUsage &AU) const
{
	// No changes to CFG, so tell the pass manager
	AU.setPreservesCFG(); 
}

bool
LPSkel::doFinalization()
{
	return false;
}

bool
LPSkel::doInitialization(Loop *L, LPPassManager &LP)
{
	return false;
}

bool
LPSkel::runOnLoop(Loop *L, LPPassManager &LP)
{
	errs() << "  Loop found:\n";
	L->print(errs(), 1);
	errs() << "  --- end of Loop ---\n";

	// return true if Function has been changed.
	return false;
}

/*
 * Register this pass to be made usable.
 * Needs the static ID initialized and the pass declaration given.
 */
char LPSkel::ID = 0;
static RegisterPass<LPSkel> XX("lpskel", "Loop Pass Skeleton");

