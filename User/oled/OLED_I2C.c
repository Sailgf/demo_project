/************************************************************************************
 *  Copyright (c), 2014, HelTec Automatic Technology co.,LTD.
 *            All rights reserved.
 *
 * Http:    www.heltec.cn
 * Email:   cn.heltec@gmail.com
 * WebShop: heltec.taobao.com
 *
 * File name: OLED_I2C.c
 * Project  : HelTec.uvprij
 * Processor: STM32F103C8T6
 * Compiler : MDK fo ARM
 *
 * Author : 小林
 * Version: 1.00
 * Date   : 2014.4.8
 * Email  : hello14blog@gmail.com
 * Modification: none
 *
 * Description:128*64点阵的OLED显示屏驱动文件，仅适用于惠特自动化(heltec.taobao.com)的SD1306驱动IIC通信方式显示屏
 *
 * Others: none;
 *
 * Function List:
 *	1. void I2C_Configuration(void) -- 配置CPU的硬件I2C
 * 2. void I2C_WriteByte(uint8_t addr,uint8_t data) --
 *向寄存器地址写一个byte的数据
 * 3. void WriteCmd(unsigned char I2C_Command) -- 写命令
 * 4. void WriteDat(unsigned char I2C_Data) -- 写数据
 * 5. void OLED_Init(void) -- OLED屏初始化
 * 6. void OLED_SetPos(unsigned char x, unsigned char y) -- 设置起始点坐标
 * 7. void OLED_Fill(unsigned char fill_Data) -- 全屏填充
 * 8. void OLED_CLS(void) -- 清屏
 * 9. void OLED_ON(void) -- 唤醒
 * 10. void OLED_OFF(void) -- 睡眠
 * 11. void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[],
 *unsigned char TextSize) -- 显示字符串(字体大小有6*8和8*16两种)
 * 12. void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N) --
 *显示中文(中文需要先取模，然后放到codetab.h中)
 * 13. void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char
 *x1,unsigned char y1,unsigned char BMP[]) -- BMP图片
 *
 * History: none;
 *
 *************************************************************************************/

#include "OLED_I2C.h"

#include "codetab.h"
#include "delay.h"
#include "stdlib.h"

u8 OLED_GRAM[128][8];

// 反显函数
void OLED_ColorTurn(u8 i) {
  if (i == 0) {
    OLED_WR_Byte(0xA6, OLED_CMD);  // 正常显示
  }
  if (i == 1) {
    OLED_WR_Byte(0xA7, OLED_CMD);  // 反色显示
  }
}

// 屏幕旋转180度
void OLED_DisplayTurn(u8 i) {
  if (i == 0) {
    OLED_WR_Byte(0xC8, OLED_CMD);  // 正常显示
    OLED_WR_Byte(0xA1, OLED_CMD);
  }
  if (i == 1) {
    OLED_WR_Byte(0xC0, OLED_CMD);  // 反转显示
    OLED_WR_Byte(0xA0, OLED_CMD);
  }
}

// 起始信号
void I2C_Start(void) {
  OLED_SDIN_Set();
  OLED_SCLK_Set();
  OLED_SDIN_Clr();
  OLED_SCLK_Clr();
}

// 结束信号
void I2C_Stop(void) {
  OLED_SCLK_Set();
  OLED_SDIN_Clr();
  OLED_SDIN_Set();
}

// 等待信号响应
void I2C_WaitAck(void)  // 测数据信号的电平
{
  OLED_SCLK_Set();
  OLED_SCLK_Clr();
}

// 写入一个字节
void Send_Byte(u8 dat) {
  u8 i;
  for (i = 0; i < 8; i++) {
    OLED_SCLK_Clr();  // 将时钟信号设置为低电平
    if (dat & 0x80)   // 将dat的8位从最高位依次写入
    {
      OLED_SDIN_Set();
    } else {
      OLED_SDIN_Clr();
    }
    OLED_SCLK_Set();  // 将时钟信号设置为高电平
    OLED_SCLK_Clr();  // 将时钟信号设置为低电平
    dat <<= 1;
  }
}

