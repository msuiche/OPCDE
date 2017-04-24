LLVM_VER=3.9
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


PASS=BBSkel.so
PASS_OBJECTS=BBSkel.o

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
	$(QUIET)rm -rf built test/*.bc


tests:
	$(CC) -emit-llvm -o test/foo.bc -c test/foo.c

runtests:
	$(OPT) -load built/BBSkel.so -mem2reg -bbskel < test/foo.bc
