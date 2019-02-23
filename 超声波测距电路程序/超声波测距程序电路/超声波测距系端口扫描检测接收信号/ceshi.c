#include<AT89X51.H>
#include<intrins.h>
#include<stdio.h>
#define uchar unsigned char
#define uint unsigned int
#define nop _nop_()
#define LCD_Data P0
#define Busy    0x80 //用于检测LCD状态字中的Busy标识

sbit csb1=P2^0;//tr 40KHz超声波发射脚
sbit in1=P2^1;

sbit csb2=P2^2;
sbit in2=P2^3;

sbit  csb3=P2^4;
sbit  in3=P2^5;

sbit  csb4=P2^6;
sbit 	in4=P2^7;

sbit LCD_RS=P3^5;//定义引脚
sbit LCD_RW=P3^6;
sbit LCD_E=P3^4;
//sbit PSB =P3^7;		
sbit PSB =P3^1;	
///////////////////////
//sbit LCD_RS=P0^7;//定义引脚
//sbit LCD_RW=P0^6;
//sbit LCD_E=P0^5;
////sbit PSB =P3^7;		
//sbit PSB =P0^4;		//PSB脚为12864-12系列的串、并通讯功能切换，我们使用8位并行接口，PSB=1
////////////////////////
sbit beef =P0^3;
/*************************///LCD显示字符函数
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
/////////////////////////////超声波发射接收子函数体

void  delay100us();
void delay_nms(uint n);
void tran1(void);
void tran2(void);
void tran3(void);
void tran4(void);
void display(uint dat);
/**************************/
unsigned char  shuju[5];//////////////////////////////字符定义；
unsigned char  shuju_send[5];
////////////////////////////////////////////////超声波发射接收全局变量
unsigned char flag;
unsigned int tmp,dis,high_time ,low_time;
///////////////////////////////////////////////

void serial_init(void)
{
		TMOD=0X20;//方式2自动重装
		TH1=0XFD;
		TL1=0XFD;
		TR1=1;//启动计数/定时
		SCON=0X50;//	SM0=0;SM1=1;SM2=0;REN=1;//10位异步通讯方式1//REN为1开始接收，为0停止接收
		EA=0;
//		TR1=0;//定时器不能关
		
}

//串口发送时是不需要串口中断参与的，而串口接收数据时则需要串口中断参与；


void serial_send(void)
{
			unsigned char i;
//		EA=0;
//		TR1=0;//定时器不能关
//delay_ms(20);
//TR1=1;//即使关掉了也要想办法打开在发送数据之前
		for(i=0;i<6;i++)
			{
				SBUF=shuju_send[i];
//				while(!TI);//等待发送数据完成
				Delay400Ms();
//				Delay400Ms();
				TI=0;
			}

}

/////////////////////////////////////////////////主程序函数
void main(void)
{
//	PT1=1;
//	serial_init();
	Delay400Ms(); //启动等待，等LCD讲入工作状态
 LCDInit(); //LCM初始化
 Delay5Ms(); //延时片刻(可不要)
//	PT0=0;

	while(1)
	{
//		uint i;
			
				tran1( );		//A
				Delay5Ms();
				tran2( );	//B
////				serial_send();				
				/*Delay5Ms();
				Delay5Ms();
////				Delay400Ms();//通过延时防止因csb1接收引起中断	
				tran3( );			//C*/
//				serial_send();				
				Delay5Ms();
////				Delay400Ms();//通过延时防止因csb1接收引起中断	
				tran4( );		//D
//				serial_send();				
				Delay5Ms();
				//---Delay400Ms();		
//				Delay400Ms();
//				Delay400Ms();//通过延时防止因csb1接收引起中断	
//		LCDClear();//清屏
//		Delay400Ms();
//		LCDClear();

	}
}
////////////////////////////////////////////////////

//延时函数体
void  delay100us()
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
//////超声波发射程序
void tran1(void)///可通过设置K数值选择执行相应的I0发射超声波
{
					uchar i;
					float temp;
						TMOD=0XF0;
						TMOD |=0X01;
						TL0=0X47;
						TH0=0XFF;
						TR0=0;
						ET0=0;//关闭timme0中断?
						csb1=1;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;

										csb1=0;
										TR0=0;
										TH0=0;
										TL0=0;
											
							while(in1==0);
							TR0=1;
							while(in1==1);
							TR0=0;		
										//		temp=(TH0*256+TL0)*1.08/58;		//1.08为分频之后的数
												temp=(TH0*256+TL0);//*1.08/58;		//1.08为分频之后的数		
												temp=(temp/1000)/2;
												temp*=340;
												temp=temp/10;
												//蜂鸣器响
							if(temp<20)beef=0;
										delay_nms(60);
													beef=1;
													//
							if(temp>4000)temp=4000;
												dis =(unsigned int)temp;//dis将直接用于显示函数
								delay_nms(60);
								display(dis);
								for(i=0;i<6;i++)
								{
										DisplayOneChar(i,1,shuju[i]);	//显示字库中的中文数子
								}
								serial_init();
								
								serial_send();
								
}

