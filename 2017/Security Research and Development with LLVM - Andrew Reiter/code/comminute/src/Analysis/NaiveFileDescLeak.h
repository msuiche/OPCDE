#ifndef __NAIVEFILEDESCLEAK_H
#define	__NAIVEFILEDESCLEAK_H

struct NaiveFileDescLeak : public ModulePass {
public:
	static char ID;

	typedef	std::pair<Function *, unsigned> FuncArg;
	typedef std::map<std::string, FuncArg> FuncArgMap;

	NaiveFileDescLeak() : ModulePass(ID) { }
	virtual bool runOnModule(Module &);
   	virtual void getAnalysisUsage(AnalysisUsage &) const;

	void printLeak(TargetCallSite *s);
	void printLikelyFP(TargetCallSite *s, std::string r);

private:
};


#endif
