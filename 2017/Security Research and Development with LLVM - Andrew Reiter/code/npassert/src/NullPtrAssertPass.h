#ifndef	__NULLPTRASSERTPASS_H
#define	__NULLPTRASSERTPASS_H

struct NullPtrAssertPass : public ModulePass {
	static char ID;

	NullPtrAssertPass() : ModulePass(ID) {  }
	virtual bool runOnModule(Module &);

private:
	void insertAssertion(Module *M, Function *F, Argument *A);
	bool attemptInsertAssert(Module *M, Function *F, int);
	void insertAssertionFunction(Module *M);
	void insertAssertionFunctionCall(Module *M, Function *F, Argument *A);

	Function *assertFn;
};

#endif
