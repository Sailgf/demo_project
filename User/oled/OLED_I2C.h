#ifndef __OLED_I2C_H
#define	__OLED_I2C_H


#include "sys.h"
#include "stdlib.h"	


//-----------------OLED�˿ڶ���---------------- 

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_8)//SCL

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_9)//SDA
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_9)



#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����
#define u8 unsigned char
#define u32 unsigned int

void OLED_ClearPoint(u8 x,u8 y);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_WaitAck(void);
void Send_Byte(u8 dat);
void OLED_WR_Byte(u8 dat,u8 mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh_Gram(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1);
void OLED_WR_BP(u8 x,u8 y);
void OLED_Init(void);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
#endif

