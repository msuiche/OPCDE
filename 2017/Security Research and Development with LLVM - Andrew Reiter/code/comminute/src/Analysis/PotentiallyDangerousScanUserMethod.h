#ifndef __POTENTIALLYDANGEROUSSCANUSERMETHOD_H
#define	__POTENTIALLYDANGEROUSSCANUSERMETHOD_H

struct PotentiallyDangerousScanUserMethod : public ModulePass {
private:
	std::string _cfgFilePath;

public:
	static char ID;

	PotentiallyDangerousScanUserMethod() : ModulePass(ID) { }
	virtual bool runOnModule(Module &);
	virtual void getAnalysisUsage(AnalysisUsage &) const;

	void setConfigFilePath(std::string s) { _cfgFilePath = s; }
	std::string getConfigFilePath() { return _cfgFilePath; }
};

#endif
