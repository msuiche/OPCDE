#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Analysis/RegionPass.h"
#include "llvm/Analysis/RegionInfo.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#include "RPSkel.h"

void
RPSkel::getAnalysisUsage(AnalysisUsage &AU) const
{
	// No changes to CFG, so tell the pass manager
	AU.setPreservesCFG(); 
}

bool
RPSkel::doFinalization()
{
	return false;
}

bool
RPSkel::doInitialization(Region *R, RGPassManager &RGPM)
{
	return false;
}

bool
RPSkel::runOnRegion(Region *R, RGPassManager &RGPM)
{
	errs() << "  Region found:\n";

	R->print(errs(), true, 1, Region::PrintNone);	
	errs() << "  --- end of Region ---\n";

	// return true if Region has been changed.
	return false;
}



/*
 * Register this pass to be made usable.
 * Needs the static ID initialized and the pass declaration given.
 */
char RPSkel::ID = 0;
static RegisterPass<RPSkel> XX("rpskel", "Region Pass Skeleton");

