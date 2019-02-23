#include<AT89X51.H>
#include<intrins.h>
#include<stdio.h>
#define uchar unsigned char
#define uint unsigned int
#define nop _nop_()
#define LCD_Data P0
#define Busy    0x80 //���ڼ��LCD״̬���е�Busy��ʶ


sbit LCD_RS=P3^5;//��������
sbit LCD_RW=P3^6;
sbit LCD_E=P3^4;
//sbit PSB =P3^7;		
sbit PSB =P3^1;		//PSB��Ϊ12864-12ϵ�еĴ�����ͨѶ�����л�������ʹ��8λ���нӿڣ�PSB=1
//sbit beef =P2^6;
/*************************///LCD��ʾ�ַ�����
void WriteDataLCD(unsigned char WDLCD);
void WriteCommandLCD(unsigned char WCLCD,BuysC);
unsigned char ReadDataLCD(void);
unsigned char ReadStatusLCD(void);
void LCDInit(void);
void LCDClear(void);
//void LCDFlash(void);
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData);
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char *DData);
//void DisplayImage (unsigned char code *DData);
void Delay5Ms(void);
void Delay400Ms(void);
/*************************/
/////////////////////////////��������������Ӻ�����
uchar k[]={"arch!"};
void  delay100us(void);
void delay_nms(uint n);

void display(uint dat);
/**************************/
////////////////////////////////////////////////�������������ȫ�ֱ���
unsigned char flag,receive[6],info,ch;

////////////////���������ܺ���///////////////////////////////
void serial_init(void)
{
		flag=0;
		TMOD=0X20;
		TH1=0XFD;
		TL1=0XFD;
		TR1=1;
		
		SM0=0;
		SM1=1;
		REN=1;	
		ES=1;	
		EA=1;
		
}

//���ڷ���ʱ�ǲ���Ҫ�����жϲ���ģ������ڽ�������ʱ����Ҫ�����жϲ��룻

/////////////////////////////////////////////////��������
void main(void)
{
		unsigned char i;
		LCDInit(); //LCM��ʼ��
		serial_init();
		flag=0;
		while(1)
			{	
//				LCDInit(); //LCM��ʼ��
				if(info=='A')
						for(i=0;i<6;i++)
							{	
								while(!flag);//flag=0ʱһֱ�ȴ���
								flag=0;									
								DisplayOneChar(i,1,info);

					
							}
//								Delay400Ms();
//								Delay400Ms();		
				if(info=='B')
						for(i=0;i<6;i++)
							{	
								while(!flag);//flag=0ʱһֱ�ȴ���
								flag=0;									
								DisplayOneChar(i,2,info);

					
							}
//								Delay400Ms();
//								Delay400Ms();		
				if(info=='C')
						for(i=0;i<6;i++)
							{	
								while(!flag);//flag=0ʱһֱ�ȴ���
								flag=0;									
								DisplayOneChar(i,3,info);

					
							}
//								Delay400Ms();
//								Delay400Ms();						
						
				if(info=='D')
						for(i=0;i<6;i++)
							{	
								while(!flag);//flag=0ʱһֱ�ȴ���
								flag=0;									
								DisplayOneChar(i,4,info);
							}
//								Delay400Ms();
//								Delay400Ms();
//								Delay400Ms();
//								Delay400Ms();								
//							
			}
	
}
////////////////////////////////////////////////////
void serial() interrupt 4
{
	
			RI=0;
			info=SBUF;
			flag=1; 
}
//��ʱ������
void  delay100us(void)
{
    uchar i;
        for(i=0;i<50;i++)
            ;
}
void delay_nms(uint n)
{
uchar i; 
while(n--)
{
for(i=123;i>0;i--);
}
}




////////////////////////////////////////////
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
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char *DData)//code *DData
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
/////////////////////////////////////











