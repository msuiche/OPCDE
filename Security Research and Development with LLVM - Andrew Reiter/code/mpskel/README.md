
# MPSkel

This is a module pass skeleton.


# Build & Run

First check the Makefile to set path to llvm-config and version.
3.8, 3.9 should be fine, so should 4.0

```
$ make
$ opt-X.Y -load built/MPSkel.so -mpskel < file.bc
...
$
```


