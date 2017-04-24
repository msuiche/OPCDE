LLBIN=/usr/lib/llvm-3.9/bin
LLVM_CONFIG=$(LLBIN)/llvm-config
#QUIET:=@
QUIET:=

BUILDJSONCPP:=#
ifdef WITHJSONCPP
BUILDJSONCPP:=
endif

SRC_DIR?=$(PWD)/src
LDFLAGS+=$(shell $(LLVM_CONFIG) --ldflags) -Lthirdparty/jsoncpp-1.8.0/build/src/lib_json -ljsoncpp

COMMON_FLAGS=-Wall -Wextra -g


CXXFLAGS+=$(COMMON_FLAGS) $(shell $(LLVM_CONFIG) --cxxflags)
CPPFLAGS+=$(shell $(LLVM_CONFIG) --cppflags) -std=c++11 -I$(SRC_DIR) -Ithirdparty/jsoncpp-1.8.0/include

LOADABLE_MODULE_OPTIONS=-shared -Wl,-O1

LDIS=$(LLBIN)/llvm-dis
CPP=$(LLBIN)/clang++
CC=$(LLBIN)/clang

BOD=build/obj
PASSMGR=Comminute
OPM=build/bin/$(PASSMGR)
PASS=ComminuteShared.so
PASS_OBJECTS=Analysis/TargetCallSitesPass.o \
  Analysis/StoreCollector.o \
  Transform/FunctionExternalizer.o \
  Transform/ChoosePhiValue.o \
  Analysis/NaiveSensitiveDataLeak.o  \
  Analysis/NaiveFileDescLeak.o  \
  Analysis/PotentiallyDangerousScan.o  \
  Analysis/PotentiallyDangerousScanUserMethod.o  \
  Analysis/PotentiallyDangerousScanFunctionPass.o  \
  Analysis/NaiveConstantArgCheck.o  

# XXX
# This is awful... Im just like "PUT IT ALL IN"
LIBS=$(shell $(LLVM_CONFIG) --libs) -lclang
LIBS+=-lpthread -ldl -lncurses -lz

TDIR=build/tests

default: prep $(PASS) passmgr

prep:
	@echo "Prep phase"
	$(QUIET)mkdir -p build/obj
	$(QUIET)mkdir -p build/obj/Analysis
	$(QUIET)mkdir -p build/obj/Transform
	$(QUIET)mkdir -p build/bin
	$(QUIET)mkdir -p build/lib

define builditdood
$(QUIET)$(CPP) -o $(BOD)/$(1)/$(@F) -c $(CPPFLAGS) $(CXXFLAGS) $<
endef

Transform/%.o: $(SRC_DIR)/Transform/%.cpp
	@echo "Compiling $*.cpp"
	$(call builditdood,Transform)

Analysis/%.o: $(SRC_DIR)/Analysis/%.cpp
	@echo "Compiling $*.cpp"
	$(call builditdood,Analysis)

%.o : $(SRC_DIR)/%.cpp
	@echo "Compiling $*.cpp"
	$(call builditdood,.)

passmgr:
	@echo "Building passmanager clean up ldflags XXX"
	$(QUIET)$(CPP) -o $(BOD)/Comminute.o -c $(CPPFLAGS) $(CXXFLAGS) src/Comminute.cpp
	$(QUIET)$(CPP) -o $(OPM) $(CXXFLAGS) build/obj/Comminute.o ${addprefix $(BOD)/,$(PASS_OBJECTS)} $(LDFLAGS) $(LIBS)


$(PASS) : $(PASS_OBJECTS)
	@echo "Linking $@"
	$(QUIET)$(CPP) -o build/lib/$@ $(LOADABLE_MODULE_OPTIONS) $(CXXFLAGS) $(LDFLAGS) ${addprefix $(BOD)/,$^}

test: testprep testnca testnsdl testpd testnfdl

testprep:
	$(QUIET)mkdir -p $(TDIR)

