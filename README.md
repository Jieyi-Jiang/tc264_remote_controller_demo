# tc264_remote_controller_demo

#### 说明
智能车TC264遥控器例程，遥控器接收机使用SBUS协议。<br>
基于逐飞TC264开源库构建，修改了 zf_device_uart_receiver.c 库文件的部分代码以适配现有的遥控器接收机。<br>
注意遥控器接收机的差异，以及引脚使用的不同，否则直接导入程序可能会出错。<br>
如果遥控器接收机输出的信号是负逻辑电平，需要在遥控器接收机和 MCU 之间接一个非门用于反相。<br>

#### 引脚定义
| 功能  | 宏定义| 引脚  |
|---|---|---|
|右电机PWM1| PWM_R1  |  ATOM0_CH4_P02_4|
|右电机PWM2| PWM_R2  | ATOM0_CH5_P02_5  |
|左电机PWM1| PWM_L1  | ATOM0_CH6_P02_6  |
|左电机PWM2| PWM_L2  |  ATOM0_CH7_P02_7 |
|舵机PWM|  SERVO_MOTOR_PWM | ATOM1_CH1_P33_9  |
|串口接收引脚<br>**连接遥控器接收机信号引脚**| UART_RECEVIER_RX_PIN|  UART2_RX_P10_6 |


