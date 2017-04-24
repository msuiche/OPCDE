#ifndef __BITFLIPRANDOMIZER_H
#define	__BITFLIPRANDOMIZER_H

#include "BaseRandomizer.h"

class BitFlipRandomizer : BaseRandomizer {
public:
	static void inject(llvm::Module& M);
};

#endif	// !__BITFLIPRANDOMIZER_H