// 发送一个字节
// 向SSD1306写入一个字节。
// mode:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat, u8 mode) {
  I2C_Start();
  Send_Byte(0x78);
  I2C_WaitAck();
  if (mode) {
    Send_Byte(0x40);
  } else {
    Send_Byte(0x00);
  }
  I2C_WaitAck();
  Send_Byte(dat);
  I2C_WaitAck();
  I2C_Stop();
}

// 开启OLED显示
void OLED_DisPlay_On(void) {
  OLED_WR_Byte(0x8D, OLED_CMD);  // 电荷泵使能
  OLED_WR_Byte(0x14, OLED_CMD);  // 开启电荷泵
  OLED_WR_Byte(0xAF, OLED_CMD);  // 点亮屏幕
}

// 关闭OLED显示
void OLED_DisPlay_Off(void) {
  OLED_WR_Byte(0x8D, OLED_CMD);  // 电荷泵使能
  OLED_WR_Byte(0x10, OLED_CMD);  // 关闭电荷泵
  OLED_WR_Byte(0xAF, OLED_CMD);  // 关闭屏幕
}

// 更新显存到OLCD
void OLED_Refresh_Gram(void) {
  u8 i, n;
  for (i = 0; i < 8; i++) {
    OLED_WR_Byte(0xb0 + i, OLED_CMD);  // 设置页地址（0~7）
    OLED_WR_Byte(0x00, OLED_CMD);      // 设置显示位置—列低地址
    OLED_WR_Byte(0x10, OLED_CMD);      // 设置显示位置—列高地址
    for (n = 0; n < 128; n++) OLED_WR_Byte(OLED_GRAM[n][i], OLED_DATA);
  }
}
// 清屏函数
void OLED_Clear(void) {
  u8 i, n;
  for (i = 0; i < 8; i++)
    for (n = 0; n < 128; n++) OLED_GRAM[n][i] = 0X00;
  OLED_Refresh_Gram();  // 更新显示
}
// 画点
// x:0~127
// y:0~63
// t:1 填充 0,清空
void OLED_DrawPoint(u8 x, u8 y, u8 t) {
  u8 pos, bx, temp = 0;
  if (x > 127 || y > 63) return;  // 超出范围了.
  pos = 7 - y / 8;
  bx = y % 8;
  temp = 1 << (7 - bx);
  if (t)
    OLED_GRAM[x][pos] |= temp;
  else
    OLED_GRAM[x][pos] &= ~temp;
}

// 清除一个点
// x:0~127
// y:0~63
void OLED_ClearPoint(u8 x, u8 y) {
  u8 i, m, n;
  i = y / 8;
  m = y % 8;
  n = 1 << m;
  OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
  OLED_GRAM[x][i] |= n;
  OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
}

// 在指定位置显示一个字符,包括部分字符
// x:0~127
// y:0~63
// size:选择字体 12/16/24
// mode:0,反白显示;1,正常显示
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 size, u8 mode) {
  u8 temp, t, t1;
  u8 y0 = y;
  u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) *
             (size / 2);  // 得到字体一个字符对应点阵集所占的字节数
  chr = chr - ' ';        // 得到偏移后的值
  for (t = 0; t < csize; t++) {
    if (size == 12)
      temp = asc2_1206[chr][t];  // 调用1206字体
    else if (size == 16)
      temp = asc2_1608[chr][t];  // 调用1608字体
    else if (size == 24)
      temp = asc2_2412[chr][t];  // 调用2412字体
    else
      return;  // 没有的字库
    for (t1 = 0; t1 < 8; t1++) {
      if (temp & 0x80)
        OLED_DrawPoint(x, y, mode);
      else
        OLED_DrawPoint(x, y, !mode);
      temp <<= 1;
      y++;
      if ((y - y0) == size) {
        y = y0;
        x++;
        break;
      }
    }
  }
}

// 显示字符串
// x,y:起点坐标
// size1:字体大小
//*chr:字符串起始地址
void OLED_ShowString(u8 x, u8 y, const u8 *p, u8 size) {
  while ((*p <= '~') && (*p >= ' '))  // 判断是不是非法字符!
  {
    if (x > (128 - (size / 2))) {
      x = 0;
      y += size;
    }
    if (y > (64 - size)) {
      y = x = 0;
      OLED_Clear();
    }
    OLED_ShowChar(x, y, *p, size, 1);
    x += size / 2;
    p++;
  }
}

