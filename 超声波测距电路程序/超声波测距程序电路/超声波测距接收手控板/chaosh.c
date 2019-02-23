#include<AT89X51.H>
#include<intrins.h>
#include<stdio.h>

#define uchar unsigned char
#define uint unsigned int
#define nop _nop_()

sbit LCD_RS=P3^5;//定义引脚
sbit LCD_RW=P3^6;
sbit LCD_E=P3^4;
sbit PSB =P3^1;		//PSB脚为12864-12系列的串、并通讯功能切换，我们使用8位并行接口，PSB=1
////////////////////////////////////csb发射接收角//////////////////////////
sbit in1=P1^4;//超声波接收到
sbit in2=P1^5;
sbit in3=P1^6;//超声波接收到
sbit in4=P1^7;

sbit csb1=P1^0;//40KHz超声波发射脚
sbit csb2=P1^1;
sbit csb3=P1^2;
sbit csb4=P1^3;
/////////////////////////////////////////////////
#define LCD_Data P0
#define Busy    0x80 //用于检测LCD状态字中的Busy标识

/****************************/
void init(void);//初始化函数
void delay_nms(uint n);//延时MS函数
void delay100us();//延时usus
void display(uint dat);//数值显示函数
void tran1(void);//超声波发射函数
void tran2(void);
void tran3(void);//3csb接收共1csb接收中断
void tran4(void);//4csb接收供2csb接收中断
/***************************/

/*************************/
void WriteDataLCD(unsigned char WDLCD);
void WriteCommandLCD(unsigned char WCLCD,BuysC);
unsigned char ReadDataLCD(void);
unsigned char ReadStatusLCD(void);
void LCDInit(void);
void LCDClear(void);
//void LCDFlash(void);
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData);
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData);
//void DisplayImage (unsigned char code *DData);
void Delay5Ms(void);
void Delay400Ms(void);
/*************************/
uint dis1,dis2,dis3,dis4,H=100,L=20;
uchar flag1=0,flag2=0,high_time1,low_time1,high_time2,low_time2,m=0;

unsigned char  shuju[5];

void  delay100us()
{
    uchar i;
        for(i=0;i<50;i++)
            ;
}


/**********************************
///////////////////////////*/
void main(void)
{
	 Delay400Ms(); //启动等待，等LCD讲入工作状态
 LCDInit(); //LCM初始化
 Delay5Ms(); //延时片刻(可不要)
	
init();
	while(1) //
{

	uint i;
init();
tran1();//超声波发射
display(dis1);///////////////////////////////
delay_nms(10);	
	
		LCDClear();//清屏
		Delay400Ms();
		LCDClear();
	
		for(i=0;i<5;i++)
		DisplayOneChar(i,1,shuju[i]);	//显示字库中的中文数子



//	////////////////////////////////此处可不用清屏
//	tran2();//超声波发射
//display(dis2);///////////////////////////////
//delay_nms(10);	
//	

//		for(i=0;i<5;i++)
//		DisplayOneChar(i,2,shuju[i]);	//显示字库中的中文数子
//		
//		Delay400Ms();
//		Delay400Ms();//通过延时防止因csb1接收引起中断
//				Delay400Ms();
//		Delay400Ms();
//				Delay400Ms();
//		Delay400Ms();
//	///////////////////////////////////////////////////
//tran3();//超声波发射3共1
//display(dis3);///////////////////////////////
//delay_nms(10);	

//	
//		for(i=0;i<5;i++)
//		DisplayOneChar(i,3,shuju[i]);	//显示字库中的中文数子

//	tran4();//超声波发射
//display(dis4);///////////////////////////////
//delay_nms(10);	
//	
//		for(i=0;i<5;i++)
//		DisplayOneChar(i,4,shuju[i]);	//显示字库中的中文数子
		Delay400Ms();
		Delay400Ms();
}
}
/**********************************
初始化
**********************************/
void init(void)
{
	P0=0XFF;
	P1=0XFF;
	P2=0XFF;
	P3=0XFF;
	TMOD=0x11;//计时	器1和计时器0工作放肆
	
	TH1=0X00;
	TL1=0X00;
	TH0=0;//对定时器0初四画
	TL0=0; //
	
	EA=1;	
	IT1=1;
	IT0=1;//
		//对定时器1进行初始化
}
/**********************************
延时1
**********************************/
void delay_nms(uint n)
{
uchar i; 
while(n--)
{
for(i=123;i>0;i--);
}
}
/**********************************
超声波发射
**********************************/
void tran1(void)
{
uchar i;
float temp;
TH0=0;
TL0=0;//
TR0=1;//定时器0打开
	delay_nms(10);	
for(i=8;i>0;i--)
{
csb1=!csb1;
nop;
nop;
nop;
nop;
nop;
nop;
nop;
nop;
nop;
}
csb1=1;
delay_nms(1);

if(in1==1)
{
nop;
nop;
nop;
nop;
nop;
nop;
while(in1==0) //将所测的时间数转换成距离
{ //
	uint tmp;
	TR0=0;//关闭定时器；
	ET0=0;//关闭定时器允许位
	tmp=TH0*256+TL0; //对计数器数值的提取
			if((tmp>0)&&(tmp<60000))//判断所测得计时值是否在量程类
				{
				high_time1=TH0;//对时间的提取
				low_time1=TL0;

				}
				else //若超出量程时对所记值回零
				{
				high_time1=0;
				low_time1=0; 
				} 
temp=high_time1*256+low_time1;
temp=(temp/1000)/2;
temp*=340;
temp=temp/10;
dis1=(unsigned int)temp;//dis将直接用于仙四函数
				break;
}
}
}
///////////////////////////////////
void display(uint dat)
{

		uchar i,j,k;//定义变量
i=dat/100;//白
j=dat%100/10;//十位
k=dat%100%10;//个	


	shuju[0]=i+48;
		shuju[1]='.';
		shuju[2]=j+48;
		shuju[3]=k+48;
		shuju[4]=109;//或者shuju[4]='m'；
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
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData)
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




