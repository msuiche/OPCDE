/*
 * Module pass that makes use of an InstVisitor but 
 * does nothing but dump values.
 */

#include "llvm/IR/Module.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/CommandLine.h"

using namespace llvm;

cl::opt<std::string> FunctionToVisit("fn-to-visit", cl::init(""));

struct VisitorSkelModulePass : public ModulePass {
	static char ID;

	VisitorSkelModulePass() : ModulePass(ID) {}

  /*
   * The point of this pass is to demonstrate how easily we can find a specific instruction type, rather
   * than looping through each instruction...  
   */
	virtual bool
	runOnModule(Module &M)
	{
		if (FunctionToVisit != "") {
			Function *f = M.getFunction(FunctionToVisit);
			if (f == NULL || f->isDeclaration()) {
				errs() << "Unable to find the function you specified.\n";
				return false;
			}
			errs() << "Visiting function: " << f->getName() << "\n";
			SkelInstVisitor v;
			v.visit(*f);
			return false;
		}
		for (auto &f : M) {
			SkelInstVisitor v;
			if (f.isDeclaration()) {
				errs() << "Skipping function declaration\n";
				continue;
			}
			if (f.hasName()) {
				errs() << "\nVisiting function: " << f.getName() << "\n";
			} else {
				errs() << "\nVisiting unnamed function:\n";
			}
			// You can call on a module, a function, or a basicblock....
			// depends on what you need.
			v.visit(f);
			errs() << "\n";
		}
		return false;
	}

	/* See llvm/IR/InstVisitor.h for full set... */
	struct SkelInstVisitor : public InstVisitor<SkelInstVisitor> {
		void
		visitReturnInst(ReturnInst &I) {
			errs() << "X return instruction\n   ";
			I.dump();
		}
		void visitBranchInst(BranchInst &I) {
			errs() << "X branch instruction\n   ";
			I.dump();
		}
		void visitSwitchInst(SwitchInst &I) {
			errs() << "X switch instruction\n   ";
			I.dump();
		}
		void visitIndirectBrInst(IndirectBrInst &I)    {
			errs() << "X indirect branch instruction\n   ";
			I.dump();
		}
		void visitCallInst(CallInst &I) {
			errs() << "X call instruction\n   ";
			I.dump();
		}
		void visitInvokeInst(InvokeInst &I) {
			errs() << "X invoke instruction\n   ";
			I.dump();
		}
		void visitCallSite(CallSite &I) {
			errs() << "X call site instruction\n   ";
		}

		void visitBinaryOperator(BinaryOperator &I) {
			errs() << "X binary instruction\n   ";
			I.dump();
		}
		void visitCmpInst(CmpInst &I) {
			errs() << "X compare instruction\n   ";
			I.dump();
		}
		void visitBitCastInst(BitCastInst &I)          {
			errs() << "X bit cast instruction\n   ";
			I.dump();
		}
		void visitICmpInst(ICmpInst &I)     {
			errs() << "X int compare instruction\n   ";
			I.dump();
		}
		void visitAllocaInst(AllocaInst &I) {
			errs() << "X alloca instruction\n   ";
			I.dump();
		}
		void visitLoadInst(LoadInst     &I) {
			errs() << "X load instruction\n   ";
			I.dump();
		}
		void visitStoreInst(StoreInst   &I) {
			errs() << "X store instruction\n   ";
			I.dump();
		}
		void visitGetElementPtrInst(GetElementPtrInst &I){
			errs() << "X get element pointer instruction\n   ";
			I.dump();
		}
		void visitPHINode(PHINode       &I) {
			errs() << "X PHI node instruction\n   ";
			I.dump();
		}
		void visitTruncInst(TruncInst &I)   {
			errs() << "X truncate instruction\n   ";
			I.dump();
		}
		void visitZExtInst(ZExtInst &I)     {
			errs() << "X zero extend instruction\n   ";
			I.dump();
		}
		void visitSExtInst(SExtInst &I)     {
			errs() << "X signed exstend instruction\n   ";
			I.dump();
		}
		void visitUnaryInstruction(UnaryInstruction &I) {
			errs() << "X unary instruction\n   ";
			I.dump();
		}
	};
};

char VisitorSkelModulePass::ID = 0;
static RegisterPass<VisitorSkelModulePass> XX("vskel",
  "Skeleton code for a module pass that visits function instructions");
