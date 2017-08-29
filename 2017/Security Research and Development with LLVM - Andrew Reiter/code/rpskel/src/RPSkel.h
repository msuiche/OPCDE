#ifndef	__RPSKEL_H
#define	__RPSKEL_H

struct RPSkel : public RegionPass {
	/*
	 * For all of your passes you will need this and to define it.
	 * It's address is used by pass system, so the value does not matter.
	 */
	static char ID;

	RPSkel() : RegionPass(ID) {
	}

	// Return true if Region was modified, otherwise false. 
	virtual bool runOnRegion(Region *R, RGPassManager &RGM);

	/*
	 * Used to help order passes by pass manager.
	 * Declare any passes you need run prior here.. as well as
	 * any information such as preserving CFG or similar. 
	 */
	virtual void getAnalysisUsage(AnalysisUsage &) const;

	virtual bool doInitialization(Region *R, RGPassManager &RGM);
	virtual bool doFinalization();
};

#endif
