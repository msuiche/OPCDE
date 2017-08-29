#ifndef	__CGSSKEL_H
#define	__CGSSKEL_H

struct CGSSkel : public CallGraphSCCPass {
	/*
	 * For all of your passes you will need this and to define it.
	 * It's address is used by pass system, so the value does not matter.
	 */
	static char ID;

	CGSSkel() : CallGraphSCCPass(ID) {
	}

	// Return true if Module was modified, otherwise false. 
	virtual bool runOnSCC(CallGraphSCC &);

	/*
	 * Used to help order passes by pass manager.
	 * Declare any passes you need run prior here.. as well as
	 * any information such as preserving CFG or similar. 
	 */
	virtual void getAnalysisUsage(AnalysisUsage &) const;

	virtual bool doInitialization(CallGraph &CG);
	virtual bool doFinalization(CallGraph &);
};

#endif
