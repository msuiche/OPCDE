#ifndef __TARGETCALLSITESPASS_H
#define	__TARGETCALLSITESPASS_H

// Probably should just derive from CallSite, but so it goes.
class TargetCallSite {
	CallSite callSite;
	int argOperandIndex;

public:
	TargetCallSite(Instruction *c, int i) : callSite(c), argOperandIndex(i) {}
	~TargetCallSite() { }

	int getArgIndex() {
		return argOperandIndex;
	}

	Instruction *getInstruction() {
		return callSite.getInstruction();
	}

	Function *getCaller() {
		return callSite.getCaller();
	}

	Function *getCalled() {
		return callSite.getCalledFunction();
	}

	Value *getTarget() {
		if (argOperandIndex == -1) {
			return callSite.getInstruction();
		}
		return callSite.getArgOperand(argOperandIndex);
	}
};

struct TargetCallSitesPass : public ModulePass {
	static char ID;

	typedef enum _TargetCallType {
		SinkCall,
		SourceCall
	} TargetCallType;

	TargetCallSitesPass() : ModulePass(ID) {
	// There is probably a better, c++-ier way to do this with templates
	// or something.
		targetConfigMap[SinkCall] = "";
		targetCallMap[SinkCall].reserve(0);
		targetConfigMap[SourceCall] = "";
		targetCallMap[SourceCall].reserve(0);
	}
	~TargetCallSitesPass() {
		for (auto k = targetCallMap.begin(); k != targetCallMap.end(); ++k) {
			targetCallMap[k->first].clear();
		}
	}

	virtual bool runOnModule(Module &);
   	virtual void getAnalysisUsage(AnalysisUsage &) const;

	void setConfig(TargetCallType ty, std::string path) {
		targetConfigMap[ty] = path;
	}

	typedef std::vector<std::unique_ptr<TargetCallSite>> TargetVector;
	typedef TargetVector::const_iterator iterator;

	iterator src_begin() {
		return targetCallMap[SourceCall].begin();
	}
	iterator src_end() {
		return targetCallMap[SourceCall].end();
	}
	bool src_empty() {
		return targetCallMap[SourceCall].empty();
	}
	iterator src_erase(iterator pos) {
		return targetCallMap[SourceCall].erase(pos);
	}
	
	iterator snk_begin() {
		return targetCallMap[SinkCall].begin();
	}
	iterator snk_end() {
		return targetCallMap[SinkCall].end();
	}
	bool snk_empty() {
		return targetCallMap[SinkCall].empty();
	}
	iterator snk_erase(iterator pos) {
		return targetCallMap[SinkCall].erase(pos);
	}

private:
	void parseConfig(std::string, TargetCallType, Module *);
	std::map<TargetCallType, std::string> targetConfigMap;
	std::map<TargetCallType, TargetVector> targetCallMap;
};


#endif
