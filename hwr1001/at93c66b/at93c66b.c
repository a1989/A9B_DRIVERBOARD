#include "at93c66b.h"
#include "includes.h"

#define OP_CODE_READ  0x6
#define OP_CODE_WRITE  0x5
#define OP_CODE_ERASE  0x3
#define OP_CODE_ERASE_ALL  0x0
#define OP_CODE_EWEN	0x13
#define ADDR_BIT_LEN	9
#define OP_CODE_WRALL		0x11
char arrMsg[64];

#define SPI_93C66_CS_HIGH()   HAL_GPIO_WritePin (SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET)
#define SPI_93C66_CS_LOW()    HAL_GPIO_WritePin (SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET)

#define CS0  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET)//GPIOA->BSRRH=GPIO_Pin_4
#define CS1  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET)//GPIOA->BSRRL=GPIO_Pin_4
#define SK0  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CK_Pin, GPIO_PIN_RESET)//GPIOA->BSRRH=GPIO_Pin_5
#define SK1  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CK_Pin, GPIO_PIN_SET)//GPIOA->BSRRL=GPIO_Pin_5
#define DI (SPI1_CS_GPIO_Port->IDR&SPI1_DI_Pin)  
#define DO0  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_DO_Pin, GPIO_PIN_RESET)//GPIOA->BSRRH=GPIO_Pin_7
#define DO1  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_DO_Pin, GPIO_PIN_SET)//GPIOA->BSRRL=GPIO_Pin_7

#define READ_D  0xc000      //???
#define WRITE_D 0xa000      //???
#define ERASE_D 0xe000      //????
#define EWEN_D  0x9800      //?/?????
#define EW_D    0x8000      //?/?????

void at93c46_clock(void)
{
  SK0;
  Delay_ms(10);
  SK1;
  Delay_ms(10);
}

//????;in_data???,d_bit?????????
void send(u16 in_data,u16 d_bit)
{
u8 i;
for(i=0;i<d_bit;i++)
   {
     if(in_data&0x8000)
       {
         DO1;
       }
     else
       {
         DO0;
       }
  in_data<<=1;
     at93c46_clock();
   }
}
//????
u16 receive(void)
{
u16 in_data=0;
u8 j;
	while(HAL_GPIO_ReadPin(SPI1_CS_GPIO_Port, SPI1_DI_Pin) == 1)
//while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)==1)
      at93c46_clock();
  for(j=0;j<16;j++)
     {
      at93c46_clock();
   in_data<<=1;
   if(DI)
     {
        in_data|=0x01;
     }
     }
  at93c46_clock();
  return(in_data);
}

//????????,?AT93C66??????????
u16 EEread9366(u16 addr)
  {
   
    u16 data_r;
		CS1;
    send(READ_D,3);             //????
    send(addr,8);               //???
    data_r=receive();            //????
    CS0;
    return(data_r);
  }
//??????
void erase(u16 addr)
{

  Delay_ms(100);
  CS1;
  Delay_ms(100);
  send(ERASE_D,3);
  send(addr,8);
  CS0;
  Delay_ms(200);
  CS1;
	while(HAL_GPIO_ReadPin(SPI1_CS_GPIO_Port, SPI1_DI_Pin) == 1);
//while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)==1);
  Delay_ms(100);
  CS0;
  Delay_ms(100);
}

//?????????AT93C66???????
void EEwrite9366(u16 addr,u16 eData)
  {
     CS1;                   //?/???
  Delay_ms(10);
     send(EWEN_D,16);
  send(0x01,2);
     CS0;
     Delay_ms(10);  
     CS1;
   
     send(WRITE_D,3);       //????
     send(addr,8);          //???
     send(eData,16);
     CS0;
  at93c46_clock();
     CS1;
  at93c46_clock();
		while(HAL_GPIO_ReadPin(SPI1_CS_GPIO_Port, SPI1_DI_Pin) == 1)
//		{
//			printf("read1");
//		}
//    while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)==1) 
			at93c46_clock();     //????
     
     CS0;
  }

