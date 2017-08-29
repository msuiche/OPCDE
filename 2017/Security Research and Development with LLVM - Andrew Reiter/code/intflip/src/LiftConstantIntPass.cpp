/*
 * This is a module pass that likely will not have
 * impact given possibly run passes, but for our
 * purposes we should have it just in case.
 *
 * What it does is identifies call instructions that
 * are passed a integral constant to the target function.
 * We lift that constant into a local variable and 
 * adjust the function call to use the variable. 
 *
 * The reason for doing this is that the passes run after
 * this want to act on variables of integer type and so
 * this makes that possible to do without checking types..
 *
 */

#include "llvm/IR/Module.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/IR/CallSite.h"

#include "TypeValueSupport.h"

using namespace llvm;

struct LiftConstantIntPass : public ModulePass {
	static char ID;

	LiftConstantIntPass() : ModulePass(ID) {}

	virtual bool
	runOnModule(Module &M)
	{
		bool modified = false;

		/*
		 * Visit the call instructions of each function in this
		 * module.
		 */
		for (auto &f : M) {
			
			CheckCallInsts cCheck;
			cCheck.visit(f);
			if (cCheck.modified) {
				modified = true;
			}
		}
		return modified;
	}

	struct CheckCallInsts : public InstVisitor<CheckCallInsts> {
		bool modified = false;

		void
		visitCallSite(CallSite callSite)
		{
			unsigned numArgOps = callSite.getNumArgOperands();
			unsigned argIdx;

			/*
			 * Check all arguments to the called function to see
		     * they are a constant integer of a size we can lift. 
			 * 
			 * If an argument is liftable, we allocated space for it
			 * and store the constant to it. Then, load value and use
 			 * it to replace the constant integer argument to the
			 * function called.
			 */
			for (argIdx = 0; argIdx < numArgOps; argIdx++) {
				Value *va = callSite.getArgOperand(argIdx);

				if (TypeValueSupport::isLiftable(va) == true) {
					ConstantInt *con = cast<ConstantInt>(va);

					unsigned nBits = con->getBitWidth();	

					AllocaInst *localized__alloc = new AllocaInst(
						IntegerType::get(callSite.getParent()->getContext(), nBits),	// type to allocate
						"__intflip_localized",	// give the slot a label 
						callSite.getInstruction());	// Insert before call instruction

					StoreInst *localized__store = new StoreInst(
						con,	// value to store
						localized__alloc,  // where to store it
						callSite.getInstruction());	// Insert before call instruction

  					LoadInst *localized__load = new LoadInst(
						localized__alloc,	// pointer to load from
						(const char *)"__intflip_loaded",	// label the slot
						callSite.getInstruction());	// Insert before call instruction

					/*
					 * after that series we now have:
					 *  alloca
					 *  store
					 *  load
					 *  call
					 * sequence of instructions
					 */

					/* replace the constant in the function call */
					callSite.setArgument(argIdx, localized__load);
					new_vars++;
					modified = true;
				}
			}
		}
	private:
		unsigned new_vars = 0;
	};
};

char LiftConstantIntPass::ID = 0;
static RegisterPass<LiftConstantIntPass> XX("lift-constant-int-args", "Lifts constant int fn args to a local variable");
