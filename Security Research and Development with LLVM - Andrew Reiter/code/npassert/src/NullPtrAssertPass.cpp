#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/CommandLine.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

using namespace llvm;

#include "NullPtrAssertPass.h"

/*
 * cl::opt represent options taken from the command line, they can include defaults 
 * help descriptions, etc.  This is how a pass can be made more granular or specifify
 * a type of analysis for instance.
 */
cl::opt<std::string> ReplaceConfigFileName("npa-target-config",
  cl::desc("configuration file for np asserts"), cl::init(""));

cl::opt<bool> AssertFunction("npa-use-function", 
  cl::desc("if set, then create new assertion function"), cl::init(false));

void
NullPtrAssertPass::insertAssertionFunction(Module *M)
{
	LLVMContext &ctx = M->getContext();

	/*
	 * Insert our own assertion function. void __NPA_assert_8__(i8 *, i32)
	 */
	Constant *assertFnCon = M->getOrInsertFunction("__NPA_assert_8__",
	  Type::getVoidTy(ctx),
	  Type::getInt8PtrTy(ctx), Type::getInt32Ty(ctx), NULL);
	assertFn = cast<Function>(assertFnCon);
	assertFn->setCallingConv(CallingConv::C);
	Argument *arg = &assertFn->getArgumentList().front();
	arg->setName("ptrToCheck");	

	/*
	 * Insert blocks for entry and branches
	 */
	BasicBlock *blkEntry = BasicBlock::Create(ctx, "npa_entry_blk", assertFn);
	BasicBlock *blkAssert = BasicBlock::Create(ctx, "npa_assert_blk", assertFn);
	BasicBlock *blkReturn = BasicBlock::Create(ctx, "npa_return_blk", assertFn);

	/*
	 * Creates a null pointer and compares it with the argument.
	 * If equal, branch to blkAssert, if not, return block
	 */
    IRBuilder<> builder8(blkEntry);
    PointerType *ptNull = cast<PointerType>(Type::getInt8PtrTy(ctx));
    Value *cpNull0 = ConstantPointerNull::get(ptNull);
    Value *eqNull = builder8.CreateICmpEQ(arg, cpNull0);
    (void)builder8.CreateCondBr(eqNull, blkAssert, blkReturn);

	/*
	 * attempt to deref null
	 */
    builder8.SetInsertPoint(blkAssert);
    Value *crLoad = builder8.CreateAlloca(IntegerType::get(ctx, 32));
    auto ptNullB = PointerType::get(IntegerType::get(ctx, 32), 0);
    Value *cpNull = ConstantPointerNull::get(ptNullB);
    Value *doLoad = builder8.CreateLoad(cpNull);
    Value *doStore = builder8.CreateStore(doLoad, crLoad, true);
	(void)builder8.CreateRetVoid();

	/* return block */
    builder8.SetInsertPoint(blkReturn);
	(void)builder8.CreateRetVoid();

}

void
NullPtrAssertPass::insertAssertionFunctionCall(Module *M, Function *F,
  Argument *A)
{
	LLVMContext &ctx = M->getContext();
	BasicBlock *oe = &F->front();
	Instruction *oi = &oe->front();	
	APInt a(32, A->getArgNo(), false);
	ConstantInt *cv = ConstantInt::get(ctx, a);
	Value *v = new BitCastInst(A, Type::getInt8PtrTy(ctx), "bitcastme_", oi);
	CallInst::Create(assertFn, {v, cv}, "", oi);
	return;
}

