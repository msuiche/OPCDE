/*
 * Often in Android a thing to do is to
 * grab source to a project and just build it
 * in with your native code. This is painful from
 * the analysis perspective because you (we) do
 * not want to scan non-customer code. So, what
 * this attempts to do is to look for any functions
 * that are libc, libssl, DESLib, etc and convert
 * them from their lifted-wrapper versions to 
 * just be the external declaration.
 *
 * To do this, you just deleteBody() on the function.
 *
 */
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/CallSite.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/SymbolTableListTraits.h"
#include "llvm/Support/raw_ostream.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace llvm;

#include "FunctionExternalizer.h"

bool
FunctionExternalizer::runOnModule(Module &M)
{
	errs() << "Running function externalizer pass.\n";


	std::ifstream fileHandle(this->_functionListFile);
	std::string fnName;
	bool rv = false;

	/*
	 * Each line is a function name to extern. It does not do any
	 * checking of the target function's signature etc, so be aware. 
	 */
	while (std::getline(fileHandle, fnName)) {
		// skip comment line.
		if (fnName.find("#", 0) == 0) {
			continue;
		}
		// Does the function exist within this module? 
		Function *f = M.getFunction(fnName);
		if (f == NULL) {
			continue;
		}
		// Definition is already outside of this module.
		if (f->isDeclaration()) {
			continue;
		}
		// Remove the body (definition) of the function. Leave declaration.
		errs() << "Deleting body of function: " << f->getName().str() << "\n";
		f->deleteBody();
		rv = true;
	}
	return rv;
}
char FunctionExternalizer::ID = 0;
static RegisterPass<FunctionExternalizer> XX("fn-extern",
  "Function externalizer");
