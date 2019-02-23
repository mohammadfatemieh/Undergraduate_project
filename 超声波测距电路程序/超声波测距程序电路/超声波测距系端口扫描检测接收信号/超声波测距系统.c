#include<AT89X51.H>
#include<intrins.h>
#include<stdio.h>


sbit in1=P3^2;//���������յ�
sbit in2=P3^3;

sbit csb1=P1^0;//40KHz�����������
sbit csb2=P1^1;
sbit csb3=P1^2;
sbit csb4=P1^3;

//sbit di=P0^7;

#define uchar unsigned char
#define uint unsigned int
#define nop _nop_()

sbit LCD_RS=P3^5;//��������
sbit LCD_RW=P3^6;
sbit LCD_E=P3^4;
sbit PSB =P3^1;		//PSB��Ϊ12864-12ϵ�еĴ�����ͨѶ�����л�������ʹ��8λ���нӿڣ�PSB=1
#define LCD_Data P0




#define Busy    0x80 //���ڼ��LCD״̬���е�Busy��ʶ

/****************************/
void init(void);//��ʼ������
void delay_nms(uint n);//��ʱMS����
void delay100us();//��ʱusus
void display(uint dat);//��ֵ��ʾ����
void tran1(void);//���������亯��
void tran2(void);
void tran3(void);//3csb���չ�1csb�����ж�
void tran4(void);//4csb���չ�2csb�����ж�
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
	 Delay400Ms(); //�����ȴ�����LCD���빤��״̬
 LCDInit(); //LCM��ʼ��
 Delay5Ms(); //��ʱƬ��(�ɲ�Ҫ)
	
init();
while(1) //
{

	uint i;

tran1();//����������
display(dis1);///////////////////////////////
delay_nms(10);	
	
//		LCDClear();//����
//		Delay400Ms();
//		LCDClear();
	
		for(i=0;i<5;i++)
		DisplayOneChar(i,1,shuju[i]);	//��ʾ�ֿ��е���������



	////////////////////////////////�˴��ɲ�������
	tran2();//����������
display(dis2);///////////////////////////////
delay_nms(10);	
	

		for(i=0;i<5;i++)
		DisplayOneChar(i,2,shuju[i]);	//��ʾ�ֿ��е���������
		
		Delay400Ms();
		Delay400Ms();//ͨ����ʱ��ֹ��csb1���������ж�
				Delay400Ms();
		Delay400Ms();
				Delay400Ms();
		Delay400Ms();
	///////////////////////////////////////////////////
tran3();//����������3��1
display(dis3);///////////////////////////////
delay_nms(10);	

	
		for(i=0;i<5;i++)
		DisplayOneChar(i,3,shuju[i]);	//��ʾ�ֿ��е���������

	tran4();//����������
display(dis4);///////////////////////////////
delay_nms(10);	
	
		for(i=0;i<5;i++)
		DisplayOneChar(i,4,shuju[i]);	//��ʾ�ֿ��е���������
		Delay400Ms();
		Delay400Ms();
}
}
/**********************************
��ʼ��
**********************************/
void init(void)
{
	P0=0XFF;
	P1=0XFF;
	P2=0XFF;
	P3=0XFF;
TMOD=0x11;//��ʱ	
	TH1=0X00;
	TL1=0X00;
	IT1=1;
TH0=0;//�Զ�ʱ��0���Ļ�
TL0=0; //
EA=1;
IT0=1;//
	//�Զ�ʱ��1���г�ʼ��


}
/**********************************
��ʱ1
**********************************/
void delay_nms(uint n)
{
uchar i; 
while(n--)
{
for(i=123;i>0;i--);
}
}

///////////////////////////////////
void display(uint dat)
{

		uchar i,j,k;//�������
i=dat/100;//��
j=dat%100/10;//ʮλ
k=dat%100%10;//��	


	shuju[0]=i+48;
		shuju[1]='.';
		shuju[2]=j+48;
		shuju[3]=k+48;
		shuju[4]=109;//����shuju[4]='m'��
}

