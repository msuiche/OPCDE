#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Analysis/CallGraphSCCPass.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#include "CGSSkel.h"

void
CGSSkel::getAnalysisUsage(AnalysisUsage &AU) const
{
	// No changes to CFG, so tell the pass manager
	AU.setPreservesCFG(); 
}

bool
CGSSkel::doFinalization(CallGraph &G)
{
	return false;
}

bool
CGSSkel::doInitialization(CallGraph &G)
{
	return false;
}

bool
CGSSkel::runOnSCC(CallGraphSCC &GSCC)
{
	errs() << "  Strongly connected component found:\n";
	
	if (GSCC.isSingular()) {
		errs() << "    SCC is singular\n";
	}
	for (auto &G : GSCC) {
		errs() << "    Dump:\n";
		G->dump();
	}
	errs() << "  --- end of SCC ---\n";

	// return true if Module has been changed.
	return false;
}



/*
 * Register this pass to be made usable.
 * Needs the static ID initialized and the pass declaration given.
 */
char CGSSkel::ID = 0;
static RegisterPass<CGSSkel> XX("cgsskel", "CallGraphSCC Pass Skeleton");

