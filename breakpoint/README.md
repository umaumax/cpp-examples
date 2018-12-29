# breakpoint

[c \- Printing all global variables/local variables? \- Stack Overflow]( https://stackoverflow.com/questions/6261392/printing-all-global-variables-local-variables )
```
$ gdb ./breakpoint
>>> run
>>> bt
#0  0x00007ffff76c0428 in __GI_raise (sig=5) at ../sysdeps/unix/sysv/linux/raise.c:54
#1  0x0000000000400726 in main (argc=1, argv=0x7fffffffda88) at main.cpp:9
>>> select-frame 1
>>> info locals
x = 1
>>> continue
```