//static void ClearMsgBuffer(void)
//{
//    int i;
//    for(i = 0; i < sizeof(arrMsg); i++)
//    {
//        arrMsg[i] = 0;
//    }
//}

//static bool At93c66b_WriteEnableAddr8 (char *strMsg)
//{
//		uint16_t iCmd = OP_CODE_EWEN << 7;
//	  HAL_StatusTypeDef iStatus;
//		char arrMsgTemp[64];
//    uint8_t arrCmd[2];
//  
//    if(NULL == strMsg)
//    {
//        strcpy(strMsg, "At93c66b_WriteEnableAddr8 strMsg Params Error");
//        return false;
//    }
//    
//		arrCmd[0] = (iCmd >> 8) & 0xFF;
//		arrCmd[1] = iCmd & 0xFF;
//		

//		
//    iStatus = HAL_SPI_Transmit(&EEPROM_SPI, arrCmd, 2, 0xFFFF);    
//    if(iStatus != HAL_OK)
//    {
//        sprintf(arrMsgTemp, "At93c66b_WriteEnableAddr8 Write cmd 0x%x Failed, Code:0x%x", iCmd, iStatus);
//        strcpy(strMsg, arrMsgTemp);
//        return false;
//    }    
//		
//		
//		
//		return true;
//}

//static bool At93c66b_WriteEnableAddr16 (char *strMsg)
//{
//		uint8_t iCmd = OP_CODE_EWEN << 3;
//	  HAL_StatusTypeDef iStatus;
//		char arrMsgTemp[64];
//    uint8_t arrCmd[2];
//  
//    if(NULL == strMsg)
//    {
//        strcpy(strMsg, "At93c66b_WriteEnableAddr8 strMsg Params Error");
//        return false;
//    }
//    
////		arrCmd[0] = (iCmd >> 8) & 0xFF;
////		arrCmd[1] = iCmd & 0xFF;
//		printf("\r\n At93c66b_WriteEnableAddr16 Write cmd:0x%x", iCmd);
//		SPI_93C66_CS_HIGH();
//		
//    iStatus = HAL_SPI_Transmit(&EEPROM_SPI, &iCmd, 1, 0xFFFF);    
//    if(iStatus != HAL_OK)
//    {
//        sprintf(arrMsgTemp, "At93c66b_WriteEnableAddr16 Write cmd 0x%x Failed, Code:0x%x", iCmd, iStatus);
//        strcpy(strMsg, arrMsgTemp);
//        return false;
//    }    
//		
//		SPI_93C66_CS_LOW();
//		
//		return true;
//}

//bool At93c66b_WriteByte (uint16_t iAddress, uint8_t iData, char *strMsg)
//{   
////    uint8_t iOP = OP_CODE_WRITE;
//    HAL_StatusTypeDef iStatus;
//    char arrMsgTemp[64];
//    uint8_t arrCmd[2]; 
//		
//    if(NULL == strMsg)
//    {
//        strcpy(strMsg, "At93c66b_WriteByte strMsg Params Error");
//        return false;
//    }
//    
//		SPI_93C66_CS_HIGH();
//		
//		if(!At93c66b_WriteEnableAddr8(arrMsgTemp))
//		{
//				strcpy(strMsg, arrMsgTemp);
//        return false;
//		}
//		
//		uint16_t iCmd = (OP_CODE_WRITE << 9) | iAddress;
//		arrCmd[0] = (iCmd >> 8) & 0xFF;
//		arrCmd[1] = iCmd & 0xFF;
//		printf("\r\n At93c66b_WriteByte Write cmd:0x%x", iCmd);
//    iStatus = HAL_SPI_Transmit(&EEPROM_SPI, arrCmd, 2, 0xFFFF);    
//    if(iStatus != HAL_OK)
//    {
//        sprintf(arrMsgTemp, "At93c66b_WriteByte Write Addr 0x%x Failed, Code:0x%x", iAddress, iStatus);
//        strcpy(strMsg, arrMsgTemp);
//        return false;
//    }    
//    
//		iStatus = HAL_SPI_Transmit(&EEPROM_SPI, &iData, 1, 0xFFFF); 
//    if(iStatus != HAL_OK)
//    {
//        sprintf(arrMsgTemp, "At93c66b_WriteByte Write data 0x%x Failed, Code:0x%x", iData, iStatus);
//        strcpy(strMsg, arrMsgTemp);
//        return false;
//    }
//    
//    return true;
//}

