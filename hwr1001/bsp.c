#include "bsp.h"
#include "includes.h"

void BSP_Initializes (void)
{
	if (bsp_init_flag == 0)
	{
		printf ("\r\n driver board start init");
		At93c66b_Init();	//初始化EEPROM
//		Driver_Board_Parameter_Init();//参数读取，必须放在最前面
		
		Driver_Can_Init();//CAN滤波器初始化
		//Drv8711_Init(); //DRV8711参数写入，默认参数
    if(!TMC2590_Init())
		{
				return;
		}
		PID_Init();//电机PID参数初始化
		//停止PWM比较输出
//		HAL_TIM_OC_Stop_IT (&htim2, TIM_CHANNEL_1);
		HAL_TIM_OC_Stop_IT (&htim4, TIM_CHANNEL_1);
//		HAL_TIM_OC_Start_IT (&htim4, TIM_CHANNEL_1 );
//		//HAL_TIM_OC_Start_IT ( &htim2,TIM_CHANNEL_1 );
		//开编码器
//		TIM3_Encoder_Switch (1);
		TIM2_Encoder_Switch (1);
		//开启定时器中断用于执行事件
//		TIM4_IT_Interrupt_Switch (1);
		TIM3_IT_Interrupt_Switch (1);
		//开启定时器1中断
		TIM1_IT_Interrupt_Switch (1);
		HAL_CAN_Receive_IT (&hcan, CAN_FIFO0); //开CAN接收中断

//		HAL_ADCEx_Calibration_Start (&hadc1);
//		HAL_ADC_Start_DMA (&hadc1, ADC_Get_Info, 3);
		Uart_Receive_Interrupt_Switch (&huart1, &uart1_receive_byte);//开中断
		bsp_init_flag = 1;
		printf ("\r\n hwrobot steper motor driver board bsp init is ok !");
	}
//	volatile int i = 0xf;
//	HAL_GPIO_WritePin(SIGNAL_LED_G_GPIO_Port, SIGNAL_LED_G_Pin,GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(SIGNAL_LED_B_GPIO_Port, SIGNAL_LED_B_Pin,GPIO_PIN_RESET);
//	HAL_GPIO_TogglePin (SIGNAL_LED_R_GPIO_Port, SIGNAL_LED_R_Pin);
//	while(i--);
}
