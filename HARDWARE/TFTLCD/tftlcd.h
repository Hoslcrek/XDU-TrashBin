#ifndef __LCD_H
#define __LCD_H
#include "sys.h"


extern u16	POINT_COLOR;	//Ĭ�ϻ�����ɫ
extern u16	BACK_COLOR;		//Ĭ�ϱ�����ɫ

//LCD�Ŀ��͸߶���
#define LCD_Width 	240
#define LCD_Height 	240

//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)


/*
    	LCD_PWR:	PB13
    	LCD_RST:	PB14
    	LCD_WR:		PB15
    	LCD_CS:		PB1
*/
#define	LCD_PWR		PBout(13)
#define	LCD_RST		PBout(14)
#define	LCD_WR		PBout(15)
#define	LCD_CS		PBout(1)

typedef unsigned short u16;
typedef unsigned char u8;


void LCD_Init(void);																	//��ʼ��
void LCD_DisplayOn(void);																//����ʾ
void LCD_DisplayOff(void);																//����ʾ
void LCD_Write_HalfWord(const u16 da);													//д����ֽ����ݵ�LCD
void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2);									//����������ʾ����
void LCD_Clear(u16 color);																//����
void LCD_Fill(u16 x_start, u16 y_start, u16 x_end, u16 y_end, u16 color);				//��䵥ɫ
void LCD_Draw_Point(u16 x, u16 y);														//����
void LCD_Draw_ColorPoint(u16 x, u16 y,u16 color);										//������ɫ��
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);										//����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);									//������
void LCD_Draw_Circle(u16 x0, u16 y0, u8 r);												//��Բ
void LCD_ShowChar(u16 x, u16 y, char chr, u8 size);										//��ʾһ���ַ�
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);									//��ʾһ������
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);							//��ʾ����
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,char *p);					//��ʾ�ַ���
void LCD_Show_Image(u16 x, u16 y, u16 width, u16 height, const u8 *p);					//��ʾͼƬ
void Display_ALIENTEK_LOGO(u16 x,u16 y);											
void Display_Hoslcrek_LOGO(u16 x, u16 y);
void Display_TongFei_LOGO(u16 x, u16 y);

#endif


