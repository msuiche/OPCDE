#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/CallSite.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#include "MPSkel.h"

void
MPSkel::getAnalysisUsage(AnalysisUsage &AU) const
{
	AU.setPreservesCFG(); 
}

bool
MPSkel::runOnModule(Module &M)
{

	/* Iterate through all functions in this module */
	for (auto &F : M) {
		std::string fname = "not named";
		if (F.hasName()) {
			fname = F.getName().str();
		}

		// If no uses, don't look further.
		if (F.user_empty()) {
			errs() << "Function (" << fname << ") not used.\n";
			continue;
		}	
		errs() << "Listing uses for function (" << fname << ")\n";
		for (auto uit = F.user_begin(); uit != F.user_end(); ++uit) {
			User *u = *uit;
			errs() << "   ";
			std::string pn = "";

			// Is this use a Call or Invoke instruction?
			if (isa<CallInst>(u) || isa<InvokeInst>(u)) {
				// It is, so let's use the common class CallSite
				CallSite cs(dyn_cast<Instruction>(u));

				// Instruction in a BasicBlock in a Function.
				Function *caller = cs.getParent()->getParent();
				if (caller->hasName()) {
					pn = caller->getName().str();
				} else {
					pn = "not named";
				}	
				errs() << pn << ": ";
			}

			// Just print out what Value is
			u->dump();
			// If has debug info, we should just dump that as well
		}
		errs() << "\n";
	}
	return false; // CFG did not change
}

/*
 * Register this pass to be made usable.
 * Needs the static ID initialized and the pass declaration given.
 */
char MPSkel::ID = 0;
static RegisterPass<MPSkel> XX("mpskel", "Module Pass Skeleton");

