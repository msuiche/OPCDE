#ifndef __POTENTIALLYDANGEROUSSCANFUNCTIONPASS_H
#define	__POTENTIALLYDANGEROUSSCANFUNCTIONPASS_H

struct PotentiallyDangerousScanFunctionPass : public FunctionPass {
private:
	std::string _cfgFilePath;
    std::vector<Function *> pdFunctions;
	bool lookupPDFunctions;


public:
	static char ID;

	PotentiallyDangerousScanFunctionPass() : FunctionPass(ID) {
		pdFunctions.clear();
		lookupPDFunctions = true;
	}
	virtual bool runOnFunction(Function &);
	virtual void getAnalysisUsage(AnalysisUsage &) const;

	std::string getConfigFilePath() { return _cfgFilePath; }
	void setConfigFilePath(std::string);
};

#endif
