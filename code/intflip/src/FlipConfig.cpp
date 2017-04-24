#include <string>
#include <map>

#include <iostream>
#include <fstream>

#include <json/config.h>
#include <json/value.h>
#include <json/reader.h>

#include "FlipConfig.h"

std::vector<std::string> skip_replace_functions = { 
    "__rep_randomizer_i64__",
    "__rep_randomizer_i32__",
    "__rep_randomizer_i16__",
    "__rep_randomizer_i8__",
    "__bitflip_randomizer_i64__",
    "__bitflip_randomizer_i32__",
    "__bitflip_randomizer_i16__",
    "__bitflip_randomizer_i8__",
	"__cxa_allocate_exception",
	"__cxa_throw" // ... There are many RT and other functions to skip... but so it goes.
};


FlipConfig::FlipConfig(std::string c)
{
	_configFile = c;
	std::ifstream cStream;
	cStream.open(_configFile);
	cStream >> _configDict;
	cStream.close();

	Json::Value globalConfig =_configDict["global"];   // XXX Not used yet
	Json::Value fnConfig = _configDict["functions"];
	Json::Value skipFn = _configDict["skip"];
	Json::Value::Members m = fnConfig.getMemberNames();
	for (auto iii = m.begin(); iii != m.end(); ++iii) {
		std::string name = *iii;

		Json::Value nv = fnConfig[name];
		Json::Value meanVal = nv["mean"];
		Json::Value typeVal = nv["type"];
		Json::Value analyzeVal = nv["analyze"];

		FunctionConfig fc{analyzeVal.asBool(), typeVal.asUInt(), meanVal.asUInt()};
		_replace[name] = fc;
	}
	if (skipFn.isArray()) {
		Json::ArrayIndex aLen = skipFn.size();
		for (Json::ArrayIndex ai = 0; ai < aLen; ai++) {
			skip_replace_functions.push_back(skipFn[ai].asString());
		}
	}
}

