# Matrix

> License: GPL-3.0-or-later

一个用 C 写的矩阵操作库，没有效率，就是自己写着玩

编译之后在 `output` 目录下有个 `matrix` 目录，编译好的静态库就在里面，
如果没有意外，在 Linux 下应该叫做 `libmatrix.a`，然后直接：

```shell
cc source.c -L./lib -lmatrix -o app
```

也就是把编译好的静态库放到某个目录下，这里是放到了源码的同级目录下的 `lib` 目录里，
然后使用 `-L` 指向那个目录，最后链接时使用 `matrix` 这个名字，`-o` 就是指定编译出来的程序的名字

除了使用编译好的静态库，还可以选择使用源码一起编译