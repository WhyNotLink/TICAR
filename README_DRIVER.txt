================================================================================
  天猛星 MSPM0G3507 小车底座 — BSP 驱动使用说明
================================================================================

【目录结构】
  empty/
  ├── empty.syscfg              <- SysConfig 引脚/外设配置
  ├── empty.c                   <- 主程序示例
  ├── ti_msp_dl_config.h/.c     <- SysConfig 自动生成（不要手动修改）
  └── bsp/
      ├── board.h / board.c     <- 公共头文件(类型定义、延时函数、OLED引脚映射)
      ├── soft_oled.h / .c      <- OLED 0.96寸 IIC 单色屏驱动 (软件I2C)
      ├── oledfont.h            <- OLED 字库
      ├── bmp.h                 <- OLED 位图数据
      ├── bsp_motor.h / .c      <- TB6612 直流电机 + MG513 编码器驱动 (4通道)
      ├── bsp_a4988.h / .c      <- A4988 步进电机驱动 (2通道)
      ├── bsp_mpu6050.h / .c    <- MPU6050 六轴陀螺仪驱动 (I2C0)
      ├── bsp_ultrasonic.h / .c <- HC-SR04 超声波测距驱动
      ├── bsp_grayscale.h / .c  <- 8通道灰度传感器驱动
      ├── bsp_button.h / .c     <- 2路按键驱动 (带消抖)
      ├── bsp_led.h / .c        <- 4路LED驱动
      ├── bsp_buzzer.h / .c     <- 蜂鸣器驱动
      └── README_DRIVER.txt     <- 本文件

================================================================================
  一、开发环境搭建
