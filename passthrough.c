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