testnca:
	$(QUIET)$(CC) -o $(TDIR)/NCA001 tests/NCA001.c
	$(QUIET)$(CC) -g -emit-llvm -o $(TDIR)/NCA001.bc -c tests/NCA001.c
	$(QUIET)$(LDIS) $(TDIR)/NCA001.bc
	$(QUIET)$(CC) -o $(TDIR)/NCA002 tests/NCA002.c
	$(QUIET)$(CC) -g -emit-llvm -o $(TDIR)/NCA002.bc -c tests/NCA002.c
	$(QUIET)$(LDIS) $(TDIR)/NCA002.bc

testnsdl:
	$(QUIET)$(CC) -o $(TDIR)/NSDL001 tests/NSDL001.c
	$(QUIET)$(CC) -g -emit-llvm -o $(TDIR)/NSDL001.bc -c tests/NSDL001.c
	$(QUIET)$(LDIS) $(TDIR)/NSDL001.bc
	$(QUIET)$(CC) -o $(TDIR)/NSDL002 tests/NSDL002.c
	$(QUIET)$(CC) -g -emit-llvm -o $(TDIR)/NSDL002.bc -c tests/NSDL002.c
	$(QUIET)$(LDIS) $(TDIR)/NSDL002.bc

testpd:
	$(QUIET)$(CC) -o $(TDIR)/PD001 tests/PD001.c
	$(QUIET)$(CC) -g -emit-llvm -o $(TDIR)/PD001.bc -c tests/PD001.c
	$(QUIET)$(LDIS) $(TDIR)/PD001.bc

testnfdl:
	$(QUIET)$(CC) -o $(TDIR)/NFDL001 tests/NFDL001.c
	$(QUIET)$(CC)  -emit-llvm -o $(TDIR)/NFDL001.bc -c tests/NFDL001.c
	$(QUIET)$(LDIS) $(TDIR)/NFDL001.bc
	$(QUIET)$(CC) -o $(TDIR)/NFDL002 tests/NFDL002.c
	$(QUIET)$(CC) -g -emit-llvm -o $(TDIR)/NFDL002.bc -c tests/NFDL002.c
	$(QUIET)$(LDIS) $(TDIR)/NFDL002.bc
	$(QUIET)$(CC) -o $(TDIR)/NFDL003 tests/NFDL003.c
	$(QUIET)$(CC) -g -emit-llvm -o $(TDIR)/NFDL003.bc -c tests/NFDL003.c
	$(QUIET)$(LDIS) $(TDIR)/NFDL003.bc
	$(QUIET)$(CC) -o $(TDIR)/NFDL004 tests/NFDL004.c
	$(QUIET)$(CC) -g -emit-llvm -o $(TDIR)/NFDL004.bc -c tests/NFDL004.c
	$(QUIET)$(LDIS) $(TDIR)/NFDL004.bc
	$(QUIET)$(CC) -o $(TDIR)/NFDL005 tests/NFDL005.c
	$(QUIET)$(CC) -g -emit-llvm -o $(TDIR)/NFDL005.bc -c tests/NFDL005.c
	$(QUIET)$(LDIS) $(TDIR)/NFDL005.bc
	$(QUIET)$(CC) -o $(TDIR)/NFDL006 tests/NFDL006.c
	$(QUIET)$(CC) -g -emit-llvm -o $(TDIR)/NFDL006.bc -c tests/NFDL006.c
	$(QUIET)$(LDIS) $(TDIR)/NFDL006.bc
	$(QUIET)$(CC) -o $(TDIR)/NFDL007 tests/NFDL007.c
	$(QUIET)$(CC) -g -emit-llvm -o $(TDIR)/NFDL007.bc -c tests/NFDL007.c
	$(QUIET)$(LDIS) $(TDIR)/NFDL007.bc

help:
	@echo "make jsoncpp"
	@echo "make "
	@echo "...See build/"
	@echo "make clean or make cleanall which requires jsoncpp rebuild"
	@echo "make test"
	@echo "make runtests"
	@echo "make runconstantarg"
	@echo "make runsensitiveleak"
	@echo "make runfdleak"
	@echo "make rundangerfn"


runtests: runconstantarg runsensitiveleak runfdleak rundangerfn

runconstantarg:
	@echo "***"
	@echo "***   Running: Naive constant arg on NCA001 ***"
	@echo "***"
	$(QUIET)$(OPM) -naive-constant-arg $(TDIR)/NCA001.bc $(TDIR)/NCA001_out.bc
	@echo "***"
	@echo "***   Running: Naive constant arg on NCA002 ***"
	@echo "***"
	$(QUIET)$(OPM) -naive-constant-arg $(TDIR)/NCA002.bc $(TDIR)/NCA002_out.bc

