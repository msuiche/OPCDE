#ifndef __CHOOSEPHIVALUE_H
#define	__CHOOSEPHIVALUE_H

struct ChoosePhiValue : public ModulePass {
private:
	unsigned edgeIndex;
	void attemptUserReduction(BasicBlock *bb);

public:
	static char ID;
	ChoosePhiValue() : ModulePass(ID) { }
	virtual bool runOnModule(Module &);

	// phi [ a, b], [ c, d] ....
	// this gets us [a, b] if edgeIndex = 0.
	void setEdgeIndex(unsigned e) {
		edgeIndex = e;
	}
	unsigned getEdgeIndex() {
		return edgeIndex;
	}
};
#endif
