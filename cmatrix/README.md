# Matrix

> License: GPL-3.0-or-later

一个用 C 写的矩阵操作库，没有效率，就是自己写着玩

使用方法：

```shell
cc source.c -L./path/to/libs -lmatrix -o app
```

也就是把编译好的静态库放到一个地方，使用 `-L` 指向那个目录，然后链接时使用 `matrix` 这个名字

不过也可以使用源码一起编译
