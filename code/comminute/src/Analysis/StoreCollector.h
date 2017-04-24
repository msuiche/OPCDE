#ifndef __STORECOLLECTOR_H
#define	__STORECOLLECTOR_H

class StoreCollector {
	// key is pointer and value is the stored value
	std::map<Value *, Value *>	storeMap;
	Function *collectedFunction;

public:
	StoreCollector() {
		collectedFunction = NULL;
	}
	void collect(Function *);
	Function *getFunction() {
		return collectedFunction;
	}
	
	Value *find(Value *storeLoc) {
		auto s = storeMap.find(storeLoc);
		if (s == storeMap.end()) {
			return NULL;
		}
		return s->second;
	}
};
#endif
