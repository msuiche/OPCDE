#ifndef	__LPSKEL_H
#define	__LPSKEL_H

struct LPSkel : public LoopPass {
	/*
	 * For all of your passes you will need this and to define it.
	 * It's address is used by pass system, so the value does not matter.
	 */
	static char ID;

	LPSkel() : LoopPass(ID) {
	}

	// Return true if Function was modified, otherwise false. 
	virtual bool runOnLoop(Loop *L, LPPassManager &LP);

	/*
	 * Used to help order passes by pass manager.
	 * Declare any passes you need run prior here.. as well as
	 * any information such as preserving CFG or similar. 
	 */
	virtual void getAnalysisUsage(AnalysisUsage &) const;

	virtual bool doInitialization(Loop *L, LPPassManager &LPM);
	virtual bool doFinalization();
};

#endif
