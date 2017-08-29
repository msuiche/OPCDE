
The code will look for any/all pointer function arguments and will
insert an assert(ptr != NULL); statement. It shows looking at
functions, their arguments, a function pass, declaring a function,
and inserting code. Why? Eh. A few reasons that are BS, but mostly
to help learn API.


## Build and Run 

This requires LLVM and Clang 3.8, 3.9, or 4.0 releases. You should
review the Makefile to setup the LLVM path and version.

```
$ make
$ make tests
$ cd tests
```

If you do not specify a configuration file, the code will look for all
functions and their arguments. If you specify a configuration file, 
it will specify the function and the argument to check. The tests/ex0*b....
files are those in which a configuration file was specified and the
ex0*a are those in which none was specified.. so you should see a difference

### Example running by hand
```
$ clang -g -emit-llvm -c -o FOO.bc FOO.c
$ opt -load built/libnpassert.so -null-ptr-assert -o FOO_assertall.bc < FOO.bc
$ opt -load built/libnpassert.so -null-ptr-assert -npa-target-config FOO.cfg -o FOO_assertbyconfigfile.bc < FOO.bc
$ clang -g -o FOO_assertall FOO_assertall.bc
$ clang -g -o FOO_assertbyconfigfile FOO_assertbyconfigfile.bc 
```

The last clang steps generate an runnable executable.
Use llvm-dis on bitcode (.bc) files to get the human readable IR
to view the differences. One may also use the -npa-use-function option 
which will tell the pass to create a separate function to perform
the assertion in.

So with the above instead of getting a crash like:
```
(gdb) r
Starting program: /home/hoser/code/npassert/test/ex02 

Program received signal SIGSEGV, Segmentation fault.
0x0000000000400549 in foo (s=0x0) at test/ex02.c:16
16              return s->one;
(gdb) disass foo
Dump of assembler code for function foo:
   0x0000000000400530 <+0>:     push   %rbp
   0x0000000000400531 <+1>:     mov    %rsp,%rbp
   0x0000000000400534 <+4>:     sub    $0x10,%rsp
   0x0000000000400538 <+8>:     mov    %rdi,-0x8(%rbp)
   0x000000000040053c <+12>:    callq  0x400420 <random@plt>
   0x0000000000400541 <+17>:    mov    %rax,-0x10(%rbp)
   0x0000000000400545 <+21>:    mov    -0x8(%rbp),%rax
=> 0x0000000000400549 <+25>:    mov    (%rax),%eax
   0x000000000040054b <+27>:    add    $0x10,%rsp
   0x000000000040054f <+31>:    pop    %rbp
   0x0000000000400550 <+32>:    retq   
End of assembler dump.
(gdb) 
```

You would get a crash early in the function..
```
(gdb) r
Starting program: /home/hoser/code/npassert/test/ex02a 
Program received signal SIGSEGV, Segmentation fault.
0x0000000000400554 in foo (s=0x0) at test/ex02.c:12
12      {
(gdb) disass foo
Dump of assembler code for function foo:
   0x0000000000400530 <+0>:     push   %rbp
   0x0000000000400531 <+1>:     mov    %rsp,%rbp
   0x0000000000400534 <+4>:     sub    $0x20,%rsp
   0x0000000000400538 <+8>:     cmp    $0x0,%rdi
   0x000000000040053c <+12>:    mov    %rdi,-0x8(%rbp)
   0x0000000000400540 <+16>:    jne    0x400558 <foo+40>
   0x0000000000400546 <+22>:    xor    %eax,%eax
   0x0000000000400548 <+24>:    mov    %eax,%ecx
   0x000000000040054a <+26>:    mov    %rsp,%rdx
   0x000000000040054d <+29>:    add    $0xfffffffffffffff0,%rdx
   0x0000000000400551 <+33>:    mov    %rdx,%rsp
=> 0x0000000000400554 <+36>:    mov    (%rcx),%eax
   0x0000000000400556 <+38>:    mov    %eax,(%rdx)
   0x0000000000400558 <+40>:    mov    %rsp,%rax
   0x000000000040055b <+43>:    add    $0xfffffffffffffff0,%rax
   0x000000000040055f <+47>:    mov    %rax,%rsp
   0x0000000000400562 <+50>:    mov    %rsp,%rcx
   0x0000000000400565 <+53>:    add    $0xfffffffffffffff0,%rcx
   0x0000000000400569 <+57>:    mov    %rcx,%rsp
   0x000000000040056c <+60>:    mov    -0x8(%rbp),%rdx
   0x0000000000400570 <+64>:    mov    %rdx,(%rax)
   0x0000000000400573 <+67>:    mov    %rax,-0x10(%rbp)
   0x0000000000400577 <+71>:    mov    %rcx,-0x18(%rbp)
   0x000000000040057b <+75>:    callq  0x400420 <random@plt>
   0x0000000000400580 <+80>:    mov    -0x18(%rbp),%rcx
   0x0000000000400584 <+84>:    mov    %rax,(%rcx)
   0x0000000000400587 <+87>:    mov    -0x10(%rbp),%rax
   0x000000000040058b <+91>:    mov    (%rax),%rdx
   0x000000000040058e <+94>:    mov    (%rdx),%eax
   0x0000000000400590 <+96>:    mov    %rbp,%rsp
   0x0000000000400593 <+99>:    pop    %rbp
   0x0000000000400594 <+100>:   retq   
```

Or if you add the  -npa-use-function option
```
(gdb) r
Starting program: /home/hoser/code/npassert/test/ex02c 

Program received signal SIGSEGV, Segmentation fault.
0x00000000004005b3 in __NPA_assert_8__ ()
(gdb) 
```
it will fault in the assertion function.
