/*
 * Comminute
 *
 * This is a very basic example of using a (legacy) pass
 * manager. We configure passes to run based on command
 * line program options given. Some passes have dependencies,
 * some do not. It is meant to help show how you can
 * not have to do passes via opt, but via a programmatic
 * means.
 *
 */

#include "llvm/LinkAllPasses.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Analysis/MemoryDependenceAnalysis.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/Bitcode/BitcodeWriterPass.h"
#include "llvm-c/Core.h"
#include "llvm/Analysis/CallGraph.h"

#include <iostream>
#include <fstream>

using namespace llvm;

#include "Analysis/TargetCallSitesPass.h"
#include "Transform/FunctionExternalizer.h"
#include "Transform/ChoosePhiValue.h"
#include "Analysis/NaiveConstantArgCheck.h"
#include "Analysis/NaiveSensitiveDataLeak.h"
#include "Analysis/NaiveFileDescLeak.h"
#include "Analysis/PotentiallyDangerousScan.h"
#include "Analysis/PotentiallyDangerousScanUserMethod.h"
#include "Analysis/PotentiallyDangerousScanFunctionPass.h"

/*
 * Command line arguments... 
 *
 *   comminute <command> <input.bc> <output.bc>
 *
 *   commands:
 *     -choose-phi-value <int> :: choose incoming edge to phi node 
 *     -naive-sensitive-data-leak :: very basic sensitive data leak check
 *     -naive-fd-leak :: very basic file descriptor leak check 
 *     -naive-constant-arg :: check some functions for constant arg usage
 *     -dangerous-function :: looks for calls a ``weak'' API
 *     -dangerous-function-user-method :: " but using User list
 *     -dangerous-function-function-pass :: " but as a Function pass
 *
 */
cl::opt<std::string> InputBitcodeFile(cl::Positional, cl::desc("<input.bc>"),
  cl::Required);
cl::opt<std::string> OutputBitcodeFile(cl::Positional, cl::desc("<output.bc>"),
  cl::Required);
cl::opt<int> ChoosePhiValuePass("choose-phi-value",
  cl::desc("Choose value to use from PhiNode (defaults to first)"),
  cl::init(-1));
cl::opt<bool> NaiveSDL("naive-sensitive-data-leak",
  cl::desc("Perform Naive Sensitive Data Leak Analysis"), cl::init(false));
cl::opt<bool> NaiveCAC("naive-constant-arg",
  cl::desc("Perform Naive Constant Argument Check"), cl::init(false));
cl::opt<bool> NaiveFDL("naive-fd-leak",
  cl::desc("Perform Naive File Desc Leak check"), cl::init(false));
cl::opt<bool> PotentiallyDangerous("dangerous-function",
  cl::desc("Silly CWE 676"), cl::init(false));
cl::opt<bool> PotentiallyDangerousUserMethod("dangerous-function-user-method",
  cl::desc("Silly CWE 676 using User list"), cl::init(false));
cl::opt<bool> PotentiallyDangerousFunctionPass("dangerous-function-fpass",
  cl::desc("Silly CWE 676 using function pass"), cl::init(false)); 

