#
# This is all a bit hack-ish, but should give the idea
# about llvm-config usage, which is the key tool to make
# life easy^Hier.
#
# make jsoncpp (once)
# make
#
# I assume you have things in /usr/bin. Often this is the
# case, but providing a path for you to set.
#
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
LDFLAGS+=-Lthirdparty/jsoncpp-1.8.0/build/src/lib_json -ljsoncpp
LDFLAGS+=-shared -Wl,-O1

CXXFLAGS+=-I$(shell $(LLVM_CONFIG) --includedir)
CXXFLAGS+=-std=c++11 -fPIC -fvisibility-inlines-hidden
CXXFLAGS+=-Wall -Wextra -g -Wno-unused-parameter -Wno-unused-variable

CPPFLAGS+=$(shell $(LLVM_CONFIG) --cppflags)
CPPFLAGS+=-I$(SRC_DIR) -Ithirdparty/jsoncpp-1.8.0/include


PASS=libIntFlip.so
PASS_OBJECTS=FlipConfig.o \
 LiftConstantIntPass.o  \
 ReplaceRandomizer.o \
 BitFlipRandomizer.o \
 InjectRandomizers.o \
 IntReplacerVisitor.o \
 IntReplacerIterate.o


# IntReplacerVisitor.o


default: prep $(PASS)

# Quite the hack :-P
jsoncpp:
	@echo Building jsoncpp-1.8.0
	cd thirdparty &&  \
	  tar zxvf jsoncpp-1.8.0.tar.gz &&  \
	  cd jsoncpp-1.8.0 &&  \
	  rm -rf build &&  \
	  mkdir -p build &&  \
	  cd build &&  \
	  cmake .. &&  \
	  make &&  \
	  cd ../../


prep:
	$(QUIET)mkdir -p built
	
%.o : $(SRC_DIR)/%.cpp
	@echo "CPPFLAGS: ${CPPFLAGS}"
	@echo "CXXFLAGS: ${CXXFLAGS}"
	@echo Compiling $*.cpp
	$(QUIET)$(CXX) -o built/$*.o -c $(CPPFLAGS) $(CXXFLAGS) $<

$(PASS) : $(PASS_OBJECTS)
	@echo Linking $@
	$(QUIET)$(CXX) -o built/$@ $(LDFLAGS) $(CXXFLAGS)  built/*.o

clean:
	$(QUIET)rm -rf built

jsonclean:
	$(QUIET)rm -rf thirdparty/jsoncpp-1.8.0

tests:
	$(QUIET)echo "Generating bitcode from C"
	$(QUIET)$(CC) -emit-llvm -c -o test/foo.bc test/foo.c
	$(QUIET)echo "Lifting constants to local variables"
	$(QUIET)$(OPT) -load built/libIntFlip.so -lift-constant-int-args -o test/foo2.bc < test/foo.bc
	$(QUIET)echo "Injecting randomizer functions"
	$(QUIET)$(OPT) -load built/libIntFlip.so -inject-randomizers  -o test/foo3.bc < test/foo2.bc
	$(QUIET)echo "Replacing ints with visitor method based on replace.cfg"
	$(QUIET)$(OPT) -load built/libIntFlip.so -replace-ints-visitor -o test/foo4.bc -repcfg=replace.cfg < test/foo3.bc
	$(QUIET)echo "Replacing ints with iteration method based on replace.cfg"
	$(QUIET)$(OPT) -load built/libIntFlip.so -replace-ints-iterate -o test/foo5.bc -repcfg=replace.cfg < test/foo3.bc
	$(QUIET)echo "llvm-dissing..."
	$(QUIET)$(DIS) --o=test/foo.ll test/foo.bc
	$(QUIET)$(DIS) --o=test/foo2.ll test/foo2.bc
	$(QUIET)$(DIS) --o=test/foo3.ll test/foo3.bc
	$(QUIET)$(DIS) --o=test/foo4.ll test/foo4.bc
	$(QUIET)$(DIS) --o=test/foo5.ll test/foo5.bc
	$(QUIET)echo "Building foo, foo4, foo5 executables"
	$(QUIET)$(CC) -o test/foo5 test/foo5.bc -pthread -lbsd
	$(QUIET)$(CC) -o test/foo4 test/foo4.bc -pthread -lbsd
	$(QUIET)$(CC) -o test/foo test/foo.bc -pthread -lbsd
	$(QUIET)echo ""
	$(QUIET)echo "Generating bitcode from C++ source"
	$(QUIET)$(CXX) -emit-llvm -c -o test/foopp.bc test/foo.cpp
	$(QUIET)echo "Lifting constants to local variables"
	$(QUIET)$(OPT) -load built/libIntFlip.so -lift-constant-int-args -o test/foopp2.bc < test/foopp.bc
	$(QUIET)echo "Injecting randomizer functions"
	$(QUIET)$(OPT) -load built/libIntFlip.so -inject-randomizers  -o test/foopp3.bc < test/foopp2.bc
	$(QUIET)echo "Replacing ints with visitor method based on replacepp.cfg"
	$(QUIET)$(OPT) -load built/libIntFlip.so -replace-ints-visitor -o test/foopp4.bc -repcfg=replacepp.cfg < test/foopp3.bc
	$(QUIET)echo "Replacing ints with iteration method based on replacepp.cfg"
	$(QUIET)$(OPT) -load built/libIntFlip.so -replace-ints-iterate -o test/foopp5.bc -repcfg=replacepp.cfg < test/foopp3.bc
	$(QUIET)echo "llvm-dissing..."
	$(QUIET)$(DIS) --o=test/foopp.ll test/foopp.bc
	$(QUIET)$(DIS) --o=test/foopp2.ll test/foopp2.bc
	$(QUIET)$(DIS) --o=test/foopp3.ll test/foopp3.bc
	$(QUIET)$(DIS) --o=test/foopp4.ll test/foopp4.bc
	$(QUIET)$(DIS) --o=test/foopp5.ll test/foopp5.bc
	$(QUIET)echo "Building foopp, foopp4, foopp5 executables"
	$(QUIET)$(CXX) -o test/foopp5 test/foopp5.bc -pthread -lbsd
	$(QUIET)$(CXX) -o test/foopp4 test/foopp4.bc -pthread -lbsd
	$(QUIET)$(CXX) -o test/foopp test/foopp.bc -pthread -lbsd

cleantests:
	rm -f test/*.bc test/*.ll foo foo4 foo5 foopp foopp4 foopp5

cleanall: clean jsonclean cleantests
