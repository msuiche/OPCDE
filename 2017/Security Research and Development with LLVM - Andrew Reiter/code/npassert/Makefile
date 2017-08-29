LLVM_VER=3.8
#LLVM_VER=3.9
#LLVM_VER=4.0
LLVM_HOME=/usr/bin
LLVM_CONFIG?=$(LLVM_HOME)/llvm-config-$(LLVM_VER)

ifndef VERBOSE
QUIET:=@
endif

SRC_DIR?=$(PWD)/src

CXX=$(LLVM_HOME)/clang++-$(LLVM_VER)
CC=$(LLVM_HOME)/clang-$(LLVM_VER)
OPT=$(LLVM_HOME)/opt-$(LLVM_VER)
DIS=$(LLVM_HOME)/llvm-dis-$(LLVM_VER)
LNK=$(LLVM_HOME)/llvm-link-$(LLVM_VER)

LDFLAGS+=$(shell $(LLVM_CONFIG) --ldflags) 
LDFLAGS+=-shared -Wl,-O1

CXXFLAGS+=-I$(shell $(LLVM_CONFIG) --includedir)
CXXFLAGS+=-std=c++11 -fPIC -fvisibility-inlines-hidden
CXXFLAGS+=-Wall -Wextra -g -Wno-unused-parameter -Wno-unused-variable

CPPFLAGS+=$(shell $(LLVM_CONFIG) --cppflags)
CPPFLAGS+=-I$(SRC_DIR)

PASS=libnpassert.so
PASS_OBJECTS=NullPtrAssertPass.o 

default: prep $(PASS)

prep:
	$(QUIET)mkdir -p built
	
%.o : $(SRC_DIR)/%.cpp
	@echo Compiling $*.cpp
	$(QUIET)$(CXX) -o built/$*.o -c $(CPPFLAGS) $(CXXFLAGS) $<

$(PASS) : $(PASS_OBJECTS)
	@echo Linking $@
	$(QUIET)$(CXX) -o built/$@ $(LDFLAGS) $(CXXFLAGS)  built/*.o

clean:
	$(QUIET)rm -rf built

tests:
	$(QUIET)echo "Generating bitcode from C"
	$(QUIET)$(CC) -g -emit-llvm -c -o test/ex01.bc test/ex01.c
	$(QUIET)$(CC) -g -emit-llvm -c -o test/ex02.bc test/ex02.c
	$(QUIET)$(OPT) -load built/libnpassert.so -null-ptr-assert -npa-use-function -o test/ex02c.bc < test/ex02.bc
	$(QUIET)echo "Attempting to inject assertions"
	$(QUIET)$(OPT) -load built/libnpassert.so -null-ptr-assert -o test/ex01a.bc < test/ex01.bc
	$(QUIET)$(OPT) -load built/libnpassert.so -null-ptr-assert -o test/ex02a.bc < test/ex02.bc
	$(QUIET)echo "Running inject with config file"	
	$(QUIET)$(OPT) -load built/libnpassert.so -null-ptr-assert -npa-target-config conf/targ.cfg -o test/ex01b.bc < test/ex01.bc
	$(QUIET)$(OPT) -load built/libnpassert.so -null-ptr-assert -npa-target-config conf/targ.cfg -o test/ex02b.bc < test/ex02.bc
	$(QUIET)echo "Running llvm-dis on the bitcode files"
	$(QUIET)$(DIS) --o=test/ex01a.ll test/ex01a.bc
	$(QUIET)$(DIS) --o=test/ex01b.ll test/ex01b.bc
	$(QUIET)$(DIS) --o=test/ex01.ll test/ex01.bc
	$(QUIET)$(DIS) --o=test/ex02a.ll test/ex02a.bc
	$(QUIET)$(DIS) --o=test/ex02b.ll test/ex02b.bc
	$(QUIET)$(DIS) --o=test/ex02.ll test/ex02.bc
	$(QUIET)echo "Compiling to machine code (elf)"
	$(QUIET)$(CC) -g -o test/ex01a test/ex01a.bc 
	$(QUIET)$(CC) -g -o test/ex01 test/ex01.bc 
	$(QUIET)$(CC) -g -o test/ex02a test/ex02a.bc 
	$(QUIET)$(CC) -g -o test/ex02c test/ex02c.bc 
	$(QUIET)$(CC) -g -o test/ex02 test/ex02.bc 

cleantests:
	rm -f test/*.bc test/*.ll test/ex01 test/ex01a test/ex02 test/ex02a test/ex02c

cleanall: clean cleantests