int
main(int argc, char **argv)
{
	std::error_code ec;

	legacy::PassManager	passManager;
	std::unique_ptr<Module> irModule;
	ModulePass *modPass;
	SMDiagnostic err;
	raw_fd_ostream *outputStream;

	cl::ParseCommandLineOptions(argc, argv);

	std::cout << "<C> Reading input bitcode file: " << InputBitcodeFile << "\n";
	irModule = parseIRFile(InputBitcodeFile, err,
	  *unwrap(LLVMGetGlobalContext()));
	if (irModule == nullptr) {
		std::cout << "<C> IR issue: " << err.getMessage().str() << "\n";
		return -1;
	}

	/*
	 * Add the pass that removes the body of some functions we do not 
	 * wish to have as local to this module.
	 */
	std::cout << "<C> Adding function externalizer pass.\n";
	FunctionExternalizer *fe = new FunctionExternalizer();
	fe->setFunctionListFilePath("conf/fexternalizer.txt");
	passManager.add(fe);

	/*
	 * The mem2reg pass promotes alloc+load+store to register based. 
	 * This helps with analysis by reducing load/store chasing.
	 */
	std::cout << "<C> Adding mem2reg pass.\n";
	passManager.add(createPromoteMemoryToRegisterPass());

	std::cout << "<C> Adding constant propagation passes.\n";
	passManager.add(createConstantPropagationPass());
	passManager.add(createIPConstantPropagationPass());

	/*
	 * The issue here is that one selection is made. To be better,
	 * you would want to reason, as best you can, about the paths
	 * selected. The same goes for branch analysis, etc. This is like
	 * taking an axe to things :-P.
	 */
	if (ChoosePhiValuePass >= 0) {
		std::cout << "<C> Adding phi value selector pass\n";
		std::cout << "<C> Using edge index: " << ChoosePhiValuePass << "\n";
		ChoosePhiValue *c = new ChoosePhiValue();
		c->setEdgeIndex(ChoosePhiValuePass);
		passManager.add(c);
	}

	if (NaiveSDL) {
	/*
	 * Add the naive sensitive data leak checking pass.
	 *
	 */
		std::cout << "<C> Adding naive sensitive data leak pass.\n";
		TargetCallSitesPass *pt = new TargetCallSitesPass();
		pt->setConfig(TargetCallSitesPass::SourceCall,
		  "conf/sensitivesource.cfg");
		pt->setConfig(TargetCallSitesPass::SinkCall,
		  "conf/sensitivesink.cfg");
		passManager.add(pt);
		NaiveSensitiveDataLeak *n = new NaiveSensitiveDataLeak();
		passManager.add(n);
	} else if (NaiveFDL) {
	/*
	 * Add the fd leak check.
	 */
		std::cout << "<C> Adding naive file descriptor leak pass.\n";
		TargetCallSitesPass *pt = new TargetCallSitesPass();
		pt->setConfig(TargetCallSitesPass::SourceCall,
		  "conf/fdsource.cfg");
		pt->setConfig(TargetCallSitesPass::SinkCall,
		  "conf/fdsink.cfg");
		passManager.add(pt);
		NaiveFileDescLeak *n = new NaiveFileDescLeak();
		passManager.add(n);
	} else if (NaiveCAC) {
	/*
	 * Add the naive constant argument checker pass. This also adds
	 * the ConstantPropagation pass which attempts to lower cases like
	 * int i = 0; foo(i); to foo(0);. This makes it easier for us to 
	 * do the constant checking without having to follow-back.
	 *
	 */
		std::cout << "<C> Adding naive constant argument pass.\n";
		NaiveConstantArgCheck *nca = new NaiveConstantArgCheck();
		nca->setConfigFilePath("conf/constantarg.cfg");
		passManager.add(nca);
	} else if (PotentiallyDangerous) {
		std::cout << "<C> Adding call graph pass\n";
		passManager.add(new CallGraphWrapperPass());
		std::cout << "<C> Adding dangerous fn scan pass.\n";
		PotentiallyDangerousScan *p = new PotentiallyDangerousScan();
		p->setConfigFilePath("conf/pdfunctions.cfg");
		passManager.add(p);
	} else if (PotentiallyDangerousUserMethod) {
		std::cout << "<C> Adding dangerous fn scan user method pass.\n";
		PotentiallyDangerousScanUserMethod *p = new PotentiallyDangerousScanUserMethod();
		p->setConfigFilePath("conf/pdfunctions.cfg");
		passManager.add(p);
	} else if (PotentiallyDangerousFunctionPass) {
		std::cout << "<C> Adding dangerous fn scan function pass.\n";
		PotentiallyDangerousScanFunctionPass *p = new PotentiallyDangerousScanFunctionPass();
		p->setConfigFilePath("conf/pdfunctions.cfg");
		passManager.add(p);
	}

	/*
	 * Open output stream and use that as conduit for writing output
	 * bitcode file.
	 *
	 */
	std::cout << "<C> Adding bitcode writer pass\n";
	outputStream = new raw_fd_ostream(OutputBitcodeFile, ec, sys::fs::F_None);
	modPass = createBitcodeWriterPass(*outputStream, false, true);
	passManager.add(modPass);

	/*
	 * Actually run the passes added on this module. With this very
	 * basic tool, there is just results to std[out|err].
	 *
	 */
	std::cout << "<C> Running passes\n";
	passManager.run(*irModule.get());
	outputStream->close();
	std::cout << "<C> Finished...\n";
	return 0;
}
