#ifndef	_NAIVECONSTANTARGCHECK_H
#define	_NAIVECONSTANTARGCHECK_H

class NaiveConstantArgCheckResult {
	Function	*caller;
	Function	*callee;
	Value		*argument;	
	unsigned	 argIndex;
	DILocation	*loc;

public:
	NaiveConstantArgCheckResult(Function *aCaller,
	  Function *aCallee, Value *arg, unsigned idx, DILocation *d) :
      caller(aCaller), callee(aCallee), argument(arg), argIndex(idx), loc(d) {};

	Function	*getCaller() { return caller; }
	Function	*getCallee() { return callee; }
	Value		*getArgument() { return argument; }
	unsigned	getArgumentIndex() { return argIndex; }
	bool	hasLocation() { 
		if (loc == NULL) {
			return false;
		}
		return true;
	}
	DILocation	*getLocation() { return loc; }
	void printResult();
};

struct NaiveConstantArgCheck : public ModulePass {
private:
	std::vector<NaiveConstantArgCheckResult>	_results;	
	std::string configFilePath;

public:
	static char ID;

	NaiveConstantArgCheck() : ModulePass(ID) {}

	virtual bool	runOnModule(Module &M);
	virtual void	getAnalysisUsage(AnalysisUsage &) const;

	void setConfigFilePath(std::string a) { configFilePath = a; }
	std::string getConfigFilePath() { return configFilePath; }	
};
#endif	// !_CONSTANTARGCHECK_H
