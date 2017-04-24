/*
 * There are methods that are much better than what I am doing. There are
 * aggressive methods such as Andersen's Alias Analysis and there are 
 * lazy methods such as making use of the MemoryDependenceAnalysis API.
 *
 * What is going on here is quite basic and will miss many things.
 */
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"

#include <iostream>
#include <map>

using namespace llvm;

#include "StoreCollector.h"

void
StoreCollector::collect(Function *f)
{
	collectedFunction = f;
	if (storeMap.empty() == false) {
		storeMap.clear();
	}
	for (auto ii = inst_begin(*f); ii != inst_end(*f); ++ii) {
		Instruction *in = &*ii;
		if (!isa<StoreInst>(in)) {
			continue;
		}
		StoreInst *s = cast<StoreInst>(in);
		Value *storedVal = s->getValueOperand();
		Value *storedLoc = s->getPointerOperand();
		storeMap[storedLoc] = storedVal;
		continue;
	}
}
