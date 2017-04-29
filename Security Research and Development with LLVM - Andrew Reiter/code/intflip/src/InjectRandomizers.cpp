#include "llvm/IR/Module.h"

#include "ReplaceRandomizer.h"
#include "BitFlipRandomizer.h"

using namespace llvm;

struct InjectRandomizers : public ModulePass {
	static char ID;

	InjectRandomizers() : ModulePass(ID) {}

	virtual bool
	runOnModule(Module &M)
	{
		ReplaceRandomizer::inject(M);
		BitFlipRandomizer::inject(M);
		return true;
	}
};

char InjectRandomizers::ID = 0;
static RegisterPass<InjectRandomizers> XX("inject-randomizers", "Inject randomizer functions");
