#ifndef	__FLIPCONFIG_H
#define	__FLIPCONFIG_H

struct FunctionConfig {
    bool shouldAnalyze;
    unsigned randomType;    // weak, i know. 0 = full replace, 1 = bit replace
    unsigned mean;          // replace int, if random value is <= mean

    FunctionConfig() { shouldAnalyze = false; randomType = 0; mean = 2828282; }
    FunctionConfig(bool a, unsigned t, unsigned m) : shouldAnalyze(a), randomType(t), mean(m) {}
};

typedef	std::map<std::string, FunctionConfig> ReplaceMap;

class FlipConfig {
private:
	std::string	_configFile;
	Json::Value _configDict;
	ReplaceMap _replace;

public:
	FlipConfig(std::string c);
	Json::Value getDict() { return _configDict; }
	ReplaceMap getReplaceMap() { return _replace; }	
};

extern std::vector<std::string> skip_replace_functions;

#endif	// !__FLIPCONFIG_H
