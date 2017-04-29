#ifndef __BASERANDOMIZER_H
#define	__BASERANDOMIZER_H

class BaseRandomizer {
public:
	static void inject(llvm::Module& M);
};

typedef	enum {
	Replace = 1,
	BitFlip = 2
} RandomizerKind;

#endif	// !__BASERANDOMIZER_H
