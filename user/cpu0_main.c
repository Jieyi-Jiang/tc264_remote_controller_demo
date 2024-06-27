/*********************************************************************************************************************
* TC264 Opensourec Library ����TC264 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC264 ��Դ���һ����
*
* TC264 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          cpu0_main
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.9.12
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-11-04       JKS            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU0��RAM��

// *************************** ����Ӳ������˵�� ***************************
//      ʹ�� sbus���ջ� ģ������
//      ģ��ܽ�                                 ��Ƭ���ܽ�
//      SBUS-TX             �鿴 zf_device_uart_receiver.h �ļ��� UART_RECEVIER_RX_PIN �궨������� Ĭ�� P10_6
//      SBUS-GND            ���İ��Դ�� GND����Ҫ���ⲿ��Դ���أ�
//      SBUS-5V             ���5V��Դ
//      SBUS-SBUS           ����
//      SBUS-BAT+           ��Դ���� (3V3)
//      SBUS-BAT-           ��Դ���� (GND)

// *************************** ���̲���˵�� ***************************
// 1.���İ���¼��ɱ����̣���Ҫ�ڶϵ�����½�������������SBUS���ջ��뵥Ƭ������
//
// 2.���������������ӵ��ԣ�ΪSBUS���ջ��ṩ5V��Դ����ϵ�(ע���ⲿ��Դ�ں��İ���Ҫ���أ�����)
//
// 3.��SBUS���ջ���ң������ԣ��Ƚ�����SBUS���ջ��������ȵ��̵�������˸����ң������Դ���ȵ����ջ��̵Ƴ����������ԡ������sbus���ջ�˵���飩
//
// 4.������ʹ�ô������ִ򿪶�Ӧ�Ĵ��ڣ����ڲ�����Ϊ DEBUG_UART_BAUDRATE �궨�� Ĭ�� 115200�����İ尴�¸�λ����
//
// 5.�����ڴ��������Ͽ������´�����Ϣ��
//                  CH1-CH6 data: ... ...
// 6.ͬʱ��ң�����Ͽ��Կ������յ�ѹΪ3.3v���ң����ҿ���ͨ��ң��������ͨ����������ť����ͨ�����ݣ����������Ͽ��Կ�����Ӧ�����ݱ仯��
//
// 7.����������������ݺ󣬽�ң�����رտ��Կ������´�����Ϣ��
//                  Remote control has been disconnected.
//
// �������������˵�����ز��� ����ձ��ļ����·� ���̳�������˵�� �����Ų�

#define PWM_R1              (ATOM0_CH4_P02_4)
#define PWM_R2              (ATOM0_CH5_P02_5)

#define PWM_L1              (ATOM0_CH6_P02_6)
#define PWM_L2              (ATOM0_CH7_P02_7)


#define SERVO_MOTOR_PWM             (ATOM1_CH1_P33_9)                           // ���������϶����Ӧ����
#define SERVO_MOTOR_FREQ            (50 )                                       // ���������϶��Ƶ��  �����ע�ⷶΧ 50-300
#define STEER_MID   790         //�����ֵ
#define STEER_LIM   80          //�������
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
        pwm_set_duty(PWM_L1, 0);                  // ����ռ�ձ�
        pwm_set_duty(PWM_L2, L_motor);                                            // ͬһʱ�� һ�����ֻ�����һ�� PWM ��һͨ�����ֵ͵�ƽ

    }
    else
    {
        pwm_set_duty(PWM_L1, -L_motor);                  // ����ռ�ձ�
        pwm_set_duty(PWM_L2, 0);                                            // ͬһʱ�� һ�����ֻ�����һ�� PWM ��һͨ�����ֵ͵�ƽ
    }
    if (R_motor >= 0)
    {
        pwm_set_duty(PWM_R1, 0);                  // ����ռ�ձ�
        pwm_set_duty(PWM_R2, R_motor);                                            // ͬһʱ�� һ�����ֻ�����һ�� PWM ��һͨ�����ֵ͵�ƽ
    }
    else
    {
        pwm_set_duty(PWM_R1, -R_motor);                  // ����ռ�ձ�
        pwm_set_duty(PWM_R2, 0);                                            // ͬһʱ�� һ�����ֻ�����һ�� PWM ��һͨ�����ֵ͵�ƽ
    }
}

// **************************** �������� ***************************
int core0_main(void)
{
    clock_init();                                                    // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                                                    // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������
    pwm_init(PWM_R1, 17000, 0);                                                 // PWM ͨ�� R1 ��ʼ��Ƶ�� 17KHz ռ�ձȳ�ʼΪ 0
    pwm_init(PWM_R2, 17000, 0);                                                 // PWM ͨ�� R2 ��ʼ��Ƶ�� 17KHz ռ�ձȳ�ʼΪ 0                                               // PWM ͨ�� R2 ��ʼ��Ƶ�� 17KHz ռ�ձȳ�ʼΪ 0
    pwm_init(PWM_L1, 17000, 0);                                                 // PWM ͨ�� L1 ��ʼ��Ƶ�� 17KHz ռ�ձȳ�ʼΪ 0
    pwm_init(PWM_L2, 17000, 0);                                                 // PWM ͨ�� L2 ��ʼ��Ƶ�� 17KHz ռ�ձȳ�ʼΪ 0

    uart_receiver_init();                                                       //sbus���ջ���ʼ��

    pwm_init(SERVO_MOTOR_PWM, SERVO_MOTOR_FREQ, 0);

    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready();
    set_speed(0, 0);
    system_delay_ms(2000);// �ȴ����к��ĳ�ʼ�����
    while (TRUE)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���
        if(1 == uart_receiver.finsh_flag)                            // ֡��ɱ�־�ж�
        {
            if(1 == uart_receiver.state)                             // ң����ʧ��״̬�ж�
//            if(1)
            {
                printf("CH1-CH6 data: ");
                for(int i = 0; i < 6; i++)
                {
                    printf("%d ", uart_receiver.channel[i]);         // �������6��ͨ������
                }
                printf("\r\n");
            }
            else
            {
                printf("Remote control has been disconnected.\r\n"); // �������ʧ����ʾ
            }
            uart_receiver.finsh_flag = 0;                            // ֡��ɱ�־��λ
        }
        if (1 == uart_receiver.state)
        {
            set_speed(0, 0);
            Set_Steer(0);
        }
        if (500 == uart_receiver.channel[4])
        {
            set_speed(0, 0);
            continue;
        }
        else if (uart_receiver.channel[4] == 1500)
        {
            set_speed((uart_receiver.channel[1]-1000)*6 + (uart_receiver.channel[3]-1000)*5,
                    (uart_receiver.channel[1]-1000)*6 - (uart_receiver.channel[3]-1000)*5);
        }
        else
        {
            set_speed((uart_receiver.channel[1]-1000)*6, (uart_receiver.channel[1]-1000)*6);
        }
//        set_speed(1000, 0);
        Set_Steer(-(uart_receiver.channel[0]-1000)/9);
        system_delay_ms(10);
        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}
#pragma section all restore
// **************************** �������� **********************************

// *************************** ���̳�������˵�� ***************************
// ��������ʱ�밴�������������б���
//
// ����1������û������
//      �鿴�������ִ򿪵��Ƿ�����ȷ�Ĵ��ڣ����򿪵� COM ���Ƿ��Ӧ���ǵ���������
//      ��ô������������Ƿ��ɶ��������İ崮�������Ƿ��Ѿ����ӣ��������߲鿴���İ�ԭ��ͼ�����ҵ�
//
// ����2��������������
//      �鿴�����������õĲ������Ƿ����������һ�£������� zf_common_debug.h �ļ��� DEBUG_UART_BAUDRATE �궨��Ϊ debug uart ʹ�õĴ��ڲ�����
//
// ����3�����ݲ��������ͣ����п���
//      �鿴���ջ����Ƿ����������ջ��Ʋ���������˵��û�жԽ��ջ�����5v���磬�ź�΢������ҪΪ���ṩ�ȶ���5V��Դ
//
// ����4��ң�����޷��������ݸı䣬����û������
//      �鿴���ջ����Ƿ����������ջ��Ʋ�������˵��û�жԽ��ջ���ң����û����ԣ�������sbus���ջ�˵����
