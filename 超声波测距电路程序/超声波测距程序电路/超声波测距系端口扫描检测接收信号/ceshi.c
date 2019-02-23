#include<AT89X51.H>
#include<intrins.h>
#include<stdio.h>
#define uchar unsigned char
#define uint unsigned int
#define nop _nop_()
#define LCD_Data P0
#define Busy    0x80 //���ڼ��LCD״̬���е�Busy��ʶ

sbit csb1=P2^0;//tr 40KHz�����������
sbit in1=P2^1;

sbit csb2=P2^2;
sbit in2=P2^3;

sbit  csb3=P2^4;
sbit  in3=P2^5;

sbit  csb4=P2^6;
sbit 	in4=P2^7;

sbit LCD_RS=P3^5;//��������
sbit LCD_RW=P3^6;
sbit LCD_E=P3^4;
//sbit PSB =P3^7;		
sbit PSB =P3^1;	
///////////////////////
//sbit LCD_RS=P0^7;//��������
//sbit LCD_RW=P0^6;
//sbit LCD_E=P0^5;
////sbit PSB =P3^7;		
//sbit PSB =P0^4;		//PSB��Ϊ12864-12ϵ�еĴ�����ͨѶ�����л�������ʹ��8λ���нӿڣ�PSB=1
////////////////////////
sbit beef =P0^3;
/*************************///LCD��ʾ�ַ�����
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
/////////////////////////////��������������Ӻ�����

void  delay100us();
void delay_nms(uint n);
void tran1(void);
void tran2(void);
void tran3(void);
void tran4(void);
void display(uint dat);
/**************************/
unsigned char  shuju[5];//////////////////////////////�ַ����壻
unsigned char  shuju_send[5];
////////////////////////////////////////////////�������������ȫ�ֱ���
unsigned char flag;
unsigned int tmp,dis,high_time ,low_time;
///////////////////////////////////////////////

void serial_init(void)
{
		TMOD=0X20;//��ʽ2�Զ���װ
		TH1=0XFD;
		TL1=0XFD;
		TR1=1;//��������/��ʱ
		SCON=0X50;//	SM0=0;SM1=1;SM2=0;REN=1;//10λ�첽ͨѶ��ʽ1//RENΪ1��ʼ���գ�Ϊ0ֹͣ����
		EA=0;
//		TR1=0;//��ʱ�����ܹ�
		
}

//���ڷ���ʱ�ǲ���Ҫ�����жϲ���ģ������ڽ�������ʱ����Ҫ�����жϲ��룻


void serial_send(void)
{
			unsigned char i;
//		EA=0;
//		TR1=0;//��ʱ�����ܹ�
//delay_ms(20);
//TR1=1;//��ʹ�ص���ҲҪ��취���ڷ�������֮ǰ
		for(i=0;i<6;i++)
			{
				SBUF=shuju_send[i];
//				while(!TI);//�ȴ������������
				Delay400Ms();
//				Delay400Ms();
				TI=0;
			}

}

/////////////////////////////////////////////////��������
void main(void)
{
//	PT1=1;
//	serial_init();
	Delay400Ms(); //�����ȴ�����LCD���빤��״̬
 LCDInit(); //LCM��ʼ��
 Delay5Ms(); //��ʱƬ��(�ɲ�Ҫ)
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
////				Delay400Ms();//ͨ����ʱ��ֹ��csb1���������ж�	
				tran3( );			//C*/
//				serial_send();				
				Delay5Ms();
////				Delay400Ms();//ͨ����ʱ��ֹ��csb1���������ж�	
				tran4( );		//D
//				serial_send();				
				Delay5Ms();
				//---Delay400Ms();		
//				Delay400Ms();
//				Delay400Ms();//ͨ����ʱ��ֹ��csb1���������ж�	
//		LCDClear();//����
//		Delay400Ms();
//		LCDClear();

	}
}
////////////////////////////////////////////////////

