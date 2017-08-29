#ifndef	__MPSKEL_H
#define	__MPSKEL_H

struct MPSkel : public ModulePass {
	/*
	 * For all of your passes you will need this and to define it.
	 * It's address is used by pass system, so the value does not matter.
	 */
	static char ID;

	MPSkel() : ModulePass(ID) { }

	// Called on each compilation unit
	virtual bool runOnModule(Module &);

	/*
	 * Used to help order passes by pass manager.
	 * Declare any passes you need run prior here.. as well as
	 * any information such as preserving CFG or similar. 
	 */
	virtual void getAnalysisUsage(AnalysisUsage &) const;

};

#endif
