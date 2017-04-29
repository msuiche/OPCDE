#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#include "FPSkel.h"

void
FPSkel::getAnalysisUsage(AnalysisUsage &AU) const
{
	AU.setPreservesCFG(); 
}


bool
FPSkel::runOnFunction(Function &F)
{
	unsigned nbb = 0;
	unsigned ins = 0;

	if (F.isDeclaration()) {
		errs() << "Ignoring function declaration.\n";
		return false;
	}
	if (F.hasName()) {
		errs() << "\nFunction: " << F.getName() << "\n";
	} else {
		errs() << "\nFunction: not named\n";
	}

	for (auto &B : F) {	// Iterate through Basic Blocks in a function
		++nbb;
		errs() << "  Basic Block found:\n";
		B.dump();
		for (auto &I : B) { // Iterate through instructions in the block
			++ins;
		}
		errs() << "  --- end of basic block ---\n";
	}
	errs() << "  Total of " << nbb << " blocks in this function\n";
	errs() << "  Total of " << ins << " instructions in this function\n";
	errs() << "---  end of function  ---\n";

	// return true if CFG has changed.
	return false;
}

/*
 * Register this pass to be made usable with -fpskel option.
 * Needs the static ID initialized and the pass declaration given.
 */
char FPSkel::ID = 0;
static RegisterPass<FPSkel> XX("fpskel", "Function Pass Skeleton");

