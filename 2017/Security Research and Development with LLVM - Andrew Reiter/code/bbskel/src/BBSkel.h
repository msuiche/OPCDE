#ifndef	__BBSKEL_H
#define	__BBSKEL_H

struct BBSkel : public BasicBlockPass {
	/*
	 * For all of your passes you will need this and to define it.
	 * It's address is used by pass system, so the value does not matter.
	 */
	static char ID;

	unsigned bbcount;
	unsigned icount;

	BBSkel() : BasicBlockPass(ID) {
		bbcount = 0;
		icount = 0;
	}

	// Called on each BasicBlock in given compilation unit 
	virtual bool runOnBasicBlock(BasicBlock &);

	/*
	 * Used to help order passes by pass manager.
	 * Declare any passes you need run prior here.. as well as
	 * any information such as preserving CFG or similar. 
	 */
	virtual void getAnalysisUsage(AnalysisUsage &) const;

	/*
	 * Called after each exec of a runOnBasicBlock.
	 */
	virtual bool doFinalization(Function &);
};

#endif