//bool At93c66b_ReadByte (uint16_t iAddress, uint8_t *iData, char *strMsg)
//{
//    uint8_t iOp = OP_CODE_READ;
//    HAL_StatusTypeDef iStatus;
//    char arrMsgTemp[64];
//		uint8_t arrCmd[2];
//    
//    if(NULL == strMsg)
//    {
//				sprintf(arrMsgTemp, "At93c66b_ReadByte strMsg Params Error");
//				strcpy(strMsg, arrMsgTemp);
//        return false;
//    }
//    
//		SPI_93C66_CS_HIGH();
////		arrCmd[0] = (iCmd >> 8) & 0xFF;
////		arrCmd[1] = iCmd & 0xFF;
////    iStatus = HAL_SPI_Transmit(&EEPROM_SPI, &iCmd, 1, 0xFFFF);
////    if(iStatus != HAL_OK)
////    {
////        sprintf(arrMsgTemp, "At93c66b_ReadByte Write cmd 0x%x Failed, Code:0x%x", iCmd, iStatus);
////        strcpy(strMsg, arrMsgTemp);
////        return false;
////    }    
//    
////		iAddress = iAddress << 7;
////		arrCmd[0] = (iAddress >> 8) & 0xFF;
////		arrCmd[1] = iAddress & 0xFF;
//		uint16_t iCmd = (OP_CODE_READ << 9) | iAddress;
//		arrCmd[0] = (iCmd >> 8) & 0xFF;
//		arrCmd[1] = iCmd & 0xFF;
//		printf("\r\n At93c66b_ReadByte Write cmd:0x%x", iCmd);
//    iStatus = HAL_SPI_Transmit(&EEPROM_SPI, arrCmd, 2, 0xFFFF);    
//    if(iStatus != HAL_OK)
//    {
//        sprintf(arrMsgTemp, "At93c66b_ReadByte Write Addr 0x%x Failed, Code:0x%x", iAddress, iStatus);
//        strcpy(strMsg, arrMsgTemp);
//        return false;
//    }    
//		
//		HAL_SPI_Receive(&EEPROM_SPI, iData, 1, 0xFFFF);
//    iStatus = HAL_SPI_Receive(&EEPROM_SPI, iData, 1, 0xFFFF);
//    if(iStatus != HAL_OK)
//    {
//        sprintf(arrMsgTemp, "At93c66b_ReadByte Read data Failed, Code:0x%x", iStatus);
//        strcpy(strMsg, arrMsgTemp);
//        return false;
//    }
////		uint8_t arrRecv[2];
////		HAL_SPI_TransmitReceive(&EEPROM_SPI, arrCmd, arrRecv, 2, 0xFFFF);  
////    iStatus = HAL_SPI_TransmitReceive(&EEPROM_SPI, arrCmd, arrRecv, 2, 0xFFFF);    
////    if(iStatus != HAL_OK)
////    {
////        sprintf(arrMsgTemp, "At93c66b_ReadByte Write Addr 0x%x Failed, Code:0x%x", iAddress, iStatus);
////        strcpy(strMsg, arrMsgTemp);
////        return false;
////    }    
//		
////		printf("\r\n 0x%x,0x%x", arrRecv[0], arrRecv[1]);
//		
//    return true;
//}