//��ʱ������
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
//////�������������
void tran1(void)///��ͨ������K��ֵѡ��ִ����Ӧ��I0���䳬����
{
					uchar i;
					float temp;
						TMOD=0XF0;
						TMOD |=0X01;
						TL0=0X47;
						TH0=0XFF;
						TR0=0;
						ET0=0;//�ر�timme0�ж�?
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
										//		temp=(TH0*256+TL0)*1.08/58;		//1.08Ϊ��Ƶ֮�����
												temp=(TH0*256+TL0);//*1.08/58;		//1.08Ϊ��Ƶ֮�����		
												temp=(temp/1000)/2;
												temp*=340;
												temp=temp/10;
												//��������
							if(temp<20)beef=0;
										delay_nms(60);
													beef=1;
													//
							if(temp>4000)temp=4000;
												dis =(unsigned int)temp;//dis��ֱ��������ʾ����
								delay_nms(60);
								display(dis);
								for(i=0;i<6;i++)
								{
										DisplayOneChar(i,1,shuju[i]);	//��ʾ�ֿ��е���������
								}
								serial_init();
								
								serial_send();
								
}

void tran2(void)///��ͨ������K��ֵѡ��ִ����Ӧ��I0���䳬����
{
					uchar i;
					float temp;
						TMOD=0XF0;
						TMOD |=0X01;
						TL0=0X47;
						TH0=0XFF;
						TR0=0;
						ET0=0;//�ر�timme0�ж�?
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
												temp=(TH0*256+TL0);//*1.08/58;		//1.08Ϊ��Ƶ֮�����		
												temp=(temp/1000)/2;
												temp*=340;
												temp=temp/10;
							if(temp<20)beef=0;
										delay_nms(60);
													beef=1;								
							if(temp>4000)temp=4000;
												dis =(unsigned int)temp;//dis��ֱ��������ʾ����
								delay_nms(60);
								display(dis);
								shuju[0]=shuju[0]+1;//B
								for(i=0;i<6;i++)
								DisplayOneChar(i,2,shuju[i]);	//��ʾ�ֿ��е���������
								shuju_send[0]=shuju_send[0]+1;
								serial_init();
								serial_send();
								
}
void tran3(void)///��ͨ������K��ֵѡ��ִ����Ӧ��I0���䳬����
{
					uchar i;
					float temp;
						TMOD=0XF0;
						TMOD |=0X01;
						TL0=0X47;
						TH0=0XFF;
						TR0=0;
						ET0=0;//�ر�timme0�ж�?
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
							//					temp=(TH0*256+TL0)*1.08/58;		//1.08Ϊ��Ƶ֮�����
												temp=(TH0*256+TL0);//*1.08/58;		//1.08Ϊ��Ƶ֮�����		
												temp=(temp/1000)/2;
												temp*=340;
												temp=temp/10;
												//��������
							if(temp<20)beef=0;
										delay_nms(60);
													beef=1;
													//												
							if(temp>4000)temp=4000;
												dis =(unsigned int)temp;//dis��ֱ��������ʾ����
								delay_nms(60);
								display(dis);
								shuju[0]=shuju[0]+2;//C
								for(i=0;i<6;i++)
								DisplayOneChar(i,3,shuju[i]);	//��ʾ�ֿ��е���������
								shuju_send[0]=shuju_send[0]+2;
								serial_init();
								serial_send();
//								
								
}
void tran4(void)///��ͨ������K��ֵѡ��ִ����Ӧ��I0���䳬����
{
					uchar i;
					float temp;
						TMOD=0XF0;
						TMOD |=0X01;
						TL0=0X47;
						TH0=0XFF;
						TR0=0;
						ET0=0;//�ر�timme0�ж�?
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
							//					temp=(TH0*256+TL0)*1.08/58;		//1.08Ϊ��Ƶ֮�����
												temp=(TH0*256+TL0);//*1.08/58;		//1.08Ϊ��Ƶ֮�����		
												temp=(temp/1000)/2;
												temp*=340;
												temp=temp/10;
												//��������
							if(temp<20)beef=0;
										delay_nms(60);
													beef=1;
													//												
							if(temp>4000)temp=4000;
												dis =(unsigned int)temp;//dis��ֱ��������ʾ����
								delay_nms(60);
								display(dis);
								shuju[0]=shuju[0]+3;//D
								for(i=0;i<6;i++)
								DisplayOneChar(i,4,shuju[i]);	//��ʾ�ֿ��е���������
								shuju_send[0]=shuju_send[0]+3;
								serial_init();
								serial_send();
								
								
}
///////////////////////////////////
void display(uint dat)
{

		uchar i,j,k;//�������
		i=dat/100;//��
		j=dat%100/10;//ʮλ
		k=dat%100%10;//��	
		
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
		shuju[5]=109;//����shuju[4]='m'��
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
/////////////////////////////////////