void
NullPtrAssertPass::insertAssertion(Module *M, Function *F, Argument *A)
{
	LLVMContext &ctx = M->getContext();

	/*
	 * Get original first BasicBlock in the function.
	 */
	BasicBlock *origEntry = &F->front();

	/*
	 * Add a new BasicBlock before this original first one.
	 * This block will hold code that contains the compare and
	 * conditional branch statements.
	 */
	BasicBlock *entryBlock = BasicBlock::Create(ctx, "npa_entry_blk", F,
	  origEntry);
	/*
	 * Add block between the new entry block and the original entry that
	 * will hold code for causing crash.
	 */
	BasicBlock *assertBlock = BasicBlock::Create(ctx, "npa_assert_blk", F,
	  origEntry);

	/*
	 * Using the IRBuilder API to add code
	 * Get a NULL pointer and see if it and the argument A
	 * are equal.
	 * Branch to the assert block if equal, otherwise, go to
	 * the original entry block and continue.
	 */
	IRBuilder<> builder(entryBlock);
	Type *typeNull = A->getType();
	assert(isa<PointerType>(typeNull) && "typeNull not PointerType");
	PointerType *ptNull = cast<PointerType>(typeNull);
	Value *cpNull0 = ConstantPointerNull::get(ptNull);
	Value *eqNull = builder.CreateICmpEQ(A, cpNull0);
	(void)builder.CreateCondBr(eqNull, assertBlock, origEntry);

	/*
	 * Add the crash code to the assertBlock.
	 * The basic idea is trying to deref null ptr. We could do this
	 * to re-use some NULLs we might have, but to show more of the API
	 * we do it in the following manner.
	 */
	builder.SetInsertPoint(assertBlock);
	Value *crLoad = builder.CreateAlloca(IntegerType::get(ctx, 32));
	auto ptNullB = PointerType::get(IntegerType::get(ctx, 32), 0);
	Value *cpNull = ConstantPointerNull::get(ptNullB);
	Value *doLoad = builder.CreateLoad(cpNull);
	Value *doStore = builder.CreateStore(doLoad, crLoad, true);
	// Add a terminator for this block so the IR is sane
	builder.CreateBr(origEntry);
}

bool
NullPtrAssertPass::attemptInsertAssert(Module *M, Function *f, int targetIdx)
{
	bool chg = false;
	int argIdx = -1;

	/* Decl not defn */
	if (f->isDeclaration() == true) {
		return false;
	}

	/* No body, nothing to inject */
	if (f->empty() == true) {
		return false;
	}

	/* Deal with just named functions. */
	if (f->hasName() == false) {
		return false;
	}

	/* No arguments, nothing to possibly assert. */
	if (f->arg_empty() == true) {
		return false;
	}
	/* We have a specified argument index */
	if (targetIdx >= 0 && f->arg_size() <= (unsigned)targetIdx) {
		return false;
	}
	for (auto ait = f->arg_begin(); ait != f->arg_end(); ++ait) {
		++argIdx;
		if (targetIdx != -1 && argIdx != targetIdx) {
			continue;
		}
		Argument *a = &*ait;
		Type *ty = a->getType();

		// The type of this argument is not of pointer; nothing to do.
		if (ty->isPointerTy() == false) {
			continue;
		}
		if (AssertFunction) {
			insertAssertionFunctionCall(M, f, a);
		} else {
			insertAssertion(M, f, a);
		}
		chg = true;
	}
	return chg;
}

bool
NullPtrAssertPass::runOnModule(Module &M)
{
	bool chg = false;

	if (AssertFunction) {
		insertAssertionFunction(&M);
	} 
	/* Do'em all */
	if (ReplaceConfigFileName == "") {
		for (auto fit = M.functions().begin();
		  fit != M.functions().end(); ++fit) {
			Function *f = &*fit;
			if (f->hasName()) {
				if (f->getName().str().find("__NPA_",0) == 0) {
					continue;
				} 
			}
			chg = attemptInsertAssert(&M, f, -1);
		}
	} else {
		std::ifstream hand(ReplaceConfigFileName);
		std::string line;
		std::string fnName;
		std::string argIdx;
		int idx;	
		// We trust that the user is giving us a sane file.
		while (std::getline(hand, line)) {
			if (line.find("#", 0) == 0) {
				continue;
			}
			size_t i = line.find(",", 0);
			fnName = line.substr(0, i);
			argIdx = line.substr(i+1); // XXX ;^p
			idx = stoi(argIdx, nullptr, 10);	
			if (idx < 0) {
				idx = -1;
			}
			Function *f = M.getFunction(fnName);
			if (f == NULL) {
				errs() << "Unable to find function: " << fnName << "\n";
				continue;
			}
			chg = attemptInsertAssert(&M, f, idx);
		}
	}
	return chg;
}

char NullPtrAssertPass::ID = 0;
static RegisterPass<NullPtrAssertPass> XX("null-ptr-assert", "Inject null ptr checks");