//bool At93c66b_WriteWord (uint8_t iAddress, uint16_t iData, char *strMsg)
//{
//    HAL_StatusTypeDef iStatus;
//    char arrMsgTemp[64];
//    uint16_t iCmd = ((OP_CODE_WRITE << 6) | (iAddress)) << 7;
//    uint8_t arrData[2];
//    arrData[0] = (iData >> 8) & 0xFF;
//    arrData[1] = (iData) & 0xFF;
//    
//    if(NULL == strMsg)
//    {
//        strcpy(strMsg, "At93c66b_ReadByte strMsg Params Error");
//        return false;
//    }
//    
//		if(!At93c66b_WriteEnableAddr16(arrMsgTemp))
//		{
//				strcpy(strMsg, arrMsgTemp);
//        return false;
//		}
//		
//		arrData[0] = (iCmd >> 8) & 0xFF;
//		arrData[1] = (iCmd) & 0xFF;
//		
//		printf("\r\n At93c66b_WriteWord cmd 0x%x", iCmd);
//		SPI_93C66_CS_LOW();
//		SPI_93C66_CS_HIGH();
//		
//    iStatus = HAL_SPI_Transmit(&EEPROM_SPI, arrData, 2, 0xFFFF);
//    if(iStatus != HAL_OK)
//    {
//        sprintf(arrMsgTemp, "At93c66b_WriteWord Write cmd 0x%x Failed, Code:0x%x", iCmd, iStatus);
//        strcpy(strMsg, arrMsgTemp);
//        return false;
//    }    
//    
//    iStatus = HAL_SPI_Transmit(&EEPROM_SPI, arrData, 2, 0xFFFF);
//    if(iStatus != HAL_OK)
//    {
//        sprintf(arrMsgTemp, "At93c66b_WriteWord Write data 0x%x%x Failed, Code:0x%x", arrData[0], arrData[1], iStatus);
//        strcpy(strMsg, arrMsgTemp);
//        return false;
//    }
//		
//		SPI_93C66_CS_LOW();
//    
//    return true;
//}

//bool At93c66b_ReadWord (uint8_t iAddress, uint8_t *iData, char *strMsg)
//{
//    HAL_StatusTypeDef iStatus;
//    char arrMsgTemp[64];
//    uint16_t iCmd = ((OP_CODE_READ << 6) | (iAddress)) << 7;
//    uint8_t arrData[2];
//    
//    if(NULL == strMsg)
//    {
//        strcpy(strMsg, "At93c66b_WriteByte strMsg Params Error");
//        return false;
//    }
//    
//		arrData[0] = (iCmd >> 8) & 0xFF;
//		arrData[1] = (iCmd) & 0xFF;
//		
//		printf("\r\n At93c66b_ReadWord cmd 0x%x", iCmd);
//		SPI_93C66_CS_LOW();
//		SPI_93C66_CS_HIGH();
//				
//    iStatus = HAL_SPI_Transmit(&EEPROM_SPI, arrData, 2, 0xFFFF);
//    if(iStatus != HAL_OK)
//    {
//        sprintf(arrMsgTemp, "At93c66b_ReadWord Write cmd 0x%x Failed, Code:0x%x", iCmd, iStatus);
//        strcpy(strMsg, arrMsgTemp);
//        return false;
//    }    
//    Delay_ms(1);
////		iStatus = HAL_SPI_Receive(&EEPROM_SPI, arrData, 2, 0xFFFF);  
//    iStatus = HAL_SPI_Receive(&EEPROM_SPI, arrData, 2, 0xFFFF);    
//    if(iStatus != HAL_OK)
//    {
//        sprintf(arrMsgTemp, "At93c66b_ReadWord Read data Failed, Code:0x%x", iStatus);
//        strcpy(strMsg, arrMsgTemp);
//        return false;
//    }
//    
//    *iData = (arrData[0] << 8) | arrData[1];
//    
//		SPI_93C66_CS_LOW();
//		
//		printf("\r\n 0x%x,0x%x", arrData[0], arrData[1]);
//		
//    return true;
//}

