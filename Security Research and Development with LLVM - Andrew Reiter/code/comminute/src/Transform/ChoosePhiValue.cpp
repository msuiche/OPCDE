/*
 * Choose a value from a Phi node and replace all
 * Phi node uses with it. This ignores some paths, but
 * simplifies analysis. Will attempt to drop no longer
 * used basic blocks.
 */
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/CallSite.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/SymbolTableListTraits.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#include "ChoosePhiValue.h"

bool
ChoosePhiValue::runOnModule(Module &M)
{
	errs() << "Running choose phi value pass.\n";
	bool rv = false;

	/*
	 * For every function in this compilation unit, iterate through
	 * the instructions looking for PHINodes. When a PHINode is found,
	 * the chosen incoming edge will be used, if it exists, otherwise
	 * the first (0) is chosen. The edge is removed.
	 *
	 * Take all the removed edges and replace the corresponding PHINode
	 * uses with the edge's value. So, where before things were using
	 * PHINode as the Value, they will use the incoming edge Value.
	 * Remove all other incoming edges and then erase the PHINode.
	 * Then, for each unused incoming edge, attempt to erase it's 
	 * BasicBlock.
 	 * 
	 */
	for (auto &f : M) {
		std::vector<std::pair<PHINode *, Value *>> replaceList;

		for (auto ii = inst_begin(f); ii != inst_end(f); ++ii) {
				Instruction *in = &*ii;
				if (PHINode *pn = dyn_cast<PHINode>(in)) {
					unsigned usedEdge = edgeIndex;
					if (pn->getNumIncomingValues() <= edgeIndex) {
						errs() << "Not enough incoming values...using 0\n";
						usedEdge = 0;
					}
					// Remove chosen incoming value, add to replacement list.
					Value *x = pn->removeIncomingValue(usedEdge, false);
					replaceList.push_back(std::make_pair(pn, x));
					
				}
		}
		// We know we are going to change the CFG.
		if (replaceList.empty() == false) {
			rv = true;
		}
		for (auto pc : replaceList) {
			/* Replace all uses of the PHINode with the selected Value */
			pc.first->replaceAllUsesWith(pc.second);
			while (pc.first->getNumIncomingValues() > 0) {
				Value *d = pc.first->removeIncomingValue((unsigned)0, false);	
				// Each instruction resides in a BasicBlock 
				assert(isa<Instruction>(d) == true);
				Instruction *vi = cast<Instruction>(d);
				BasicBlock *bb = vi->getParent();
				if (bb->user_empty()) {
					bb->eraseFromParent();
					continue;
				}
				// Attempt to remove users of BasicBlock so we can axe it 
				attemptUserReduction(bb);
				if (bb->user_empty()) {
					bb->eraseFromParent();
					continue;
				} 
			}
			assert(pc.first->users_empty());
			pc.first->eraseFromParent();
		}
#ifdef	DEBUG
		for (auto ii = inst_begin(f); ii != inst_end(f); ++ii) {
				Instruction *in = &*ii;
				assert(!isa<PHINode>(in) && "PHINode chooser missed.");
		}
#endif
	}
	return rv;
}

void
ChoosePhiValue::attemptUserReduction(BasicBlock *bb)
{
	/*
	 * Go through each user of this and we should just be finding
	 * BranchInst. If there is just one operand, then this is
	 * an unconditional branch and cannot be mucked with (unless
	 * we did deeper analysis), so we just return because
	 * we know there will always be a user of this BasicBlock.
	 *
	 * Otherwise, we make the true/false branch be the same and 
	 * not our basic block. ... Repeat all this until gone 
	 * through all users...
	 */
	for (auto ui = bb->user_begin(); ui != bb->user_end(); ++ui) {
		User *u = *ui;
		if (Instruction *i = dyn_cast<Instruction>(u)) {
			if (BranchInst *bi = dyn_cast<BranchInst>(i)) {
				if (bi->getNumOperands() == 1) {
					return;
				}
				Value *tb = bi->getOperand(1);
				Value *fb = bi->getOperand(2);
				if (tb == cast<Value>(bb)) {
					bi->setOperand(1, fb);
				} else if (fb == cast<Value>(bb)) {
					bi->setOperand(2, tb);
				}
			}
		}
	}
}

char ChoosePhiValue::ID = 0;
static RegisterPass<ChoosePhiValue> XX("choose-phi-value",
  "Choose Phi value to use");
