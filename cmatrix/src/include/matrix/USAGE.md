# USAGE

## 编译

来到最顶层目录，如果使用 `tree -L 1`，应该会有类似如下输出：

```text
$ tree -L 1

.
├── build.sh
├── configure.sh
├── data
├── docs
├── output
├── README.md
└── src
```

然后执行 `./configure.sh D` 编译 **Debug** 版本，
或者执行 `./configure.sh R` 编译 **Release** 版本

最后执行 `./build.sh install` 编译，编译结果会在 `./pack` 目录下，
同时还会生成一个 `cmatrix.tar.gz` 压缩包，压缩包中包含库文件和头文件

## 使用

解压缩 `cmatrix.tar.gz` 文件，
或者复制 `./pack` 目录到你想要的目录，
最好将 `pack` 重命名为 `matrix` 之类的名字

编译时使用

```shell
cc source.c -L./path/to/matrix -lmatrix -o app
```

其中，`-L` 指明库文件所在的路径，不包含文件名，
`-l` 指明链接的库的名字是 `matrix`，
`-o` 就是指定编译出来的程序的名字

除了使用编译好的静态库，还可以选择使用源码一起编译，
即将 `./src/main.c` 中内容替换为你的内容
