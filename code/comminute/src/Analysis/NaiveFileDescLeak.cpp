/*
 * NaiveFileDescLeak
 * 
 * Look for cases where
 *
 * %k = call srcfnthatreturnsfd
 * ... 
 *
 * and no call close %k in the same function
 */
 
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/DebugLoc.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/CallSite.h"
#include "llvm/Support/raw_ostream.h"

#include <string>

using namespace llvm;

#include "StoreCollector.h"
#include "TargetCallSitesPass.h"
#include "NaiveFileDescLeak.h"

void
NaiveFileDescLeak::getAnalysisUsage(AnalysisUsage &AU) const
{
	AU.addRequired<TargetCallSitesPass>();
	AU.setPreservesCFG();
}

void
NaiveFileDescLeak::printLeak(TargetCallSite *s)
{
	Instruction *i = s->getInstruction();
	DILocation *loc = i->getDebugLoc().get();

	errs() << "  ! file descriptor leak:\n";
	if (loc) {	
		errs() << "      " << s->getCaller()->getName() << " calls "
		  << s->getCalled()->getName() << " at " << loc->getFilename()
		  << ":" << loc->getLine() << " and never closes\n";
	} else {
		// No debug info.
		errs() << "      " << s->getCaller()->getName() << " calls "
		  << s->getCalled()->getName() << " and never closes       \n";
		i->getDebugLoc().dump();
	}
}

void
NaiveFileDescLeak::printLikelyFP(TargetCallSite *s, std::string reason)
{
	Instruction *i = s->getInstruction();
	DILocation *loc = i->getDebugLoc().get();

	errs() << "  ! file descriptor FP likely:\n";
	if (loc) {	
		errs() << "      " << s->getCaller()->getName() << " calls "
		  << s->getCalled()->getName() << " at " << loc->getFilename()
		  << ":" << loc->getLine() << " reason: " << reason << "\n";
	} else {
		// No debug info.
		errs() << "      " << s->getCaller()->getName() << " calls "
		  << s->getCalled()->getName() << " reason: " << reason << "\n";
		i->getDebugLoc().dump();
	}
}

bool
NaiveFileDescLeak::runOnModule(Module &M)
{
	errs() << "Running naive file descriptor leak pass\n";

	/*
	 * Make use of the result of running the TargetCallSitesPass.
	 * It gives locations where file descriptors were created
	 * and locations of file descriptor destroyer calls (e.g. close(2)).
	 */
	TargetCallSitesPass &p = getAnalysis<TargetCallSitesPass>();
	if (p.src_empty()) {
		return false;
	}

	/*
	 * If there exist no close-like calls, then every source is
	 * leaking or we are missing some function that closes (e.g.
 	 * it exists in a different compilation unit (Module) or we
	 * do not know some call from an API we externalized does a
	 * close).
	 */
	if (p.snk_empty()) {
		errs() << "  ! No close-like calls found.\n";
		for (auto tcs = p.src_begin(); tcs != p.src_end(); ++tcs) {
			TargetCallSite *s = tcs->get();
			printLeak(s);
		}
		return false;
	}
	/*
	 * For every open()-like, if there is not even a use of the
	 * Value, then we know there is something up.
	 */
	for (auto srcIt = p.src_begin(); srcIt != p.src_end(); ) {
		TargetCallSite *srcSite = &*srcIt->get();
		Value *possiblyLeaked = srcSite->getTarget();
		if (possiblyLeaked->user_empty()) {
			printLeak(srcSite);
			srcIt = p.src_erase(srcIt);
		} else {
			++srcIt;
		}
	}

	for (auto snkIt = p.snk_begin(); snkIt != p.snk_end(); ++snkIt) {
		TargetCallSite *snkSite = &*snkIt->get();
		Value *closingVar = snkSite->getTarget();
		if (isa<Argument>(closingVar)) {
			printLikelyFP(snkSite, "Value is an Argument to parent function (unsupported)");
		} else if (isa<GlobalVariable>(closingVar)) {
			printLikelyFP(snkSite, "Value is a GlobalVariable (unsupported)");
		}
	}

	/*
	 * Now, for every close()-like, go through and see if we can easily
	 * find a source open(). If we can, we remove the value from being
	 * a possible leak.
	 *
	 * Note that this is naive. Note that if you use the PHINode axe that
	 * you may have FN. Etc etc etc :P
	 */
	for (auto snkIt = p.snk_begin(); snkIt != p.snk_end(); ) {
		TargetCallSite *snkSite = &*snkIt->get();
		Value *closedValue = snkSite->getTarget();
		bool remd = false;
		for (auto srcIt = p.src_begin(); srcIt != p.src_end(); ) {
			TargetCallSite *srcSite = &*srcIt->get();
			Value *possiblyLeaked = srcSite->getTarget();
			if (closedValue == possiblyLeaked) {
				snkIt = p.snk_erase(snkIt);
				srcIt = p.src_erase(srcIt);
				remd = true;
				break;
			} else {
				++srcIt;
			}
		}
		if (remd == false) {
			++snkIt;
		}
	}

	/*
	 * If we did not remove some sources then either:
	 *   (a) we failed to track things properly (very likely! :D)
	 *   (b) we have some fd leak
	 * these are basic, weak assumptions.
	 */
	for (auto srcIt = p.src_begin(); srcIt != p.src_end(); ++srcIt) {
		TargetCallSite *srcSite = &*srcIt->get();
		Value *v = srcSite->getTarget();
		if (isa<Argument>(v)) {
			printLikelyFP(srcSite, "Value is an Argument to parent function (unsupported)");
		} else if (isa<GlobalVariable>(v)) {
			printLikelyFP(srcSite, "Value is a GlobalVariable (unsupported)");
		} else {
			printLeak(srcSite);
		}
	}
	return false;
}

char NaiveFileDescLeak::ID = 0;
static RegisterPass<NaiveFileDescLeak> XX("naive-fd-leak", "Naive fd leak");
