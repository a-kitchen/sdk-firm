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

## 文件

* ak.h -- sdk 代码库头文件
* passthrough.h -- 透传示例代码源文件

## 透传示例

```c
#include <project.h>
#include <ak.h>

#define	LEN	0                                       // 数据帧长度占位符
#define	SUM	0                                       // 数据帧校验和占位符

#define	DISP_BEAT	0x01
#define	DISP_CLOCK	0x02

static U08 beat;
static U32 clock;
static U08 disp;
static U08 param;

static void Callback(U08 key) {
  param = key;
}

static void Recv(U08 value) {                       // 解析下行数据
  static U08 a, k, n;

  a += value;
  n--;
  switch (k) {
  case 0:                                           // 检查帧头
    if (value == 'A')
      k = 1;
    return;
  case 1:                                           // 检查帧头
    if(value == 'K')
      k = 2;
    else k = value == 'A'? 1 : 0;
    return;
  case 2:
    if(value < 4)
      k = value == 'A'? 1 : 0;
    else {
      a = 'A' + 'K' + value;
      k = 3;
      n = value - 2;                                // 提取帧长度
    }
    return;
  case 3:                                           // 越过校验和
    k = 4;
    return;
  case 4:
    if(n) {
      k = value;                                    // 提取数据键
      return;
    }
    break;
  default:
    if(n) {                                         // 未到达帧尾
    }
  }
  k = value == 'A' ? 1 : 0;
}

/*static void send(U08 *bits, U08 len) {
  U08 sum;
  sum = 0;
  bits[2] = len;
  bits[3] = sum;
  for(U08 i = 0; i < len; i++)
    sum -= bits[i];
  bits[3] = sum;
  SCB_SpiUartPutArray(bits, len);
}*/

static void Send(void) {
}

static void Clock_Start() {
}

int main(void) {
  Ak_Prepare();                                     // 准备 OTA 参数，必须首先调用
  CyGlobalIntEnable;                                // 系统初始化
  Clock_Start();                                    // 时钟初始化
  SCB_Start();                                      // 串口初始化
  Ak_Start(Callback);                               // 启动引擎

  for(;;) {
    if (disp & DISP_BEAT) {                         // 心跳，16 次/秒
      disp &= ~DISP_BEAT;                           // 清除心跳标志
      Ak_OnBeat(beat);                              // 发送心跳至引擎
      Send();                                       // 发送下行数据
	}
    if (disp & DISP_CLOCK) {                        // 秒时钟
      disp &= ~DISP_CLOCK;                          // 清除秒时钟标志
      Ak_OnClock(clock);                            // 发送秒时钟至引擎
	}
    if (param) {                                    // 参数变化
      param = 0;                                    // 清除参数标志
    }
    if(SCB_SpiUartGetRxBufferSize())                // 获取上行数据
      Recv(SCB_SpiUartReadRxData());                // 解析数据
    Ak_OnIdle();                                    // 空闲操作时间片
  }
}
```