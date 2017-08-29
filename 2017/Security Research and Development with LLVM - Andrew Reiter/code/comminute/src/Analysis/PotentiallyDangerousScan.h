#ifndef __POTENTIALLYDANGEROUSSCAN_H
#define	__POTENTIALLYDANGEROUSSCAN_H

struct PotentiallyDangerousScan : public ModulePass {
private:
	std::string _cfgFilePath;

public:
	static char ID;

	PotentiallyDangerousScan() : ModulePass(ID) { }
	virtual bool runOnModule(Module &);
	virtual void getAnalysisUsage(AnalysisUsage &) const;

	void setConfigFilePath(std::string s) { _cfgFilePath = s; }
	std::string getConfigFilePath() { return _cfgFilePath; }
};

#endif
