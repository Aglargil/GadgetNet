# GadgetNet

## 简介

本项目是参考[(chenshuo/muduo)](https://github.com/chenshuo/muduo)实现的一个小型的基于Reactor模型的多线程网络库。

项目目前实现了基础的事件循环模块`EventLoopPool`, 接下来将完善TcpServer部分的内容。

## 开发环境

- 操作系统 `Ubuntu 20.04.5 LT`
- 编译器 `g++ 9.4.0`
- 项目构建 `cmake 3.16.3`

## 编译

```bash
mkdir build && cd build
cmake ..
make
make test
```