// m^n
u32 OLED_Pow(u8 m, u8 n) {
  u32 result = 1;
  while (n--) {
    result *= m;
  }
  return result;
}

//////显示2个数字
//////x,y :起点坐标
//////num :数值
//////len :数字的位数
//////size:字体大小
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size) {
  u8 t, temp;
  u8 enshow = 0;
  for (t = 0; t < len; t++) {
    temp = (num / OLED_Pow(10, len - t - 1)) % 10;
    if (enshow == 0 && t < (len - 1)) {
      if (temp == 0) {
        OLED_ShowChar(x + (size / 2) * t, y, ' ', size, 1);
        continue;
      } else
        enshow = 1;
    }
    OLED_ShowChar(x + (size / 2) * t, y, temp + '0', size, 1);
  }
}

// 配置写入数据的起始位置
void OLED_WR_BP(u8 x, u8 y) {
  OLED_WR_Byte(0xb0 + y, OLED_CMD);  // 设置行起始地址
  OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
  OLED_WR_Byte((x & 0x0f) | 0x01, OLED_CMD);
}

// OLED的初始化
void OLED_Init(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  // 使能A端口时钟

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   // 推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 速度50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);             // 初始化GPIOB10,11
  GPIO_SetBits(GPIOB, GPIO_Pin_8 | GPIO_Pin_9);

  OLED_WR_Byte(0xAE, OLED_CMD);  // 关闭显示
  OLED_WR_Byte(0xD5, OLED_CMD);  // 设置时钟分频因子,震荡频率
  OLED_WR_Byte(80, OLED_CMD);    //[3:0],分频因子;[7:4],震荡频率
  OLED_WR_Byte(0xA8, OLED_CMD);  // 设置驱动路数
  OLED_WR_Byte(0X3F, OLED_CMD);  // 默认0X3F(1/64)
  OLED_WR_Byte(0xD3, OLED_CMD);  // 设置显示偏移
  OLED_WR_Byte(0X00, OLED_CMD);  // 默认为0

  OLED_WR_Byte(0x40, OLED_CMD);  // 设置显示开始行 [5:0],行数.

  OLED_WR_Byte(0x8D, OLED_CMD);  // 电荷泵设置
  OLED_WR_Byte(0x14, OLED_CMD);  // bit2，开启/关闭
  OLED_WR_Byte(0x20, OLED_CMD);  // 设置内存地址模式
  OLED_WR_Byte(
      0x02,
      OLED_CMD);  //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
  OLED_WR_Byte(0xA1, OLED_CMD);  // 段重定义设置,bit0:0,0->0;1,0->127;
  OLED_WR_Byte(0xC0, OLED_CMD);  // 设置COM扫描方向;bit3:0,普通模式;1,重定义模式
                                 // COM[N-1]->COM0;N:驱动路数
  OLED_WR_Byte(0xDA, OLED_CMD);  // 设置COM硬件引脚配置
  OLED_WR_Byte(0x12, OLED_CMD);  //[5:4]配置

  OLED_WR_Byte(0x81, OLED_CMD);  // 对比度设置
  OLED_WR_Byte(0xEF, OLED_CMD);  // 1~255;默认0X7F (亮度设置,越大越亮)
  OLED_WR_Byte(0xD9, OLED_CMD);  // 设置预充电周期
  OLED_WR_Byte(0xf1, OLED_CMD);  //[3:0],PHASE 1;[7:4],PHASE 2;
  OLED_WR_Byte(0xDB, OLED_CMD);  // 设置VCOMH 电压倍率
  OLED_WR_Byte(0x30, OLED_CMD);  //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

  OLED_WR_Byte(0xA4, OLED_CMD);  // 全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
  OLED_WR_Byte(0xA6, OLED_CMD);  // 设置显示方式;bit0:1,反相显示;0,正常显示
  OLED_WR_Byte(0xAF, OLED_CMD);  // 开启显示
  OLED_Clear();
}
