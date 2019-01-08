# static variable in inline funtion

__TL;DR__
* Avoid to use static variable in inline function. (too slow)

## Darwin(Mac)
* 通常は`weak`でglobalな領域に持つ
* inlineの場合には`GOT(Global Offset Table)`経由

### no inline
```
	movl	__ZZ13no_inline_addiiE3cnt(%rip), %esi
	addl	$1, %esi
	movl	%esi, __ZZ13no_inline_addiiE3cnt(%rip)
	.loc	1 12 10                 ## main.cpp:12:10
	movl	__ZZ13no_inline_addiiE3cnt(%rip), %esi
```
```
.zerofill __DATA,__bss,__ZZ13no_inline_addiiE3cnt,4,2 ## @_ZZ13no_inline_addiiE3cnt
	.section	__DATA,__data
	.globl	__ZZ10inline_addiiE3cnt ## @_ZZ10inline_addiiE3cnt
	.weak_definition	__ZZ10inline_addiiE3cnt
	.p2align	2
__ZZ10inline_addiiE3cnt:
	.long	0                       ## 0x0
```

### inline
```
movq	__ZZ10inline_addiiE3cnt@GOTPCREL(%rip), %rax
```

----

## FYI
* [C言語のinline \- 簡潔なQ]( https://qnighy.hatenablog.com/entry/2017/03/10/002559 )

> C++のinline関数は、複数の翻訳単位が実体を提供してもよい。このあたりはリンカが頑張る。そのため、 inline と extern inline の区別はなく、いずれの場合も実体が生成される。その他、宣言していてもodr-usedでない場合の制限が緩い、inline関数がstatic変数を持っていてもよいなどいくつかの違いがある。

* [c\+\+ \- static variables in an inlined function \- Stack Overflow]( https://stackoverflow.com/questions/185624/static-variables-in-an-inlined-function )