//bool At93c66b_EraseAll(char *strMsg)
//{
//    uint8_t iCmd = (OP_CODE_ERASE_ALL << 6) | (0x20);
//    HAL_StatusTypeDef iStatus;
//    char arrMsgTemp[64];
//    
//    iStatus = HAL_SPI_Transmit(&EEPROM_SPI, &iCmd, 1, 0xFFFF);
//    if(iStatus != HAL_OK)
//    {
//        sprintf(arrMsgTemp, "At93c66b_EraseAll Write cmd 0x%x Failed, Code:0x%x", iCmd, iStatus);
//        strcpy(strMsg, arrMsgTemp);
//        return false;
//    }    
//    
//    return true;
//}

//bool At93c66b_Erase(uint8_t iAddress, char *strMsg)
//{
//    uint8_t iCmd = (OP_CODE_ERASE << 6) | (iAddress);  
//    HAL_StatusTypeDef iStatus;
//    char arrMsgTemp[64];
//    
//    iStatus = HAL_SPI_Transmit(&EEPROM_SPI, &iCmd, 1, 0xFFFF);
//    if(iStatus != HAL_OK)
//    {
//        sprintf(arrMsgTemp, "At93c66b_Erase Write cmd 0x%x Failed, Code:0x%x", iCmd, iStatus);
//        strcpy(strMsg, arrMsgTemp);
//        return false;
//    }    
//    
//    return true;
//}

//bool At93c66b_WriteAll (uint8_t iData, char *strMsg)
//{
//		//    uint8_t iOP = OP_CODE_WRITE;
//    HAL_StatusTypeDef iStatus;
//    char arrMsgTemp[64];
//    uint8_t arrCmd[2]; 
//		uint16_t i = 0xfff;
//    if(NULL == strMsg)
//    {
//        strcpy(strMsg, "At93c66b_WriteByte strMsg Params Error");
//        return false;
//    }
//    
//		if(!At93c66b_WriteEnableAddr8(arrMsgTemp))
//		{
//				strcpy(strMsg, arrMsgTemp);
//        return false;
//		}
//		
//		uint16_t iCmd = (OP_CODE_WRALL << 7);
//		arrCmd[0] = (iCmd >> 8) & 0xFF;
//		arrCmd[1] = iCmd & 0xFF;
////		arrCmd[2] = iData;
//		printf("\r\n At93c66b_WriteAll Write cmd:0x%x", iCmd);
//    iStatus = HAL_SPI_Transmit(&EEPROM_SPI, arrCmd, 3, 0xFFFF);    
//    if(iStatus != HAL_OK)
//    {
//        sprintf(arrMsgTemp, "At93c66b_WriteByte Write cmd 0x%x Failed, Code:0x%x", iCmd, iStatus);
//        strcpy(strMsg, arrMsgTemp);
//        return false;
//    }    
//    
//		//Delay_ms(100);
//		//while(i--);
//		
//		iStatus = HAL_SPI_Transmit(&EEPROM_SPI, &iData, 1, 0xFFFF); 
//    if(iStatus != HAL_OK)
//    {
//        sprintf(arrMsgTemp, "At93c66b_WriteAll Write data 0x%x Failed, Code:0x%x", iData, iStatus);
//        strcpy(strMsg, arrMsgTemp);
//        return false;
//    }
//    
//		//Delay_ms(100);
//		
//    return true;
//}

//static void RegWrite(uint8_t iData)
//{
//		uint8_t i;
//		for(i = 0; i < 8; i++)
//		{
//				GPIOA->BRR = GPIO_BRR_BR9;
//		}
//}