void tran2(void)///可通过设置K数值选择执行相应的I0发射超声波
{
					uchar i;
					float temp;
						TMOD=0XF0;
						TMOD |=0X01;
						TL0=0X47;
						TH0=0XFF;
						TR0=0;
						ET0=0;//关闭timme0中断?
						csb2=1;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;

										csb2=0;
										TR0=0;
										TH0=0;
										TL0=0;
											
							while(in2==0);
							TR0=1;
							while(in2==1);
							TR0=0;		
												temp=(TH0*256+TL0);//*1.08/58;		//1.08为分频之后的数		
												temp=(temp/1000)/2;
												temp*=340;
												temp=temp/10;
							if(temp<20)beef=0;
										delay_nms(60);
													beef=1;								
							if(temp>4000)temp=4000;
												dis =(unsigned int)temp;//dis将直接用于显示函数
								delay_nms(60);
								display(dis);
								shuju[0]=shuju[0]+1;//B
								for(i=0;i<6;i++)
								DisplayOneChar(i,2,shuju[i]);	//显示字库中的中文数子
								shuju_send[0]=shuju_send[0]+1;
								serial_init();
								serial_send();
								
}
void tran3(void)///可通过设置K数值选择执行相应的I0发射超声波
{
					uchar i;
					float temp;
						TMOD=0XF0;
						TMOD |=0X01;
						TL0=0X47;
						TH0=0XFF;
						TR0=0;
						ET0=0;//关闭timme0中断?
						csb3=1;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;

										csb3=0;
										TR0=0;
										TH0=0;
										TL0=0;
											
							while(in3==0);
							TR0=1;
							while(in3==1);
							TR0=0;		
							//					temp=(TH0*256+TL0)*1.08/58;		//1.08为分频之后的数
												temp=(TH0*256+TL0);//*1.08/58;		//1.08为分频之后的数		
												temp=(temp/1000)/2;
												temp*=340;
												temp=temp/10;
												//蜂鸣器响
							if(temp<20)beef=0;
										delay_nms(60);
													beef=1;
													//												
							if(temp>4000)temp=4000;
												dis =(unsigned int)temp;//dis将直接用于显示函数
								delay_nms(60);
								display(dis);
								shuju[0]=shuju[0]+2;//C
								for(i=0;i<6;i++)
								DisplayOneChar(i,3,shuju[i]);	//显示字库中的中文数子
								shuju_send[0]=shuju_send[0]+2;
								serial_init();
								serial_send();
//								
								
}
void tran4(void)///可通过设置K数值选择执行相应的I0发射超声波
{
					uchar i;
					float temp;
						TMOD=0XF0;
						TMOD |=0X01;
						TL0=0X47;
						TH0=0XFF;
						TR0=0;
						ET0=0;//关闭timme0中断?
						csb4=1;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;
						nop;

										csb4=0;
										TR0=0;
										TH0=0;
										TL0=0;
											
							while(in4==0);
							TR0=1;
							while(in4==1);
							TR0=0;		
							//					temp=(TH0*256+TL0)*1.08/58;		//1.08为分频之后的数
												temp=(TH0*256+TL0);//*1.08/58;		//1.08为分频之后的数		
												temp=(temp/1000)/2;
												temp*=340;
												temp=temp/10;
												//蜂鸣器响
							if(temp<20)beef=0;
										delay_nms(60);
													beef=1;
													//												
							if(temp>4000)temp=4000;
												dis =(unsigned int)temp;//dis将直接用于显示函数
								delay_nms(60);
								display(dis);
								shuju[0]=shuju[0]+3;//D
								for(i=0;i<6;i++)
								DisplayOneChar(i,4,shuju[i]);	//显示字库中的中文数子
								shuju_send[0]=shuju_send[0]+3;
								serial_init();
								serial_send();
								
								
}
///////////////////////////////////
void display(uint dat)
{

		uchar i,j,k;//定义变量
		i=dat/100;//白
		j=dat%100/10;//十位
		k=dat%100%10;//个	
		
//		serial_char(i);
//		delay_nms(40);
//		serial_char(j);
//		delay_nms(40);
//		serial_char(k);
//		delay_nms(40);
		shuju_send[0]=65;//A
		shuju_send[1]=i+48;
		shuju_send[2]='.';
		shuju_send[3]=j+48;
		shuju_send[4]=k+48;
		shuju_send[5]=109;
		
		shuju[0]=65;
		shuju[1]=i+48;
		shuju[2]='.';
		shuju[3]=j+48;
		shuju[4]=k+48;
		shuju[5]=109;//或者shuju[4]='m'；
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
/////////////////////////////////////











