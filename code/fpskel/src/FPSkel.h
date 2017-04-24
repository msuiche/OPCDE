#ifndef	__FPSKEL_H
#define	__FPSKEL_H

struct FPSkel : public FunctionPass {
	/*
	 * For all of your passes you will need this and to define it.
	 * It's address is used by pass system, so the value does not matter.
	 */
	static char ID;

	FPSkel() : FunctionPass(ID) { }

	// Called on each function in given compilation unit 
	virtual bool runOnFunction(Function &);

	/*
	 * Used to help order passes by pass manager.
	 * Declare any passes you need run prior here.. as well as
	 * any information such as preserving CFG or similar. 
	 */
	virtual void getAnalysisUsage(AnalysisUsage &) const;

};

#endif
