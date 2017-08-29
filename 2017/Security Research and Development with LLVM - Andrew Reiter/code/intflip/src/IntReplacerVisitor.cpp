/*
 * This is one of three examples of integer replacer pass.
 * In this example, we make use of the visitor patterned 
 * API.. which allows us to implement an InstVisitor which
 * let's avoid the if statment checking of instructions 
 * that you can see in the second integer replacer pass
 * 
 * We implement visitCallSite() which handles both CallInst and
 * InvokeInst cases as one.
 * 
 */
 

#include "llvm/IR/Module.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/IR/CallSite.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/CommandLine.h"

#include "TypeValueSupport.h"
#include "BaseRandomizer.h"

#include <json/value.h>
#include "FlipConfig.h"

#include <iostream>
#include <map>

using namespace llvm;

cl::opt<std::string> ReplaceConfigFileName("repcfg", cl::desc("<replacer cfg>"));

class CodeModificationLocation {
private:
	Instruction	*callLocation;
	Type        *argumentType;
	Value       *argumentValue;
	unsigned     argumentIdx;

public:
	CodeModificationLocation(Instruction *c, Type *t, Value *v, unsigned i) : callLocation(c), argumentType(t), argumentValue(v), argumentIdx(i) { };
	CallSite getCallSite() { return CallSite(callLocation); }
	Type *getArgumentType() { return argumentType; }
	Value *getArgumentValue() { return argumentValue; }
	unsigned getArgumentIdx() { return argumentIdx; }
};

struct IntReplacerVisitor: public ModulePass {
	static char ID;

	std::vector<CodeModificationLocation>	modificationList;
	FlipConfig *_zConfig;

	IntReplacerVisitor() : ModulePass(ID)
	{
		_zConfig = new FlipConfig(ReplaceConfigFileName);
	}	

	virtual bool
	runOnModule(Module &M)
	{
		for (auto mIt = _zConfig->getReplaceMap().begin();
		  mIt != _zConfig->getReplaceMap().end(); ++mIt) {

			std::string fnName = mIt->first;
			FunctionConfig mFc = mIt->second;
			if (mFc.shouldAnalyze == false) {
				errs() << "Per config: skipping analysis of " << fnName << " \n";
				continue;
			}

			Function *f = M.getFunction(fnName);
			if (f == NULL) {
				errs() << "Can't find function: " << fnName << " \n";
				continue;
			}
			
			/*
			 * The way this currently works -- you can do it many ways -- but the 
			 * way this works is that the visit finds any/all call instructions in
			 * the target function. There, it looks to see if there are any integer
			 * arguments that we want to replace with a randomizer.. if there are,
			 * we save them to a list of locations to change. Once gathering that
			 * list, we iterate and modify. 
			 */
			CheckCallInsts cCheck;
			cCheck.setIntReplacerVisitor(this);
			cCheck.visit(*f);
	
			for (auto iml = modificationList.begin();
			  iml != modificationList.end(); ++iml) {
				CodeModificationLocation cLoc = *iml;
				CallSite cs = cLoc.getCallSite();
				IntegerType *t = cast<IntegerType>(cLoc.getArgumentType());
				unsigned nBits = t->getBitWidth();

			/* Lookup randomizer function to use */
				std::string rndtype = "";
				if (mFc.randomType == 0) {
					rndtype = "rep";
				} else {
					rndtype = "bitflip";
				}
				std::string rndFnName = "__" + rndtype + "_randomizer_i" \
				  + std::to_string(nBits) + "__";

				// the randomizers should already be in, so we assert.
				Function *insertedRndFn = cs.getParent()->getModule()->getFunction(rndFnName);	
				assert(insertedRndFn != NULL);

				// Base mean value on what the configuration says
				ConstantInt *mn = ConstantInt::get(M.getContext(), APInt(32, mFc.mean, false));

				CallInst *callNewFunc = CallInst::Create(insertedRndFn,
				  { cLoc.getArgumentValue(), mn }, // Arguments are the integer to maybe flip and the mean value
				  "__rnd_replicant_",
				  cs.getInstruction());	// insert our call to the rnd fn before the targeted call instruction

				cs.setArgument(cLoc.getArgumentIdx(), callNewFunc);
			}
			modificationList.clear();
		}
		return true;
	}

	struct CheckCallInsts : public InstVisitor<CheckCallInsts> {
		IntReplacerVisitor *__ziInst;

		void
		setIntReplacerVisitor(IntReplacerVisitor *p)
		{
			__ziInst = p;
		}

		/*
		 * Could do this with two separate functions visitCallInst() and
		 * visitInvokeInst().. the difference is one is in the exception handling
	 	 * case and tthe other (the former) is for normal calling contexts.
		 * CallSite provides a reasonable common ground for the two.
		 *
		 */
		void
		visitCallSite(CallSite callSite)
		{
			/*
			 * Skip called functions that are on the skip_replace list.
			 */
			Function *called = callSite.getCalledFunction();
			if (!called->hasName()) {
				return;
			}
			if (std::find(skip_replace_functions.begin(),
				 skip_replace_functions.end(),
				 called->getName()) != skip_replace_functions.end()) {
				errs() << "Skipping replace function: " << called->getName() << "\n";
				return;
			}

			/*
			 * For each argument, determine if it of type and value that is replaceable.
		  	 * At this point, we just note the location by storing CallSite, Type,
			 * Value, and an arg index as a place to change.
			 *
			 */
			unsigned numArgOps = callSite.getNumArgOperands();
			for (unsigned ii = 0; ii < numArgOps; ii++) {
				Value *va = callSite.getArgOperand(ii);
				Type *ta = va->getType();
				if (TypeValueSupport::isReplaceable(ta, va)) {
					CodeModificationLocation ml = CodeModificationLocation(
					  callSite.getInstruction(),
					  ta,
					  va,
					  ii);
					__ziInst->modificationList.push_back(ml);
				}
			}
		}
	private:
	};
};

char IntReplacerVisitor::ID = 0;
static RegisterPass<IntReplacerVisitor> XX("replace-ints-visitor", "Replace int function args with randomizer-integers using instruction visitor");
