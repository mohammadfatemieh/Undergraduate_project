#include<AT89X51.H>
#include<intrins.h>
#include<stdio.h>
#define uchar unsigned char
#define uint unsigned int
#define nop _nop_()
#define LCD_Data P0
#define Busy    0x80 //用于检测LCD状态字中的Busy标识


sbit LCD_RS=P3^5;//定义引脚
sbit LCD_RW=P3^6;
sbit LCD_E=P3^4;
//sbit PSB =P3^7;		
sbit PSB =P3^1;		//PSB脚为12864-12系列的串、并通讯功能切换，我们使用8位并行接口，PSB=1
//sbit beef =P2^6;
/*************************///LCD显示字符函数
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
/////////////////////////////超声波发射接收子函数体
uchar k[]={"arch!"};
void  delay100us(void);
void delay_nms(uint n);

void display(uint dat);
/**************************/
////////////////////////////////////////////////超声波发射接收全局变量
unsigned char flag,receive[6],info,ch;

////////////////超声波接受函数///////////////////////////////
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

//串口发送时是不需要串口中断参与的，而串口接收数据时则需要串口中断参与；

/////////////////////////////////////////////////主程序函数
void main(void)
{
		unsigned char i;
		LCDInit(); //LCM初始化
		serial_init();
		flag=0;
		while(1)
			{	
//				LCDInit(); //LCM初始化
				if(info=='A')
						for(i=0;i<6;i++)
							{	
								while(!flag);//flag=0时一直等待；
								flag=0;									
								DisplayOneChar(i,1,info);

					
							}
//								Delay400Ms();
//								Delay400Ms();		
				if(info=='B')
						for(i=0;i<6;i++)
							{	
								while(!flag);//flag=0时一直等待；
								flag=0;									
								DisplayOneChar(i,2,info);

					
							}
//								Delay400Ms();
//								Delay400Ms();		
				if(info=='C')
						for(i=0;i<6;i++)
							{	
								while(!flag);//flag=0时一直等待；
								flag=0;									
								DisplayOneChar(i,3,info);

					
							}
//								Delay400Ms();
//								Delay400Ms();						
						
				if(info=='D')
						for(i=0;i<6;i++)
							{	
								while(!flag);//flag=0时一直等待；
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
//延时函数体
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
//1写数据
void WriteDataLCD(unsigned char WDLCD)
{
 ReadStatusLCD(); //检测忙 
 LCD_RS = 1;
 LCD_RW = 0;
 LCD_Data = WDLCD;
 LCD_E = 1;
 LCD_E = 1;
 LCD_E = 1;
 LCD_E = 0;
}

//2写指令
void WriteCommandLCD(unsigned char WCLCD,BuysC) //BuysC为0时忽略忙检测
{
 if (BuysC) ReadStatusLCD(); //根据需要检测忙 
 LCD_RS = 0;
 LCD_RW = 0; 
 LCD_Data = WCLCD;
 LCD_E = 1; 
 LCD_E = 1;
 LCD_E = 1;
 LCD_E = 0;  
}

//3读数据
unsigned char ReadDataLCD(void)
{
 LCD_RS = 1; 
 LCD_RW = 1;
 LCD_E = 0;
 LCD_E = 0;
 LCD_E = 1;
 return(LCD_Data);
}

//4读状态
unsigned char ReadStatusLCD(void)
{
 LCD_Data = 0xFF; 
 LCD_RS = 0;
 LCD_RW = 1; 
 LCD_E = 1;
 while (LCD_Data & Busy); //检测忙信号
 LCD_E = 0;
 return(LCD_Data);
}

void LCDInit(void) //5LCM初始化
{
 WriteCommandLCD(0x30,1); //显示模式设置,开始要求每次检测忙信号
 WriteCommandLCD(0x01,1); //显示清屏
 WriteCommandLCD(0x06,1); // 显示光标移动设置
 WriteCommandLCD(0x0C,1); // 显示开及光标设置
}
void LCDClear(void) //6清屏
{
 WriteCommandLCD(0x01,1); //显示清屏
 WriteCommandLCD(0x34,1); // 显示光标移动设置
 WriteCommandLCD(0x30,1); // 显示开及光标设置
}

void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
 if(Y<1)
 	Y=1;
 if(Y>4)
 	Y=4;
 X &= 0x0F; //限制X不能大于16，Y不能大于1
 switch(Y){
	case 1:X|=0X80;break;
	case 2:X|=0X90;break;
	case 3:X|=0X88;break;
	case 4:X|=0X98;break;
	}
 WriteCommandLCD(X, 0); //这里不检测忙信号，发送地址码
 WriteDataLCD(DData);
}

//9按指定位置显示一串字符
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char *DData)//code *DData
{
 unsigned char ListLength,X2;
  ListLength = 0;
  X2=X;
 if(Y<1)
 	Y=1;
 if(Y>4)
 	Y=4;
 X &= 0x0F; //限制X不能大于16，Y在1-4之内
 switch(Y){
	case 1:X2|=0X80;break;	//根据行数来选择相应地址
	case 2:X2|=0X90;break;
	case 3:X2|=0X88;break;
	case 4:X2|=0X98;break;
	}
 WriteCommandLCD(X2, 1); //10发送地址码
  while (DData[ListLength]>=0x20) //若到达字串尾则退出
  {
   if (X <= 0x0F) //X坐标应小于0xF
    {
     WriteDataLCD(DData[ListLength]); //
     ListLength++;
	 X++;
	 Delay5Ms();
    }
  }
}
//125ms延时
void Delay5Ms(void)
{
 unsigned int TempCyc = 5552;
 while(TempCyc--);
}

//13400ms延时
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











