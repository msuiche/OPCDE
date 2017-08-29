#ifndef __FUNCTIONEXTERNALIZER_H
#define	__FUNCTIONEXTERNALIZER_H

struct FunctionExternalizer : public ModulePass {
private:
	std::string	_functionListFile;

public:
	static char ID;
	FunctionExternalizer() : ModulePass(ID) { }
	virtual bool runOnModule(Module &);
	void	setFunctionListFilePath(std::string a) { this->_functionListFile = a; }
	std::string getFunctionListFilePath() { return this->_functionListFile; }
};

#endif