runsensitiveleak:
	@echo "***"
	@echo "***   Running: Naive sensitive leak on NSDL001 ***"
	@echo "***"
	$(QUIET)$(OPM) -naive-sensitive-data-leak $(TDIR)/NSDL001.bc $(TDIR)/NSDL001_out.bc
	$(QUIET)$(LDIS) $(TDIR)/NSDL001_out.bc
	@echo "***"
	@echo "***   Running: Naive sensitive leak on NSDL002 ***"
	@echo "***"
	$(QUIET)$(OPM) -naive-sensitive-data-leak $(TDIR)/NSDL002.bc $(TDIR)/NSDL002_out.bc
	$(QUIET)$(LDIS) $(TDIR)/NSDL002_out.bc

rundangerfn:
	@echo "***"
	@echo "***   Running: Potentially danger function on PD001 ***"
	@echo "***"
	$(QUIET)$(OPM) -dangerous-function $(TDIR)/PD001.bc $(TDIR)/PD001_out.bc
	@echo "***"
	@echo "***   Running: Potentially danger function via user method on PD001 ***"
	@echo "***"
	$(QUIET)$(OPM) -dangerous-function-user-method $(TDIR)/PD001.bc $(TDIR)/PD001_out-um.bc
	@echo "***"
	@echo "***   Running: Potentially danger function via function pass on PD001 ***"
	@echo "***"
	$(QUIET)$(OPM) -dangerous-function-fpass $(TDIR)/PD001.bc $(TDIR)/PD001_out-fp.bc

runfdleak:
	@echo "***"
	@echo "***   Running: Naive file descriptor leak on NFDL001 ***"
	@echo "***"
	$(QUIET)$(OPM) -naive-fd-leak $(TDIR)/NFDL001.bc $(TDIR)/NFDL001_out.bc
	@echo "***"
	@echo "***   Running: Naive file descriptor leak on NFDL002 ***"
	@echo "***"
	$(QUIET)$(OPM) -naive-fd-leak $(TDIR)/NFDL002.bc $(TDIR)/NFDL002_out.bc
	@echo "***"
	@echo "***   Running: Naive file descriptor leak on NFDL003 ***"
	@echo "***"
	$(QUIET)$(OPM) -naive-fd-leak $(TDIR)/NFDL003.bc $(TDIR)/NFDL003_out.bc
	@echo "***"
	@echo "***   Running: Naive file descriptor leak on NFDL004 ***"
	@echo "***"
	$(QUIET)$(OPM) -naive-fd-leak $(TDIR)/NFDL004.bc $(TDIR)/NFDL004_out.bc
	@echo "***"
	@echo "***   Running: Naive file descriptor leak on NFDL005 ***"
	@echo "***"
	$(QUIET)$(OPM) -naive-fd-leak $(TDIR)/NFDL005.bc $(TDIR)/NFDL005_out.bc
	@echo "***"
	@echo "***   Running: Naive file descriptor leak on NFDL006 ***"
	@echo "***"
	$(QUIET)$(OPM) -naive-fd-leak $(TDIR)/NFDL006.bc $(TDIR)/NFDL006_out.bc
	@echo "***"
	@echo "***   Running: Naive file descriptor leak on NFDL007 ***"
	@echo "***"
	$(QUIET)$(OPM) -naive-fd-leak $(TDIR)/NFDL007.bc $(TDIR)/NFDL007_out.bc
		

# :^D
jsoncpp:
	@echo "Building jsoncpp-1.8.0"
	cd thirdparty &&  \
	  tar zxvf jsoncpp-1.8.0.tar.gz &&  \
	  cd jsoncpp-1.8.0 &&  \
	  rm -rf build &&  \
	  mkdir -p build &&  \
	  cd build &&  \
	  cmake .. &&  \
	  make &&  \
	  cd ../../

jsonclean:
	$(QUIET)rm -rf thirdparty/jsoncpp-1.8.0

clean:
	$(QUIET)rm -rf build

cleanall: clean jsonclean

