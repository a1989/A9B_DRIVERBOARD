#include "includes.h"
#include "led.h"
#include "TMC2590.h"

extern EncoderType GetEncoder;

void LED_Running(void)
{
//			SPI_TMC2590_SendByte (0xef020, &iRead);
//		iStatus = SPI_TMC2590_SendByte (0xef020, &iRead);
//		iStatus = SPI_TMC2590_SendByte (0xef020, &iRead);
	BSP_Initializes();//初始化。
	if (led_display_cnt_flag == 1) //1秒一次
	{	
//		if (driver_error_flag == 1)	//过流
//		{
//			LEDR_TOGGLE;
//			LEDG(OFF);
//			LEDB(OFF);
//		}
//		else if (driver_error_flag ==2)
//		{
//			LEDB_TOGGLE;
//			LEDG(OFF);
//			LEDR(OFF);
//		}
//		else if ((bsp_init_flag == 1) && (driver_board_enable_flag == 1) && (driver_error_flag == 0)) //1秒闪一次
//		{
//			LEDR(OFF);
//			LEDB(OFF);
//			LED_RUN_TOGGLE;
//		}
//		Get_Drv8711_Statu();
//		Ina201_ADC_Get();//获取ADC的值
//		led_display_cnt_flag = 0;
//                printf ("\r\n id:%d", driver_can_stdid);
		//printf ("\r\n soft_reset_flag:%d", soft_reset_flag);
                //printf("\r\nMotor:%d,%d,%d,%d,%d", Aim_Location, Location_Cnt, motor_speed, GetEncoder.V3, Toggle_Pulse);
	}
//	Iwdg_Updata();
	System_Control();
//	TMC2590_TestStandalone();
//	Motor_Location_Write(); //写电机位置 zyg2019.11.8
	Limit_Switch_Scanning();//按键扫描
//	Signal_LED_Control(); //状态灯控制
}

// 状态灯控制
void Signal_LED_Control (void)
{
    if (driver_over_current_flag == 1)
	{
		driver_error_flag = 1;
	}
	else if ((can_Receive_Right_flag == 0) || (eep_i2c_status == 1) || (TMC2590_error_status == 1))
//    else if ((can_Receive_Right_flag == 0) || (eep_i2c_status == 1) || (drv8711_error_status == 1))
	{
		driver_error_flag = 2;//通信异常
    }
    else
    {
    	driver_error_flag = 0;
    }
}
