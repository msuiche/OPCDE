#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"

#include "ReplaceRandomizer.h"

using namespace llvm;
/*

 * This, for each K bits integer type, will inject the function
 *
 * intK_t
 * __rep_randomizer_iK(intK_t inArg0)
 * {
 *    unsigned rv = arc4random();
 *    if (rv <= (unsigned)meanValue) {
 *        return rv;
 *    }
 *    return inArg0
 * }
 *
 * The comments in the macro below show the generated IR along each step..but for
 * K=64.
 */
#define injectRIntN(nBits) \
	static void  																			\
	injectInt##nBits(llvm::Module& M, Function *fnRand) 									\
	{ 																						\
		LLVMContext& ctx = M.getContext();  												\
		/*																					\
		 * define i64 @__rep_randomizer_i64__(i64 %intToFlip, i32 %meanValue) {				\
		 * }																				\
		 */																					\
		std::string int##nBits##_rand = "__rep_randomizer_i" #nBits "__"; 					\
		Constant *cTmp = M.getOrInsertFunction(int##nBits##_rand, 							\
			Type::getInt##nBits##Ty(ctx), 													\
			Type::getInt##nBits##Ty(ctx), 													\
			Type::getInt32Ty(ctx), 															\
			NULL);  																		\
		Function *rep_i##nBits = cast<Function>(cTmp);  									\
		rep_i##nBits->setCallingConv(CallingConv::C); 										\
		Argument& inArg0 = rep_i##nBits->getArgumentList().front(); 						\
		inArg0.setName("intToFlip"); 														\
		Argument& inArg1 = rep_i##nBits->getArgumentList().back(); 							\
		inArg1.setName("meanValue"); 														\
		/*																					\
		 * entry:																			\
	 	 * replace_it:																		\
		 * return:																			\
		 */																					\
		BasicBlock *blkEntry = BasicBlock::Create(ctx, "entry", rep_i##nBits);  			\
		BasicBlock *blkRepIt = BasicBlock::Create(ctx, "replace_it", rep_i##nBits); 		\
		BasicBlock *blkReturn = BasicBlock::Create(ctx, "return", rep_i##nBits);  			\
		/*																					\
		 * entry:																			\
		 *  %__rep_rand_ = call i32 @arc4random()											\
		 *  %__rep_lessthan_ = icmp ule i32 %__rep_rand_, %meanValue						\
		 *  br i1 %__rep_lessthan_, label %replace_it, label %return						\
		 * replace_it:																		\
		 * return:																			\
		 */																					\
		IRBuilder<> builder(blkEntry);  													\
		Value *callArc4Random = builder.CreateCall(fnRand, None, "__rep_rand_", nullptr);  	\
		Value *lessThan = builder.CreateICmpULE(callArc4Random, &inArg1, "__rep_lessthan_");\
		Value *branchRep = builder.CreateCondBr(lessThan, blkRepIt, blkReturn); 			\
																							\
		/*																					\
		 * ...																				\
		 * replace_it:                                       ; preds = %entry				\
		 *  %__rep_cast_ = zext i32 %__rep_rand_ to i64										\
		 *  ret i64 %__rep_cast_															\
		 */																					\
		builder.SetInsertPoint(blkRepIt); 													\
		Value *castRand = builder.CreateZExtOrTrunc(callArc4Random,							\
		    Type::getInt##nBits##Ty(ctx), "__rep_cast_");  									\
		Value *cr = builder.CreateRet(castRand);  											\
																							\
		/*																					\
		 * ...																				\
		 * return:                                           ; preds = %entry				\
		 *  ret i64 %intToFlip																\
		 */																					\
		builder.SetInsertPoint(blkReturn);  												\
		Value *cr2 = builder.CreateRet(&inArg0);  											\
	} 																						\


injectRIntN(64)
injectRIntN(32)
injectRIntN(16)
injectRIntN(8)

void
ReplaceRandomizer::inject(Module& M)
{
	LLVMContext& ctx = M.getContext();

	/*
	 * If arc4random is already in the module, then return the Function for it; otherwise, 
 	 * declare it so it will be pulled in. It should be within scope, so we assert.
 	 */	
	Constant *lookupRand = M.getOrInsertFunction("arc4random", Type::getInt32Ty(ctx), NULL);
	assert(lookupRand != NULL && "ReplaceRandomizer::inject(): failed getFunction(arc4random)\n");

	Function *fnRand = cast<Function>(lookupRand);

	injectInt64(M, fnRand);
	injectInt32(M, fnRand);
	injectInt16(M, fnRand);
	injectInt8(M, fnRand);

}


