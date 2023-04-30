# LettleJVM
Learn the Java bytecode case.

# 开发环境

mac OS 13.3.1

CPU: M1

```bash
$ g++ -v
Apple clang version 14.0.3 (clang-1403.0.22.14.1)
Target: arm64-apple-darwin22.4.0
Thread model: posix
```

```bash
$ make -v
GNU Make 3.81
Copyright (C) 2006  Free Software Foundation, Inc.
This is free software; see the source for copying conditions.
There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

This program built for i386-apple-darwin11.3.0
```

# 快速开始

编译程序

```bash
make all
```

运行程序

```bash
make run
```



# 开发日志

2023/4/30：可以解析文件public类的全部信息，找到main函数并解析运行

执行结果：

```bash
Lettle Java VM V1.0
-----------------------
Using Java 17
magic number: BE BA FE CA 
minor_version: 00 00 
major_version: 00 3D 

public class cn/lettle/Main extends java/lang/Object
{
  constant pool count: 29
  interfaces_count: 0
  fields_count: 0
  methods_count: 2
}
-----------------------
```

