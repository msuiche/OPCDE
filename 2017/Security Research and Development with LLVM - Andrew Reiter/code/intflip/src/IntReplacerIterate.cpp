
#include "llvm/IR/Module.h"
#include "llvm/Support/Casting.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/CallSite.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/CommandLine.h"

#include "TypeValueSupport.h"
#include "BaseRandomizer.h"

#include <json/value.h>
#include "FlipConfig.h"

#include <iostream>
#include <map>

using namespace llvm;

extern cl::opt<std::string> ReplaceConfigFileName;

struct IntReplacerIterate: public ModulePass {
	static char ID;
	FlipConfig *_zConfig;


	IntReplacerIterate() : ModulePass(ID)
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
				errs() << "Skipping analysis of " << fnName << " \n";
				continue;
			}

			Function *f = M.getFunction(fnName);
			if (f == NULL) {
				continue;
			}
		        	
			for (inst_iterator I = inst_begin(f), E = inst_end(f); I != E; ++I) {
				if (isa<CallInst>(&*I) || isa<InvokeInst>(&*I)) {
					CallSite cs(&*I);
					Function *called = cs.getCalledFunction();
					if (!called->hasName()) {
						continue;	// XXX Currently require functions to have names
					}

					/*
					 * Determine if the call is of a function we don't want to replace args of..
					 */
					if (std::find(skip_replace_functions.begin(),
				 		skip_replace_functions.end(),
						called->getName()) != skip_replace_functions.end()) {
						errs() << "Skipping: " << called->getName().str() << "\n";
						continue;	// Skip it.
					}

					/*
					 * Go through all the called function's arguments. See if
					 * any are supported by replacement.
					 */
					unsigned numArgOps = cs.getNumArgOperands();
					for (unsigned ii = 0; ii < numArgOps; ii++) {
						Value *va = cs.getArgOperand(ii);
						Type *ta = va->getType();

						/*
						 * If not a 8, 16, 32, or 64 bit integer, we skip it.
						 */
						if (TypeValueSupport::isReplaceable(ta, va) == false) {
							continue;
						}

						/*
						 * Based on configuration, choose the randomization method.
						*/
						std::string rndtype = "";
						if (mFc.randomType == 0) {
							rndtype = "rep";
						} else {
							rndtype = "bitflip";
						}
						unsigned nBits = ta->getIntegerBitWidth();
						std::string rndFnName = "__" + rndtype + "_randomizer_i" + std::to_string(nBits) + "__";

						/*
						 * The randomizer functions /should/ already be in the module, so get the handle.
						 */
						Function *insertedRndFn = M.getFunction(rndFnName);	
						assert(insertedRndFn != NULL);

						/*
						 * We allow different means for different functions.
						 */
						ConstantInt *mn = ConstantInt::get(M.getContext(), APInt(32, mFc.mean, false));

						/*
						 * Insert call to randomizer with input integer and a mean value.
						 * It will be inserted before the CallInst.
						 */
						CallInst *callNewFunc = CallInst::Create(insertedRndFn,
							{ va, mn }, // Arguments are the integer to maybe flip and the mean value
							"__rnd_replicant_",
							cs.getInstruction());	// insert our call to the rnd fn before the targeted call instruction

						/*
						 * Replace the old integer argument with the randomized one
						 */
						cs.setArgument(ii, callNewFunc);

					}
				} 
			}
		}	
		return true;
	}

};

char IntReplacerIterate::ID = 0;
static RegisterPass<IntReplacerIterate> XX("replace-ints-iterate", "Replace int function args with randomizer-integers using instruction iteration");