================================================================================

  1. 安装 Keil MDK-ARM (V5.38+)
  2. 安装 MSPM0 SDK (mspm0_sdk_2_02_00_05)
  3. 安装 SysConfig 1.21+ (https://www.ti.com/tool/SYSCONFIG)
  4. 用 Keil 打开 keil/empty_LP_MSPM0G3507_nortos_keil.uvprojx
  5. 打开 empty.syscfg → 点击 "Generate" 生成 ti_msp_dl_config.h/.c
  6. 编译下载

================================================================================
  二、引脚总览
================================================================================

  【TB6612 直流电机 (TIMA0 PWM, 4通道)】
  MOTORA_PWM  : PB12 (TIMA0-C2)   MOTORA_DIR1: PB15   MOTORA_DIR2: PB16
  MOTORB_PWM  : PB13 (TIMA0-C3)   MOTORB_DIR1: PB5    MOTORB_DIR2: PB4
  MOTORC_PWM  : PA8  (TIMA0-C0)   MOTORC_DIR1: PB9    MOTORC_DIR2: PB7
  MOTORD_PWM  : PA9  (TIMA0-C1)   MOTORD_DIR1: PB26   MOTORD_DIR2: PA13

  【MG513 编码器 (GPIO中断, 4通道)】
  ENCA_A: PB10   ENCA_B: PB11
  ENCB_A: PA29   ENCB_B: PB27
  ENCC_A: PA26   ENCC_B: PA27
  ENCD_A: PB21   ENCD_B: PB19

  【A4988 步进电机 (2通道)】
  PTZ1_PWM : PB2  (TIMA1-C0)    PTZ1_DIR: PA12
  PTZ2_PWM : PB3  (TIMG6-C1)    PTZ2_DIR: PB23

  【MPU6050 陀螺仪 (I2C0, 400kHz)】
  SDA_MPU: PA28   SCL_MPU: PA31

  【OLED 0.96寸 IIC屏 (软件I2C, PA15=SCL, PA16=SDA)】
  OLED_SCL: PA15   OLED_SDA: PA16   地址: 0x78 (0x3C<<1)

  【HC-SR04 超声波】
  TRIG: PB25   ECHO: PB20 (TIMG12-C0, 4MHz/32位, 40ms周期)

  【灰度传感器 8通道 (数字输入)】
  OUT1: PA24  OUT2: PA25  OUT3: PB24  OUT4: PA22
  OUT5: PA14  OUT6: PA17  OUT7: PB17  OUT8: PB18

  【按键 2个 (中断触发, 下降沿)】
  KEY1: PA0    KEY2: PA1

  【LED 4个 (初始值=SET=高电平=灯灭)】
  LED1: PB22   LED2: PA30   LED3: PB0   LED4: PB1

  【蜂鸣器 (有源, 低电平响)】
  BEEP: PA7

  【串口 UART (不定长接收)】
  TX: PA10   RX: PA11 (UART0, 9600bps/8N1, RX中断+RX超时帧检测)

  【调试口 (SWD)】
  SWDIO: PA19     SWCLK: PA20

  【7合1拨码开关 — 硬件直连, 不经过MCU GPIO】
  以下开关通过高低电平直接控制各模块的EN/使能脚:
    PTZ1_EN → A4988-1  EN脚      PTZ2_EN → A4988-2  EN脚
    MAB_EN  → TB6612-1 EN脚      MCD_EN  → TB6612-2 EN脚
    MS1/MS2/MS3 → A4988 微步细分脚 (M0/M1/M2)
  软件无需读取开关状态；开关闭合=模块使能，断开=模块禁用。

  【调试口 (SWD)】
  SWDIO: PA19     SWCLK: PA20

================================================================================
  三、定时器/外设时钟配置 (BUSCLK=32MHz)
================================================================================

  MOTOR_PWM    ── TIMA0, 32MHz÷8÷4=1MHz, period=1000 → PWM频率=1kHz
                  4通道: CC0(PA8), CC1(PA9), CC2(PB12), CC3(PB13)
                  Edge-Aligned, INIT_VAL_LOW, 无反转 → HIGH=导通电机

  PTZ1_PWM     ── TIMA1, 32MHz÷8÷100=40kHz, period=1000 → 动态调频
                  CC0(PB2), duty=50%

  PTZ2_PWM     ── TIMG6, 32MHz÷8÷100=40kHz, period=1000 → 动态调频
                  CC1(PB3), duty=50%

  PID_TIMER    ── TIMG7, 32MHz÷8÷4=1MHz, load=9999 → 中断周期=10ms
                  触发 ZERO 事件中断, 运行 PID 闭环和编码器速度计算

  ULTRASONIC   ── TIMG12, 32MHz÷8=4MHz, load=159999 → 计数器周期=40ms
  _TIMER          32位向下计数, 1 tick=0.25µs, CCP0(PB20) Capture模式
                  (驱动中手动读写计数器测脉宽, 不使用捕获中断)

  I2C_0        ── I2C0, 400kHz (MPU6050 通信)

  UART_0       ── UART0, 9600bps/8N1, RX 中断 + RX 超时(20位周期≈2.08ms)
                  不定长帧检测: RTIM 超时触发帧就绪标志

================================================================================
  四、各驱动 API 速查
================================================================================

--------- 4.1 TB6612 直流电机 (bsp_motor.h) ---------

  初始化:     Motor_Init()
  设置速度:   Motor_SetSpeed(MOTOR_A, 500)     // 范围 -1000~1000, 负数=反转
  设置方向:   Motor_SetDirection(MOTOR_A, MOTOR_FORWARD)
  使能/禁用:  Motor_Enable(MOTOR_A)  /  Motor_Disable(MOTOR_A)
  通道选择:   Motor_EnableChannels(0x05)        // bit0=A,bit1=B,bit2=C,bit3=D
                                                // 0x05 = 只启用A和C
  读编码器:   int32_t enc = Motor_GetEncoder(MOTOR_A)
  清编码器:   Motor_ClearEncoder(MOTOR_A)
  读转速:     int32_t rpm = Motor_GetSpeed(MOTOR_A)
  PID参数:    Motor_SetPID(MOTOR_A, 1.5, 0.02, 0.01)  // Kp, Ki, Kd

  注意:
  - 默认启用 Motor A 和 C (两个后轮差速驱动)
  - 如需4轮全用: Motor_EnableChannels(0x0F)
  - AB 和 CD 各由一个 TB6612 驱动，硬件 EN 由拨码开关 MAB_EN/MCD_EN 控制
  - PID闭环在 TIMG7 ZERO中断中自动运行 (10ms周期)
  - 编码器分辨率: 12PPR × 4(正交) × 28(减速比) = 1344计数/圈 (MG513X GMR)
  - 编码器中断在 GROUP1_IRQHandler 中统一处理

--------- 4.2 A4988 步进电机 (bsp_a4988.h) ---------

  初始化:     A4988_Init()
  设置方向:   A4988_SetDirection(PTZ1, A4988_DIR_CW)
  设置速度:   A4988_SetSpeed(PTZ1, 1000)         // 1000步/秒
  移动步数:   A4988_Step(PTZ1, 200)              // 移动200步(阻塞)
  使能/禁用:  A4988_Enable(PTZ1) / A4988_Disable(PTZ1)

  注意:
  - 硬件 EN 和微步细分(MS1/MS2/MS3)由拨码开关直连A4988, 软件不控制
  - PTZ1 使用 TIMA1 CC0 硬件PWM脉冲
  - PTZ2 使用 TIMG6 CC1 硬件PWM脉冲
  - 速度通过动态修改定时器周期实现, 频率范围可调
  - A4988_Enable/Disable 控制软件侧是否输出脉冲 (硬件EN由拨码开关控制)

--------- 4.3 MPU6050 (bsp_mpu6050.h) ---------

  初始化:     MPU6050_Init()
  更新数据:   MPU6050_Update()                   // 每次读取前调用
  读加速度:   float ax = MPU6050_GetAccelX_G()   // 单位: g
  读角速度:   float gz = MPU6050_GetGyroZ_DPS()  // 单位: 度/秒

--------- 4.4 OLED 屏幕 (soft_oled.h) ---------

  初始化:     OLED_Init()
  清屏:       OLED_Clear()
  显示字符串: OLED_ShowString(0, 0, "Hello", 16, 1)  // x,y,字符串,字号,模式
  显示数字:   OLED_ShowNum(0, 2, 1234, 4, 16, 1)     // x,y,数字,位数,字号,模式
  显示汉字:   OLED_ShowChinese(0, 4, 0, 16, 1)        // x,y,序号,字号,模式
  画点/线/圆: OLED_DrawPoint / OLED_DrawLine / OLED_DrawCircle
  刷新显存:   OLED_Refresh()                     // 修改GRAM后调用

  字号选项: 8(6x8), 12(6x12), 16(8x16), 24(12x24)
  模式: 0=反色, 1=正常
  汉字序号: 参考 oledfont.h 中 Hzk1 数组 (0=中,1=景,2=园...)

--------- 4.5 超声波 HC-SR04 (bsp_ultrasonic.h) ---------

  初始化:     Ultrasonic_Init()
  测距:       float dist = Ultrasonic_GetDistance() // 返回cm, 超时=-1

  注意:
  - SysConfig: TIMG12, 4MHz, 32位向下计数, 40ms周期
  - 驱动中手动读写硬件计数器 (DL_TimerG_getCount) 测量脉宽
  - 不使用捕获中断, 无需在 GROUP1_IRQHandler 中添加超声波代码
  - 10µs TRIG 脉冲 → 等待 ECHO 上升/下降沿 → 计算距离

--------- 4.6 灰度传感器 (bsp_grayscale.h) ---------

  读单通道:   uint8_t v = Grayscale_Read(0)      // ch=0~7, 0=黑 1=白
  读全部:     uint8_t all = Grayscale_ReadAll()   // bit0~bit7 对应 OUT1~OUT8

--------- 4.7 按键 (bsp_button.h) ---------

  初始化:     Button_Init()
  任务轮询:   Button_Task()                       // 每10ms调用一次
  读状态:     Button_Read(0)                      // 0=按下 1=松开
  读按下事件: Button_GetPress(0)                  // 下降沿触发, 自动清除
  读长按:     Button_GetLongPress(0, 1000)        // 按下超过1000ms返回1

--------- 4.8 LED (bsp_led.h) ---------

  初始化:     LED_Init()
  开/关/翻转: LED_On(LED1) / LED_Off(LED1) / LED_Toggle(LED1)
  全部控制:   LED_AllOn() / LED_AllOff()
  注意:      LED 低电平点亮 (DL_GPIO_clearPins=亮, setPins=灭)

--------- 4.9 蜂鸣器 (bsp_buzzer.h) ---------

  初始化:     Buzzer_Init()
  鸣叫:       Buzzer_Beep(200)                   // 鸣叫200ms(阻塞)
  多次鸣叫:   Buzzer_BeepTimes(3, 100, 50)       // 鸣3次,100ms on,50ms off
  注意:      有源蜂鸣器, 低电平响

--------- 4.10 串口 (bsp_uart.h) ---------

  【SysConfig 配置步骤】
  1. 在 SysConfig GUI 中添加 UART 模块
  2. Name: "UART_0", 选择 UART0 外设
  3. TX Pin: PA10, RX Pin: PA11
  4. Baud Rate: 9600, Data Bits: 8, Parity: None, Stop Bits: 1
  5. 勾选 "Enable RX FIFO Interrupts" (接收中断)
  6. 保存生成

  初始化:     UART_Init(9600)
  发送单字节: UART_SendByte(0xAA)
  发送数组:   UART_SendBytes(buf, len)
  发送字符串: UART_SendString("Hello\r\n")

  【不定长接收 (帧模式)】
  等待帧:     if (UART_IsFrameReady()) {         // 超时后帧就绪
                  uint8_t buf[64];
                  uint16_t len = UART_ReadFrame(buf, 64);
                  // 处理 buf[0..len-1]
              }

  【逐字节接收】
  可读字节数: uint16_t n = UART_Available()
  读单字节:   uint8_t b = UART_ReadByte()
  读多个:     uint16_t n = UART_ReadBytes(buf, 64)

  清空缓冲区: UART_Flush()

  【中断处理 (在 UART0_IRQHandler 中调用) */
   void UART0_IRQHandler(void) { UART_IRQHandler(); }

  注意:
  - RX 超时用于帧检测, 发送方连续发送的数据会在间隙 > RTIM 后识别为一帧
  - 若只需简单接收, 可直接用 UART_Available() + UART_ReadByte() 轮询

================================================================================
  五、典型应用示例
================================================================================

【示例1: 两轮差速小车 (Motor A + C)】
  #include "bsp_motor.h"

  Motor_Init();                          // 只启用A和C
  Motor_SetSpeed(MOTOR_A, 500);          // A电机 50%前进
  Motor_SetSpeed(MOTOR_C, 500);          // C电机 50%前进 (直线)

  // 差速转向:
  Motor_SetSpeed(MOTOR_A, 500);          // 左轮快
  Motor_SetSpeed(MOTOR_C, 200);          // 右轮慢 → 右转

【示例2: 超声波避障】
  #include "bsp_ultrasonic.h"
  #include "bsp_motor.h"

  float dist = Ultrasonic_GetDistance();
  if (dist > 0 && dist < 20.0f) {        // 距离小于20cm
      Motor_SetSpeed(MOTOR_A, -300);     // 后退
      Motor_SetSpeed(MOTOR_C, -300);
      Buzzer_Beep(100);                  // 蜂鸣报警
  }

【示例3: 步进云台控制】
  #include "bsp_a4988.h"

  // 确保拨码开关 PTZ1_EN 闭合(硬件使能)
  A4988_SetDirection(PTZ1, A4988_DIR_CW);
  A4988_SetSpeed(PTZ1, 800);
  A4988_Step(PTZ1, 200);                 // 顺时针转200步

【示例4: OLED 显示传感器数据】
  #include "soft_oled.h"
  #include "bsp_mpu6050.h"

  MPU6050_Update();
  OLED_ShowString(0, 0, (uint8_t*)"Angle:", 16, 1);
  OLED_ShowNum(50, 0, (int)MPU6050_GetAccelX_G(), 3, 16, 1);
  OLED_Refresh();

================================================================================
  六、关于 7合1 拨码开关（硬件直连）
================================================================================

  7合1开关不经过MCU的GPIO，而是通过高低电平直接连到各模块:

    PTZ1_EN → A4988-1 的 ENABLE 脚
    PTZ2_EN → A4988-2 的 ENABLE 脚
    MAB_EN  → TB6612-1 的 ENABLE 脚 (控制 Motor A+B)
    MCD_EN  → TB6612-2 的 ENABLE 脚 (控制 Motor C+D)
    MS1     → A4988-1/2 的 MS1 脚
    MS2     → A4988-1/2 的 MS2 脚
    MS3     → A4988-1/2 的 MS3 脚

  开关闭合 → 对应模块使能；开关断开 → 对应模块禁用。

  MS1/MS2/MS3 组合决定步进细分:
    MS1=0 MS2=0 MS3=0 → 全步 (Full Step)
    MS1=1 MS2=0 MS3=0 → 半步 (Half Step)
    MS1=0 MS2=1 MS3=0 → 1/4 步
    MS1=1 MS2=1 MS3=0 → 1/8 步
    MS1=1 MS2=1 MS3=1 → 1/16 步

  软件侧无需读取开关状态，A4988_Enable/Disable 仅控制是否输出脉冲。

================================================================================
  七、修改 SysConfig 后重新生成
================================================================================

  如果需要在 SysConfig GUI 中修改配置:
  1. 打开 empty.syscfg
  2. 修改引脚/外设
  3. Ctrl+S 保存 → 自动生成新的 ti_msp_dl_config.h/.c
  4. 如果引脚宏名称变化, 需要同步更新 bsp/board.h 中的 OLED 引脚映射
  5. 重新编译

================================================================================
  八、注意事项
================================================================================

  1. OLED 使用软件 I2C (PA15/PA16)，与硬件 I2C1 引脚复用。
     如果改用硬件 I2C, 请使用 hard_oled.c 并配置 I2C1 外设。

  2. 电机编码器和按键共用 GROUP1_IRQHandler (GPIOA/GPIOB 多路中断)。
     修改 bsp_motor.c 中断处理时注意不要影响按键功能。

  3. 超声波使用 TIMG12 硬件计数器直接读值, 不占用中断。

  4. PID 参数需要根据实际小车负载在调试中整定。
     默认值: Kp=1.5, Ki=0.02, Kd=0.01 (仅参考)。
     PID 运算在 TIMG7 ZERO中断中执行, 周期=10ms。

  5. A4988 需要 12V 电源供电（可从电池直接取），信号线 3.3V/5V 兼容。

  6. 电机 PWM 频率=1kHz, 占空比范围 0~1000。
     CC=0 → 100% HIGH → 全速; CC=1000 → 0% HIGH → 停止。

================================================================================
  版本: v2.0   日期: 2026-07   LIN
================================================================================
