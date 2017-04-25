# Security Related LLVM Projects

Some entries are on the cusp of security and non-security related. Please
submit a pull request (or a friendly email) to add or remove projects. I
highly doubt it is a complete list, so always welcoming additions and/or
corrections. Further, I had thought to break out papers into it's own 
section, but decided against it for reasons related to sleep. And even
further... There are some things I just wanted in here for informational
reasons; i.e. they may not be projects. :camel:

## Projects

### AddressSanitizer (ASan)
> Memory error detector for C/C++ finding UAF, buffer overflow, 
> UAR, etc

[GitHub](https://github.com/google/sanitizers/wiki/AddressSanitizer)
Note the code in lib/Transform/Instrumentation.

[``AddressSanitizer: A Fast Address Sanity Checker''](https://static.googleusercontent.com/media/research.google.com/en//pubs/archive/37752.pdf) 

### American Fuzzy Lop (AFL)
> a security-oriented fuzzer that employs a novel type of compile-time
> instrumentation and genetic algorithms to automatically discover
> clean, interesting test cases that trigger new internal states in the
> targeted binary

http://lcamtuf.coredump.cx/afl

### Andersen's Analysis
[``Program Analysis and Specialization for the C Programming Language''](http://www.cs.cornell.edu/courses/cs711/2005fa/papers/andersen-thesis94.pdf)

[Standalone LLVM impl](https://github.com/grievejia/andersen)

### ASAP (EPFL)
> Analyze runtime checks to balance those that are needed and those that are not

http://dslab.epfl.ch/proj/asap/

### Causal, Adaptive, Distributed, and Efficient Tracing System (CADETS)
> address flaws in current audit and information-flow systems through
> fundamental improvements in dynamic instrumentation, scalable
> distributed tracing, and programming-language support.

http://www.cl.cam.ac.uk/research/security/cadets/

### cclyzer
> A tool for analyzing LLVM bitcode using Datalog.

[GitHub](https://github.com/plast-lab/cclyzer)

### DangSan (VUSec)
> Dangling pointer detection

[GitHub](https://github.com/vusec/dangsan)

[Paper](http://www.cs.vu.nl/~giuffrida/papers/dangsan_eurosys17.pdf)

### DataFlowSanitizer
> Generalized data flow API

http://clang.llvm.org/docs/DataFlowSanitizerDesign.html 

### Divine
> Explicit-state model checker

https://divine.fi.muni.cz

[Thesis](https://is.muni.cz/th/373979/fi_m/thesis.pdf)

### DynamicTools
> This project consists of several useful tool for dealing with LLVM
> IR runtime behaviors. Currently it consists of two parts,
> a custom-written LLVM IR interpreter, and an LLVM IR fuzzer (abandoned).

[GitHub](https://github.com/grievejia/LLVMDynamicTools)

### fdc
> An optimizing decompiler (for reversing)

http://zneak.github.io/fcd/

[GitHub](https://github.com/zneak/fcd) 

### FindFlows
https://llvm.org/svn/llvm-project/giri/trunk/lib/Static/FindFlows.cpp

### Foreign Inference
> Auto-generate wrappers for C code for Python etc.

[GitHub](https://github.com/travitch/foreign-inference) 
Out of date, someone should revive and build LibFuzzer wrappers

### Gist Static Analyzer (EPFL)
> Failure-sketching to help determine reasons for faults

http://dslab.epfl.ch/proj/gist/

[Paper](http://dslab.epfl.ch/pubs/gist.pdf)

### Trail of Bits CGC
[``How we fared in the Cyber Grand Challenge''](https://blog.trailofbits.com/2015/07/15/how-we-fared-in-the-cyber-grand-challenge/)

### Infer (Facebook)
> Compile time static analyzer

http://fbinfer.com/

[GitHub](https://github.com/facebook/infer)

[Infer Clang Plugin](https://github.com/facebook/facebook-clang-plugins/tree/ee26293dd046acc5c2dd862d3201aa9f7dace96a)

### Kryptonite obfuscator
[GitHub](https://github.com/0vercl0k/stuffz/blob/master/llvm-funz/kryptonite/llvm-functionpass-kryptonite-obfuscater.cpp)

### KULFI (Utah)
> Instruction level fault injector

[GitHub](https://github.com/soarlab/KULFI)

http://formalverification.cs.utah.edu/fmr/#kulfi

### IKOS (NASA)
> a C++ library designed to facilitate the development of sound static analyzers based on Abstract Interpretation

https://ti.arc.nasa.gov/opensource/ikos/

### lafindel's transforms
> Splitting certain compares to improve fuzzing

[``Circumventing Fuzzing Roadblocks with Compiler Transformations''](https://lafintel.wordpress.com/2016/08/15/circumventing-fuzzing-roadblocks-with-compiler-transformations/)

### LeakSanitizer
> Memory leak detection

[GitHub](https://github.com/google/sanitizers/wiki/AddressSanitizerLeakSanitizer)

### LibFuzzer
> Evolutionary, in-process guided fuzzing library

http://llvm.org/docs/LibFuzzer.html

[GitHub](https://github.com/llvm-mirror/llvm/tree/master/lib/Fuzzer)

[LibFuzzer Tutorial](https://github.com/google/fuzzer-test-suite/blob/master/tutorial/libFuzzerTutorial.md) 

### llmc
[``Model checking LLVM IR using LTSmin''](http://fmt.cs.utwente.nl/files/sprojects/190.pdf) 

### llStar
> Pre/post condition intra-procedural verifier

https://bitbucket.org/jvillard/llstar/wiki/Home

### Mc Sema (ToB)
> Lift MC to IR for retargeting, patching, recompilation, symbolic exec

[Github](https://github.com/trailofbits/mcsema)

### MemorySanitizer
> Detect pointer misalignment etc

[GitHub](https://github.com/google/sanitizers/wiki/MemorySanitizer) and see source in lib/Transform/Instrumentation

### Obfuscator-LLVM
[GitHub](https://github.com/obfuscator-llvm/obfuscator)

Note: https://github.com/obfuscator-llvm/obfuscator/tree/llvm-3.6.1/lib/Transforms/Obfuscation

### Passes from QuarksLab
[GitHub](https://github.com/quarkslab/llvm-passes/tree/master/llvm-passes)

[``Turning regular code into atrocities''](http://blog.quarkslab.com/turning-regular-code-into-atrocities-with-llvm.html)

### pmGen
> Translate IR to Promela for verification

[GitHub](https://github.com/roselone/pmGen)

### PRESAGE (Utah)
> protecting structured address computations against soft errors

http://formalverification.cs.utah.edu/fmr/#presage

[Git]https://utahfmr.github.io/PRESAGE/

[Paper](https://arxiv.org/abs/1606.08948)
Basically, it attempts to reorganize GetElementPtr's to best handle alpha particle hits.

### Pointer analysis with tunable precision (TPA)
[GitHub](https://github.com/grievejia/tpa)

### Remill (ToB)
> Lift MC instructions to IR

[GitHub](https://github.com/trailofbits/remill)

### Return-less code
> Transform IR to have no return statements.. attempt to avoid ROP

[Paper](http://www4.ncsu.edu/~mcgrace/EUROSYS10.pdf) 

### Rev.ng
> Lift MC to IR by using QEMU

https://rev.ng/

[Rev.ng LLVM developer mtg 2016](http://llvm.org/devmtg/2016-11/Slides/DiFederico-rev.ng.pdf)

### s2e (EPFL)
[GitHub](https://github.com/dslab-epfl/s2e)

### SAFECode
http://safecode.cs.illinois.edu/ 

[``Memory Safety for Low-Level Software/Hardware Interactions''](http://llvm.org/pubs/2009-08-12-UsenixSecurity-SafeSVAOS.html)

### SafeInit (VUSec)
> Detect uninitialized memory use errors

[GitHub](https://github.com/vusec/safeinit)

[Paper](https://www.vusec.net/download/?t=papers/safeinit_ndss17.pdf)

### Seahorn
> Intraprocedural model checker

[GitHub](https://github.com/seahorn/seahorn)

See the links there to CRAB etc

### Security-Oriented Analysis of Application Programs (SOAAP) (Cambridge)
http://www.cl.cam.ac.uk/research/security/ctsrd/soaap

[GitHub](https://github.com/CTSRD-SOAAP)

### Sloopy
http://forsyte.at/people/pani/sloopy/ 

### Smack
[GitHub](https://github.com/smackers/smack)

http://soarlab.org/2014/05/smack-decoupling-source-language-details-from-verifier-implementations/

### Software Analysis Workbench (SAW) (Galois Inc)
> Formal verification via equivelancy checks

http://saw.galois.com/

[GitHub SAW Script](https://github.com/GaloisInc/saw-script) 

[GitHub llvm-verifier](https://github.com/GaloisInc/llvm-verifier)

### Static Value Flow (SVF) (UNSW)
> Pointer Analysis and Program Dependence Analysis for C and C++ Programs

http://unsw-corg.github.io/SVF/

[GitHub](https://github.com/unsw-corg/SVF)


### Temporally Enhanced Security Logic Assertions (TESLA) (Cambridge)
http://www.cl.cam.ac.uk/research/security/ctsrd/tesla

[GitHub](https://github.com/CTSRD-TESLA/)

### TokenCap
> Find tokens/magics in code so as to quickly pass fuzzing blockers

[GitHub](https://github.com/0vercl0k/stuffz/blob/master/llvm-funz/afl-llvm-tokencap-pass.so.cc)

### Typesan (VUSec)
[GitHub](https://github.com/vusec/typesan) 

### Verified LLVM (VeLLVM)
> Model syntax and semantics of LLVM IR in Coq for proving things about code reasoning on IR

http://www.cis.upenn.edu/~stevez/vellvm/

https://deepspec.org/main

### Whole program LLVM
> Help linking multiple .bc files to one

[GitHub](https://github.com/travitch/whole-program-llvm)
