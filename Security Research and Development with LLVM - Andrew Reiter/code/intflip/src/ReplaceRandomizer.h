#ifndef __REPLACERANDOMIZER_H
#define	__REPLACERANDOMIZER_H

#include "BaseRandomizer.h"

class ReplaceRandomizer : BaseRandomizer {
public:
	static void inject(llvm::Module& M);
};
#endif	// !__REPLACERANDOMIZER_H
