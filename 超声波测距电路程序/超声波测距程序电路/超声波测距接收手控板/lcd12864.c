#include <reg51.h>
#include <intrins.h>
//#include <at89x51.h>

sbit LCD_RS=P3^5;//��������
sbit LCD_RW=P3^6;
sbit LCD_E=P3^4;
sbit PSB =P3^1;		//PSB��Ϊ12864-12ϵ�еĴ�����ͨѶ�����л�������ʹ��8λ���нӿڣ�PSB=1
#define LCD_Data P0




#define Busy    0x80 //���ڼ��LCD״̬���е�Busy��ʶ
void WriteDataLCD(unsigned char WDLCD);
void WriteCommandLCD(unsigned char WCLCD,BuysC);
unsigned char ReadDataLCD(void);
unsigned char ReadStatusLCD(void);
void LCDInit(void);
void LCDClear(void);
void LCDFlash(void);
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData);
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData);
void DisplayImage (unsigned char code *DData);
void Delay5Ms(void);
void Delay400Ms(void);

code unsigned char  shuju[] = {"33.888"};


void main(void)
{
 Delay400Ms(); //�����ȴ�����LCD���빤��״̬
 LCDInit(); //LCM��ʼ��
 Delay5Ms(); //��ʱƬ��(�ɲ�Ҫ)
 while(1){
 		LCDClear();
		Delay400Ms();
//		Delay400Ms();
//		Delay400Ms();
//		Delay400Ms();
//		Delay400Ms();
		LCDClear();
		DisplayListChar(0,1,shuju);	//��ʾ�ֿ��е���������

		Delay400Ms();
		Delay400Ms();
//		Delay400Ms();
//		Delay400Ms();
// 		LCDFlash();	//��˸Ч��
		//}
	}
}

//1д����
void WriteDataLCD(unsigned char WDLCD)
{
 ReadStatusLCD(); //���æ 
 LCD_RS = 1;
 LCD_RW = 0;
 LCD_Data = WDLCD;
 LCD_E = 1;
 LCD_E = 1;
 LCD_E = 1;
 LCD_E = 0;
}

//2дָ��
void WriteCommandLCD(unsigned char WCLCD,BuysC) //BuysCΪ0ʱ����æ���
{
 if (BuysC) ReadStatusLCD(); //������Ҫ���æ 
 LCD_RS = 0;
 LCD_RW = 0; 
 LCD_Data = WCLCD;
 LCD_E = 1; 
 LCD_E = 1;
 LCD_E = 1;
 LCD_E = 0;  
}

//3������
unsigned char ReadDataLCD(void)
{
 LCD_RS = 1; 
 LCD_RW = 1;
 LCD_E = 0;
 LCD_E = 0;
 LCD_E = 1;
 return(LCD_Data);
}

//4��״̬
unsigned char ReadStatusLCD(void)
{
 LCD_Data = 0xFF; 
 LCD_RS = 0;
 LCD_RW = 1; 
 LCD_E = 1;
 while (LCD_Data & Busy); //���æ�ź�
 LCD_E = 0;
 return(LCD_Data);
}

void LCDInit(void) //5LCM��ʼ��
{
 WriteCommandLCD(0x30,1); //��ʾģʽ����,��ʼҪ��ÿ�μ��æ�ź�
 WriteCommandLCD(0x01,1); //��ʾ����
 WriteCommandLCD(0x06,1); // ��ʾ����ƶ�����
 WriteCommandLCD(0x0C,1); // ��ʾ�����������
}
void LCDClear(void) //6����
{
 WriteCommandLCD(0x01,1); //��ʾ����
 WriteCommandLCD(0x34,1); // ��ʾ����ƶ�����
 WriteCommandLCD(0x30,1); // ��ʾ�����������
}
void LCDFlash(void)	//7��˸Ч��
{
 WriteCommandLCD(0x08,1); //��ʾ����
 Delay400Ms();
 WriteCommandLCD(0x0c,1); // ��ʾ�����������
 Delay400Ms();
 WriteCommandLCD(0x08,1); //��ʾ����
 Delay400Ms();
 WriteCommandLCD(0x0c,1); // ��ʾ�����������
 Delay400Ms();
 WriteCommandLCD(0x08,1); //��ʾ����
 Delay400Ms();
}
//8��ָ��λ����ʾһ���ַ�
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
 if(Y<1)
 	Y=1;
 if(Y>4)
 	Y=4;
 X &= 0x0F; //����X���ܴ���16��Y���ܴ���1
 switch(Y){
	case 1:X|=0X80;break;
	case 2:X|=0X90;break;
	case 3:X|=0X88;break;
	case 4:X|=0X98;break;
	}
 WriteCommandLCD(X, 0); //���ﲻ���æ�źţ����͵�ַ��
 WriteDataLCD(DData);
}

//9��ָ��λ����ʾһ���ַ�
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData)
{
 unsigned char ListLength,X2;
  ListLength = 0;
  X2=X;
 if(Y<1)
 	Y=1;
 if(Y>4)
 	Y=4;
 X &= 0x0F; //����X���ܴ���16��Y��1-4֮��
 switch(Y){
	case 1:X2|=0X80;break;	//����������ѡ����Ӧ��ַ
	case 2:X2|=0X90;break;
	case 3:X2|=0X88;break;
	case 4:X2|=0X98;break;
	}
 WriteCommandLCD(X2, 1); //10���͵�ַ��
  while (DData[ListLength]>=0x20) //�������ִ�β���˳�
  {
   if (X <= 0x0F) //X����ӦС��0xF
    {
     WriteDataLCD(DData[ListLength]); //
     ListLength++;
	 X++;
	 Delay5Ms();
    }
  }
}

//11ͼ����ʾ122*32
void DisplayImage (unsigned char code *DData){
	unsigned char x,y,i;
	unsigned int tmp=0;
	for(i=0;i<9;){		//���������ϰ������°�������Ϊ��ʼ��ַ��ͬ����Ҫ�ֿ�
	for(x=0;x<32;x++){				//32��
		WriteCommandLCD(0x34,1);
		WriteCommandLCD((0x80+x),1);//�е�ַ
		WriteCommandLCD((0x80+i),1);	//�е�ַ���°������������е�ַ0X88
		WriteCommandLCD(0x30,1);		
		for(y=0;y<16;y++)	
			WriteDataLCD(DData[tmp+y]);//��ȡ����д��LCD
		tmp+=16;		
	}
	i+=8;
	}
	WriteCommandLCD(0x36,1);	//���书���趨
	WriteCommandLCD(0x30,1);
}

//125ms��ʱ
void Delay5Ms(void)
{
 unsigned int TempCyc = 5552;
 while(TempCyc--);
}

//13400ms��ʱ
void Delay400Ms(void)
{
 unsigned char TempCycA = 5;
 unsigned int TempCycB;
 while(TempCycA--)
 {
  TempCycB=7269;
  while(TempCycB--);
 };
}
