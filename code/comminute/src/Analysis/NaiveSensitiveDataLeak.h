#ifndef __NAIVESENSITIVEDATALEAK_H
#define	__NAIVESENSITIVEDATALEAK_H

struct NaiveSensitiveDataLeak : public ModulePass {
	static char ID;
	typedef	std::pair<Function *, unsigned> FuncArg;
	typedef std::map<std::string, FuncArg> FuncArgMap;

	NaiveSensitiveDataLeak() : ModulePass(ID) { }
	virtual bool runOnModule(Module &);
   	virtual void getAnalysisUsage(AnalysisUsage &) const;
private:
	void parseAndCheckConfig(FuncArgMap *, bool);
	void printResult(TargetCallSite *srcSite, TargetCallSite *snkSite);

};


#endif
