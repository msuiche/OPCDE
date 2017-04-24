#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#include "BBSkel.h"

void
BBSkel::getAnalysisUsage(AnalysisUsage &AU) const
{
	// No changes to CFG, so tell the pass manager
	AU.setPreservesCFG(); 
}

bool
BBSkel::doFinalization(Function &F)
{
	errs() << "#BBs: " << bbcount << "\n";
	errs() << "#Is: " << icount << "\n";
	return false;
}

bool
BBSkel::runOnBasicBlock(BasicBlock &B)
{
	bbcount++;
	errs() << "  Basic Block found:\n";
	for (auto &I : B) { // Iterate through instructions in the block
		++icount;
		//  Note in output if instruction is a call/invoke
		if (isa<CallInst>(I) || isa<InvokeInst>(I)) {
			errs() << " C ";
		} else {
			errs() << "   ";
		}
		I.dump();
		errs() << "    used by:\n";	
		// Go through and dump the uses for each instruction.
		for (auto ui = I.user_begin(); ui != I.user_end(); ++ui) {
			errs() << "     U: ";
			ui->dump();
		}
		errs() << " ~~~~ \n";
	}
	errs() << "  --- end of basic block ---\n";

	// return true if CFG has changed.
	return false;
}



/*
 * Register this pass to be made usable.
 * Needs the static ID initialized and the pass declaration given.
 */
char BBSkel::ID = 0;
static RegisterPass<BBSkel> XX("bbskel", "BasicBlock Pass Skeleton");

