
# VisitorSkel

This is a module pass that runs an instruction visitor
on each function it encounters in the given compilation
unit. 

## Build

Check the makefile to be sure the LLVM version and llvm-config
paths are ok. Then just make. This will produce build/VisitorSkel.so.

## Run

```
$ clang -emit-llvm -o file.bc -c file.c
$ opt-X.Y -load built/VisitorSkel.so -vskel < file.bc
...
$ opt-X.Y -load built/VisitorSkel.so -vskel -fn-to-visit someFuncName < file.bc
...
```

