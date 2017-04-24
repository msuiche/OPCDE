
# intflip

Currently, what this does is replaces all 8, 16, 32, and 64 bit integers arguments to functions
with possibly randomly changed values. The purpose is to re-run test suites 
with the modified application in order to simulate the low-probability bit or
more flips that could occur in certain extreme situations. The aim is to perform
some basic analysis as to the stability of the code given random changes.

## Aside

- I do not do much of anything with being sane about memory usage. You're warned. :-P

- The way I do the RNG bit is possibly overkill. *shrug*. ISO C standard
one should be fine.

- The randomizer insertion could just be C code that gets linked in, but
going through the generation and insertion of a function is a good 
exercise.

# Requirements

- LLVM 3.9.0 (should work with 3.8 & 4.0)

# Building

$ make jsoncpp
$ make

# Process 

All executables listed below are supposed to be in your path. You will 
``compile'' to IR and then work on that. I would follow the basic steps
and then determine how you want to handle performing the analysis. The
analysis would be a combination of running unit, or other, tests with 
the injected randomizers and analyzing how those runs performed given
the probability distribution you are looking at.

## Passes

This is a list of passes available. They are intended to be used per the
``basic steps'' section below.

- -lift-constant-int-args
- -inject-randomizers
- -replace-ints-visitor
- -replace-ints-iterate
- -replace-ints-cgpass

## Basic Work Flow 

*Setup replace.cfg*

The file replace.cfg informs the passes how to setup the random integer replacement action. As the 
key, you specify the function that will have any call instructions /in/ it that have at least one
integer argument, replaced to use an integer value that is randomly selected.

Each value is a dict that should have the keys/values:
- "analyze" : true|false
- "type" : 0|1 ... 0 for randomly replace with random value. 1 for randomly replace with random bit flipp
- "mean" : we are dealing with 32-bit integers, and compare <= mean on random number. 
The for
{
	"foo" : {
	  "analyze" : true,
	  "type" : 0,
	  "mean" : 500
	}
}

*Compile your code to IR*

> clang++-3.9 -emit-llvm -o foo.bc -c foo.c

Of course with a larger code base there will be more work involved.
There is a tool that is out there to help with, at least, the merging
of multiple bitcode files, it may be found [link](https://github.com/travitch/whole-program-llvm "whole-program-llvm").

*Lift ConstantInt to local variable*

> opt-3.9 -load built/libIntFlip.so -lift-constant-ints -o foo2.bc < foo.bc

This will convert constant integers into local variables. This helps
the next step of replacing integers used with randomizer function as
there is no special casing for constant integers.. we just go after
the local vars.

*Inject the randomizer functions*

> opt-3.9 -load built/libIntFlip.so -inject-randomizers  -o foo3.bc < foo2.bc

This generates the functions that will possibly change integer values 
and injects them into the bitcode file. We could just link in code, but
we do this to demonstrate some writing of our own functions via the API.

*Modify integer arguments to use randomizer functions*

> opt-3.9 -load built/libIntFlip.so -replace-ints -o foo4.bc < foo3.bc

*Build the executable*

> llc-3.9 -o=foo4.s foo4.bc
> clang++-3.9 -o foo4 foo4.s

or however you want...

*Dump IR from resultant bitcode file*

> llvm-dis-3.9 -o=foo.ll test/foo.bc

You can do the above for each bitcode file and compare them.


# Some things one might wish to do 

- Wrap all and use a pass manager
-- add pass dependencies for ordering
- Improved control over configurations
- Build out a whole test run harness 
-- run test cases that have known outcomes (typically, just unit tests and some other)
-- adjusting probability distribution mean and seeing how that impacts results
- Improved probability distributions.. 
-- evolutionary (evolve with execution steps)
- Overall model of app that is intelligently injected based on a model of a specific event
-- that would be like a model based on a real gamma ray birst or something weird
- Randomly change instructions
-- Could be done in a few ways, but would want to be arch specific
-- to find all 1-bit mutatable instructions for instruction i, find all instructions j st dist(i,j) = 1
-- similar for 2 bit.. just make dist(i,j) = 2...
-- Then lift those sets to IR
-- so.. you attempt to fix it in IR.. this will not always work because of ``nice graph'' desires.
- whatever.


# Inspirational credits
- NASA
- John Regehr (Utah)
- Gamma rays, alpha particles
