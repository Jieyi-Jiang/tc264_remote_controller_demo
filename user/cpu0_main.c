/*********************************************************************************************************************
* TC264 Opensourec Library 即（TC264 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 TC264 开源库的一部分
*
* TC264 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          cpu0_main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          ADS v1.9.12
* 适用平台          TC264D
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2022-11-04       JKS            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中

// *************************** 例程硬件连接说明 ***************************
//      使用 sbus接收机 模块连接
//      模块管脚                                 单片机管脚
//      SBUS-TX             查看 zf_device_uart_receiver.h 文件中 UART_RECEVIER_RX_PIN 宏定义的引脚 默认 P10_6
//      SBUS-GND            核心板电源地 GND（需要与外部电源共地）
//      SBUS-5V             外接5V电源
//      SBUS-SBUS           悬空
//      SBUS-BAT+           电源正极 (3V3)
//      SBUS-BAT-           电源负极 (GND)

// *************************** 例程测试说明 ***************************
// 1.核心板烧录完成本例程，需要在断电情况下将调试下载器和SBUS接收机与单片机连接
//
// 2.将调试下载器连接电脑，为SBUS接收机提供5V电源完成上电(注意外部电源于核心板需要共地！！！)
//
// 3.将SBUS接收机与遥控器配对，先将长按SBUS接收机按键，等到绿灯连续闪烁，打开遥控器电源，等到接收机绿灯常亮，完成配对。（详见sbus接收机说明书）
//
// 4.电脑上使用串口助手打开对应的串口，串口波特率为 DEBUG_UART_BAUDRATE 宏定义 默认 115200，核心板按下复位按键
//
// 5.可以在串口助手上看到如下串口信息：
//                  CH1-CH6 data: ... ...
// 6.同时在遥控器上可以看见接收电压为3.3v左右，并且可以通过遥控器各个通道按键或旋钮更改通道数据，串口助手上可以看见对应的数据变化。
//
// 7.在正常输出串口数据后，将遥控器关闭可以看到如下串口信息：
//                  Remote control has been disconnected.
//
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查

#define PWM_R1              (ATOM0_CH4_P02_4)
#define PWM_R2              (ATOM0_CH5_P02_5)

#define PWM_L1              (ATOM0_CH6_P02_6)
#define PWM_L2              (ATOM0_CH7_P02_7)


#define SERVO_MOTOR_PWM             (ATOM1_CH1_P33_9)                           // 定义主板上舵机对应引脚
#define SERVO_MOTOR_FREQ            (50 )                                       // 定义主板上舵机频率  请务必注意范围 50-300
#define STEER_MID   790         //舵机中值
#define STEER_LIM   80          //舵机极限
#define Limit_Min_Max(data, min, max) (((data) > (max)) ? (max):(((data) < (min)) ? (min):(data)))


void Set_Steer(int16 duty)
{
    duty = Limit_Min_Max(duty, -STEER_LIM, STEER_LIM);
    pwm_set_duty(SERVO_MOTOR_PWM, STEER_MID+duty);
}

void set_speed(int L_motor, int R_motor)
{
    if (L_motor >= 0)
    {
        pwm_set_duty(PWM_L1, 0);                  // 计算占空比
        pwm_set_duty(PWM_L2, L_motor);                                            // 同一时间 一个电机只能输出一个 PWM 另一通道保持低电平

    }
    else
    {
        pwm_set_duty(PWM_L1, -L_motor);                  // 计算占空比
        pwm_set_duty(PWM_L2, 0);                                            // 同一时间 一个电机只能输出一个 PWM 另一通道保持低电平
    }
    if (R_motor >= 0)
    {
        pwm_set_duty(PWM_R1, 0);                  // 计算占空比
        pwm_set_duty(PWM_R2, R_motor);                                            // 同一时间 一个电机只能输出一个 PWM 另一通道保持低电平
    }
    else
    {
        pwm_set_duty(PWM_R1, -R_motor);                  // 计算占空比
        pwm_set_duty(PWM_R2, 0);                                            // 同一时间 一个电机只能输出一个 PWM 另一通道保持低电平
    }
}

// **************************** 代码区域 ***************************
int core0_main(void)
{
    clock_init();                                                    // 获取时钟频率<务必保留>
    debug_init();                                                    // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等
    pwm_init(PWM_R1, 17000, 0);                                                 // PWM 通道 R1 初始化频率 17KHz 占空比初始为 0
    pwm_init(PWM_R2, 17000, 0);                                                 // PWM 通道 R2 初始化频率 17KHz 占空比初始为 0                                               // PWM 通道 R2 初始化频率 17KHz 占空比初始为 0
    pwm_init(PWM_L1, 17000, 0);                                                 // PWM 通道 L1 初始化频率 17KHz 占空比初始为 0
    pwm_init(PWM_L2, 17000, 0);                                                 // PWM 通道 L2 初始化频率 17KHz 占空比初始为 0

    uart_receiver_init();                                                       //sbus接收机初始化

    pwm_init(SERVO_MOTOR_PWM, SERVO_MOTOR_FREQ, 0);

    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();
    set_speed(0, 0);
    system_delay_ms(2000);// 等待所有核心初始化完毕
    while (TRUE)
    {
        // 此处编写需要循环执行的代码
        if(1 == uart_receiver.finsh_flag)                            // 帧完成标志判断
        {
            if(1 == uart_receiver.state)                             // 遥控器失控状态判断
//            if(1)
            {
                printf("CH1-CH6 data: ");
                for(int i = 0; i < 6; i++)
                {
                    printf("%d ", uart_receiver.channel[i]);         // 串口输出6个通道数据
                }
                printf("\r\n");

                if (500 == uart_receiver.channel[4])
                {
                    set_speed(0, 0);
                    continue;
                }
                else if (uart_receiver.channel[4] == 1500)
                {
                    set_speed((uart_receiver.channel[1]-1000)*6 + (uart_receiver.channel[3]-1000)*3,
                            (uart_receiver.channel[1]-1000)*6 - (uart_receiver.channel[3]-1000)*3);
                }
                else
                {
                    set_speed((uart_receiver.channel[1]-1000)*6, (uart_receiver.channel[1]-1000)*6);
                }
                Set_Steer(-(uart_receiver.channel[3]-1000)/8);
            }
            else
            {
                set_speed(0, 0);
                Set_Steer(0);
                printf("Remote control has been disconnected.\r\n"); // 串口输出失控提示
            }
            uart_receiver.finsh_flag = 0;                            // 帧完成标志复位
        }
//        system_delay_ms(5);
        // 此处编写需要循环执行的代码
    }
}
#pragma section all restore
// **************************** 代码区域 **********************************

// *************************** 例程常见问题说明 ***************************
// 遇到问题时请按照以下问题检查列表检查
//
// 问题1：串口没有数据
//      查看串口助手打开的是否是正确的串口，检查打开的 COM 口是否对应的是调试下载器
//      那么检查下载器线是否松动，检查核心板串口跳线是否已经焊接，串口跳线查看核心板原理图即可找到
//
// 问题2：串口数据乱码
//      查看串口助手设置的波特率是否与程序设置一致，程序中 zf_common_debug.h 文件中 DEBUG_UART_BAUDRATE 宏定义为 debug uart 使用的串口波特率
//
// 问题3：数据不连续发送，会有卡顿
//      查看接收机灯是否常亮，若接收机灯不规律闪，说明没有对接收机进行5v供电，信号微弱，需要为其提供稳定的5V电源
//
// 问题4：遥控器无法控制数据改变，或者没有数据
//      查看接收机灯是否常亮，若接收机灯不常亮，说明没有对接收机与遥控器没有配对，配对详见sbus接收机说明书
