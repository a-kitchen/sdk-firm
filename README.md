# 启厨智能烹饪固件 SDK

“启厨智能烹饪固件 SDK”适用于多种嵌入式硬件平台，SDK 由开发智能烹饪固件的代码库和其他信息组成，包含步序引擎、火控逻辑和射频协议栈功能。

## SDK 组成

* 目标代码库
* 示例源程序
* 目标硬件参考设计
* 说明文档

## 目标系统功能

目标系统可以是一下两种功能之一

* 数据透传：目标系统将用于在启厨智能烹饪 app 或兼容启厨智能烹饪系统的其他 app，及另外具备独立控制功能的烹饪设备间传递烹饪数据。
* 集成控制：目标系统本身具备完全的烹饪控制功能，同时还能连接启厨智能烹饪 app 或兼容启厨智能烹饪系统的其他 app。

## 目标硬件

SDK 目标硬件最低要求

* Cortext M0，主频 48-MHz
* RAM 16-KByte
* ROM/flash 128-MByte
* Bluetooth Smart (BLE 4.0)

可选的部分硬件功能

* UART 通信接口
* I2C 通信接口
* SPI 通信接口
* WiFi 射频通信接口
* 温度传感器接口
* 按键开关接口
* 数码管显示接口

目前 SDK 已经在如下嵌入式处理器系统上调试运行成功

* Cypress PSoC 4
* Telink 8267