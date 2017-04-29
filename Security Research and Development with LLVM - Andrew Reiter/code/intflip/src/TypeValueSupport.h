#ifndef	__TYPEVALUESUPPORT_H
#define	__TYPEVALUESUPPORT_H

class TypeValueSupport {

public:
	TypeValueSupport() {}

	static bool
	isLiftable(llvm::Value *v)
	{
		/* dyn_cast<>() will return NULL if cast fails */
		if (llvm::ConstantInt *c = llvm::dyn_cast<llvm::ConstantInt>(v)) {
			unsigned nBits = c->getBitWidth();
			switch (nBits) {
			case 64:
			case 32:
			case 16:
			case 8:
				return true;
			}
		}
		return false;
	}

	static bool
	isReplaceable(llvm::Type *t, llvm::Value *v)
	{

		/* isa<>() returns boolean as to whether can cast */
		if (llvm::isa<llvm::ConstantInt>(v) == true) {
			/*
			 * Maybe assert() and say you should run the lifting
			 * pass before this one? Or just report that...
			 */
			return false;
		}

		/* dyn_cast<>() will return NULL if cast fails */
		if (llvm::IntegerType *intType = llvm::dyn_cast<llvm::IntegerType>(t)) {	
			unsigned nBits = intType->getBitWidth();
			switch (nBits) {
			case 64:
			case 32:
			case 16:
			case 8:
				return true;
			}
		}
		return false;
	}
};
#endif	// !__TYPEVALUESUPPORT_H
