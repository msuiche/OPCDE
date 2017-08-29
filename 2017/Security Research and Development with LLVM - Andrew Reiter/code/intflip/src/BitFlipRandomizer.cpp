#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"

#include "BitFlipRandomizer.h"

using namespace llvm;

/*
 * This macro helps with the varying bit cases. It generates the code
 * that generates the code where K = nBits is a value:
 *
 * intK_t
 * __bitflip_randomize_iK(intK inArg0)
 * {
 *   unsigned rv = arc4random();
 *   if (rv <= (unsigned)2^31) {
 *     rv = 1 << (rv % K); 
 *     return inArg0 ^ rv;
 *   } 
 *   return inArg0;
 * }
 *
 * So this says get a random number and if it is less than some mean
 * then we flip a random bit of the input integer and return it.
 * Otherwise, we just return it unscathed.
 *
 */
#define injectBFIntN(nBits) \
	static void  																			\
	injectInt##nBits(llvm::Module& M, Function *fnRand) 									\
	{ 																						\
		/*																					\
		 * Add the function intN_t __bitflip_randomizer_iN__(intN_t, int32_t) 				\
		 */																					\
		LLVMContext& ctx = M.getContext();  												\
		/*																					\
		 * define i8 @__bitflip_randomizer_i8__(i8 %intToFlip, i32 %meanValue) {			\
		 * }																				\
		 */																					\
		std::string int##nBits##_rand = "__bitflip_randomizer_i" #nBits "__"; 				\
		Constant *cTmp = M.getOrInsertFunction(int##nBits##_rand, 							\
			Type::getInt##nBits##Ty(ctx), 													\
			Type::getInt##nBits##Ty(ctx), 													\
			Type::getInt32Ty(ctx), 															\
			NULL);  																		\
		Function *bf_i##nBits = cast<Function>(cTmp);  										\
		bf_i##nBits->setCallingConv(CallingConv::C); 										\
																							\
		Argument& inArg0 = bf_i##nBits->getArgumentList().front(); 							\
		inArg0.setName("intToFlip"); 														\
		Argument& inArg1 = bf_i##nBits->getArgumentList().back(); 							\
		inArg1.setName("meanValue"); 														\
    																						\
		/*																					\
		 * entry:																			\
		 * bf_it:																			\
		 * return:																			\
		 */																					\
		BasicBlock *blkEntry = BasicBlock::Create(ctx, "entry", bf_i##nBits);  				\
		BasicBlock *blkBitFlipIt = BasicBlock::Create(ctx, "bf_it", bf_i##nBits); 			\
		BasicBlock *blkReturn = BasicBlock::Create(ctx, "return", bf_i##nBits);  			\
		/*																					\
		 * entry:																			\
		 *  %__bf_rand_ = call i32 @arc4random()											\
		 *  %__bf_lessthan_ = icmp ule i32 %__bf_rand_, %meanValue							\
		 *  br i1 %__bf_lessthan_, label %bf_it, label %return								\
		 */																					\
		IRBuilder<> builder(blkEntry);  													\
		Value *callArc4Random = builder.CreateCall(fnRand, None, "__bf_rand_", nullptr);  	\
		Value *lessThan = builder.CreateICmpULE(callArc4Random, &inArg1, "__bf_lessthan_"); \
		Value *branchBitFlip = builder.CreateCondBr(lessThan, blkBitFlipIt, blkReturn); 	\
																							\
		/*																					\
		 * bf_it:                                            ; preds = %entry				\
		 *  %__bf_bitflip_ = urem i32 %__bf_rand_, 8										\
		 *  %__bf_cast_randrem_ = trunc i32 %__bf_bitflip_ to i8							\
		 *  %__bf_shifted_bit_ = shl i8 1, %__bf_cast_randrem_								\
		 *  %__bf_xord_retval_ = xor i8 %intToFlip, %__bf_shifted_bit_						\
	 	 *  ret i8 %__bf_xord_retval_														\
		 */																					\
		builder.SetInsertPoint(blkBitFlipIt); 												\
		Value *randModulus = ConstantInt::get(IntegerType::get(ctx, 32), nBits, false); 	\
		Value *randRemainder = builder.CreateURem(callArc4Random, randModulus,				\
		    "__bf_bitflip_");  																\
		Value *defaultBit = ConstantInt::get(IntegerType::get(ctx, nBits), 1, false);   	\
		Value *castRandRem = builder.CreateZExtOrTrunc(randRemainder,						\
		    Type::getInt##nBits##Ty(ctx), "__bf_cast_randrem_");  							\
		Value *shiftedBit = builder.CreateShl(defaultBit, castRandRem,						\
		    "__bf_shifted_bit_");   														\
		Value *xordReturnVal = builder.CreateXor(&inArg0, shiftedBit, "__bf_xord_retval_");	\
		builder.CreateRet(xordReturnVal);  													\
																							\
		/*																					\
		 * return:                                           ; preds = %entry				\
		 *  ret i8 %intToFlip																\
		 */																					\
		builder.SetInsertPoint(blkReturn);  												\
		builder.CreateRet(&inArg0);  														\
	} 																						\



injectBFIntN(64)
injectBFIntN(32)
injectBFIntN(16)
injectBFIntN(8)

void
BitFlipRandomizer::inject(Module& M)
{
	LLVMContext& ctx = M.getContext();

	/*
	 * If arc4random is already in the module, then return the Function for it; otherwise, 
	 * declare it so it will be pulled in. It should be within scope, so we assert.
	 */
	Constant *lookupRand = M.getOrInsertFunction("arc4random", Type::getInt32Ty(ctx), NULL);
	assert(lookupRand != NULL && "BitFlipRandomizer::inject: Unable to getFunction(arc4random)\n");

	Function *fnRand = cast<Function>(lookupRand);

	injectInt64(M, fnRand);
	injectInt32(M, fnRand);
	injectInt16(M, fnRand);
	injectInt8(M, fnRand);
}