/**********************************
����������
**********************************/
void tran1(void)
{
uchar i;
float temp;
TH0=0;
TL0=0;//???
TR0=1;//��ʱ��0��
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
EX0=1;//�����ⲿ�ж�
if(flag1==1) //�������ʱ����ת���ɾ���
{ //
temp=high_time1*256+low_time1;
temp=(temp/1000)/2;
temp*=340;
temp=temp/10;
dis1=(unsigned int)temp;//dis��ֱ���������ĺ���
flag1=0;
}
}
//////////////////////////////////////////////////////////////////
void tran2(void)
{
uchar i;
float temp;
TH1=0;
TL1=0;//???
TR1=1;//��ʱ��1��
	delay_nms(10);	
for(i=8;i>0;i--)
{
csb2=!csb2;
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
csb2=1;
delay_nms(1);
EX1=1;//�����ⲿ�ж�
if(flag2==1) //�������ʱ����ת���ɾ���
{ //
temp=high_time2*256+low_time2;
temp=(temp/1000)/2;
temp*=340;
temp=temp/10;
dis2=(unsigned int)temp;//dis��ֱ��������ʾ����
flag2=0;
}
}

////////////////3���չ�1csb
void tran3(void)
{
uchar i;
float temp;
TH0=0;
TL0=0;//???
TR0=1;//��ʱ��0��
	delay_nms(10);	
for(i=8;i>0;i--)
{
csb3=!csb3;
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
csb3=1;
delay_nms(1);
EX0=1;//�����ⲿ�ж�
if(flag1==1) //�������ʱ����ת���ɾ���
{ //
temp=high_time1*256+low_time1;
temp=(temp/1000)/2;
temp*=340;
temp=temp/10;
dis3=(unsigned int)temp;//dis��ֱ���������ĺ���
flag1=0;
}
}
////////////////4���չ�2csb
void tran4(void)
{
uchar i;
float temp;
TH1=0;
TL1=0;//???
TR1=1;//��ʱ��1��
	delay_nms(10);	
for(i=8;i>0;i--)
{
csb4=!csb4;
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
csb4=1;
delay_nms(1);
EX1=1;//�����ⲿ�ж�
if(flag2==1) //�������ʱ����ת���ɾ���
{ //
temp=high_time2*256+low_time2;
temp=(temp/1000)/2;
temp*=340;
temp=temp/10;
dis4=(unsigned int)temp;//dis��ֱ��������ʾ����
flag2=0;
}
}
/**********************************



**********************************/
void TT() interrupt 0//�жϺ�Ϊ0ʱ��Ӧ�ж�0
{
uint tmp;
TR0=0;//�رն�ʱ����
ET0=0;//�رն�ʱ������λ
flag1=1; //��־λ��Ψһ
tmp=TH0*256+TL0; //�Լ�������ֵ����ȡ
if((tmp>0)&&(tmp<60000))//�ж�����ü�ʱֵ�Ƿ���������
{
high_time1=TH0;//��ʱ�����ȡ
low_time1=TL0;

}
else //����������ʱ������ֵ����
{
high_time1=0;
low_time1=0; 
} 
}
//////�ⲿ�ж�һ
void TT1() interrupt 2//�жϺ�Ϊ2ʱ��Ӧ�ж�1
{
uint tmp;
TR1=0;//�رն�ʱ��1��
ET1=0;//�رն�ʱ������λ1
flag2=1; //��־λ��Ψһ
tmp=TH1*256+TL1; //�Լ�������ֵ����ȡ
if((tmp>0)&&(tmp<60000))//�ж�����ü�ʱֵ�Ƿ���������
{
high_time2=TH1;//��ʱ�����ȡ
low_time2=TL1;

}
else //����������ʱ������ֵ����
{
high_time2=0;
low_time2=0; 
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
