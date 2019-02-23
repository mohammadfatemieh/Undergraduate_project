#include<AT89X51.H>
#include<intrins.h>
#include<stdio.h>
#define uchar unsigned char
#define uint unsigned int
#define nop _nop_()
#define LCD_Data P2
#define Busy    0x80 //ÓÃÓÚ¼ì²âLCD×´Ì¬×ÖÖĞµÄBusy±êÊ¶
/*=================================

Ô´³ÌĞò


====================================*/
sbit csb1=P1^0;//tr 40KHz³¬Éù²¨·¢Éä½Å
sbit in1=P1^1;

sbit csb2=P1^2;
sbit in2=P1^3;

sbit  csb3=P1^4;
sbit  in3=P1^5;

sbit  csb4=P1^6;
sbit 	in4=P1^7;


sbit LCD_RS=P0^7;//¶¨ÒåÒı½Å
sbit LCD_RW=P0^6;
sbit LCD_E=P0^5;
//sbit PSB =P3^7;		
sbit PSB =P0^4;		//PSB½ÅÎª12864-12ÏµÁĞµÄ´®¡¢²¢Í¨Ñ¶¹¦ÄÜÇĞ»»£¬ÎÒÃÇÊ¹ÓÃ8Î»²¢ĞĞ½Ó¿Ú£¬PSB=1
sbit beef =P0^3;
/*************************///LCDÏÔÊ¾×Ö·ûº¯Êı
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
/////////////////////////////³¬Éù²¨·¢Éä½ÓÊÕ×Óº¯ÊıÌå

void  delay100us();
void delay_nms(uint n);
void tran1(void);
void tran2(void);
void tran3(void);
void tran4(void);
void display(uint dat);
/**************************/
unsigned char  shuju[5];//////////////////////////////×Ö·û¶¨Òå£»
unsigned char  shuju_send[5];
////////////////////////////////////////////////³¬Éù²¨·¢Éä½ÓÊÕÈ«¾Ö±äÁ¿
unsigned char flag;
unsigned int tmp,dis,high_time ,low_time;
///////////////////////////////////////////////

void serial_init(void)
{
		TMOD=0X20;//·½Ê½2×Ô¶¯ÖØ×°
		TH1=0XFD;
		TL1=0XFD;
		TR1=1;//Æô¶¯¼ÆÊı/¶¨Ê±
		SCON=0X50;//	SM0=0;SM1=1;SM2=0;REN=1;//10Î»Òì²½Í¨Ñ¶·½Ê½1//RENÎª1¿ªÊ¼½ÓÊÕ£¬Îª0Í£Ö¹½ÓÊÕ
		EA=0;
//		TR1=0;//¶¨Ê±Æ÷²»ÄÜ¹Ø
		
}

//´®¿Ú·¢ËÍÊ±ÊÇ²»ĞèÒª´®¿ÚÖĞ¶Ï²ÎÓëµÄ£¬¶ø´®¿Ú½ÓÊÕÊı¾İÊ±ÔòĞèÒª´®¿ÚÖĞ¶Ï²ÎÓë£»


void serial_send(void)
{
			unsigned char i;
//		EA=0;
//		TR1=0;//¶¨Ê±Æ÷²»ÄÜ¹Ø
//delay_ms(20);
//TR1=1;//¼´Ê¹¹ØµôÁËÒ²ÒªÏë°ì·¨´ò¿ªÔÚ·¢ËÍÊı¾İÖ®Ç°
		for(i=0;i<6;i++)
			{
				SBUF=shuju_send[i];
//				while(!TI);//µÈ´ı·¢ËÍÊı¾İÍê³É
				Delay400Ms();
//				Delay400Ms();
				TI=0;
			}

}

/////////////////////////////////////////////////Ö÷³ÌĞòº¯Êı
void main(void)
{
//	PT1=1;
//	serial_init();
	Delay400Ms(); //Æô¶¯µÈ´ı£¬µÈLCD½²Èë¹¤×÷×´Ì¬
 LCDInit(); //LCM³õÊ¼»¯
 Delay5Ms(); //ÑÓÊ±Æ¬¿Ì(¿É²»Òª)
//	PT0=0;

	while(1)
	{
//		uint i;
			
				tran1( );		

				tran2( );	
////				serial_send();				
				Delay400Ms();
////				Delay400Ms();//Í¨¹ıÑÓÊ±·ÀÖ¹Òòcsb1½ÓÊÕÒıÆğÖĞ¶Ï	
				tran3( );			
//				serial_send();				
				Delay400Ms();
////				Delay400Ms();//Í¨¹ıÑÓÊ±·ÀÖ¹Òòcsb1½ÓÊÕÒıÆğÖĞ¶Ï	
				tran4( );		
//				serial_send();				
				Delay400Ms();
				Delay400Ms();		
//				Delay400Ms();
//				Delay400Ms();//Í¨¹ıÑÓÊ±·ÀÖ¹Òòcsb1½ÓÊÕÒıÆğÖĞ¶Ï	
//		LCDClear();//ÇåÆÁ
//		Delay400Ms();
//		LCDClear();

	}
}
////////////////////////////////////////////////////

//ÑÓÊ±º¯ÊıÌå
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
//////³¬Éù²¨·¢Éä³ÌĞò
void tran1(void)///¿ÉÍ¨¹ıÉèÖÃKÊıÖµÑ¡ÔñÖ´ĞĞÏàÓ¦µÄI0·¢Éä³¬Éù²¨
{
					uchar i;
					float temp;
						TMOD=0XF0;
						TMOD |=0X01;
						TL0=0X47;
						TH0=0XFF;
						TR0=0;
						ET0=0;//¹Ø±Õtimme0ÖĞ¶Ï£
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
												temp=(TH0*256+TL0)*1.08/58;		//1.08Îª·ÖÆµÖ®ºóµÄÊı
												//·äÃùÆ÷Ïì
							if(temp<20)beef=0;
										delay_nms(60);
													beef=1;
													//
							if(temp>4000)temp=4000;
												dis =(unsigned int)temp;//dis½«Ö±½ÓÓÃÓÚÏÔÊ¾º¯Êı
								delay_nms(60);
								display(dis);
								for(i=0;i<6;i++)
								{
										DisplayOneChar(i,1,shuju[i]);	//ÏÔÊ¾×Ö¿âÖĞµÄÖĞÎÄÊı×Ó
								}
								serial_init();
								
								serial_send();
								
}

void tran2(void)///¿ÉÍ¨¹ıÉèÖÃKÊıÖµÑ¡ÔñÖ´ĞĞÏàÓ¦µÄI0·¢Éä³¬Éù²¨
{
					uchar i;
					float temp;
						TMOD=0XF0;
						TMOD |=0X01;
						TL0=0X47;
						TH0=0XFF;
						TR0=0;
						ET0=0;//¹Ø±Õtimme0ÖĞ¶Ï£
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
												temp=(TH0*256+TL0)*1.08/58;		//1.08Îª·ÖÆµÖ®ºóµÄÊı
												//·äÃùÆ÷Ïì
							if(temp<20)beef=0;
										delay_nms(60);
													beef=1;
													//												
							if(temp>4000)temp=4000;
												dis =(unsigned int)temp;//dis½«Ö±½ÓÓÃÓÚÏÔÊ¾º¯Êı
								delay_nms(60);
								display(dis);
								shuju[0]=shuju[0]+1;
								for(i=0;i<6;i++)
								DisplayOneChar(i,2,shuju[i]);	//ÏÔÊ¾×Ö¿âÖĞµÄÖĞÎÄÊı×Ó
								shuju_send[0]=shuju_send[0]+1;
								serial_init();
								serial_send();
								
}
void tran3(void)///¿ÉÍ¨¹ıÉèÖÃKÊıÖµÑ¡ÔñÖ´ĞĞÏàÓ¦µÄI0·¢Éä³¬Éù²¨
{
					uchar i;
					float temp;
						TMOD=0XF0;
						TMOD |=0X01;
						TL0=0X47;
						TH0=0XFF;
						TR0=0;
						ET0=0;//¹Ø±Õtimme0ÖĞ¶Ï£
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
												temp=(TH0*256+TL0)*1.08/58;		//1.08Îª·ÖÆµÖ®ºóµÄÊı
												//·äÃùÆ÷Ïì
							if(temp<20)beef=0;
										delay_nms(60);
													beef=1;
													//												
							if(temp>4000)temp=4000;
												dis =(unsigned int)temp;//dis½«Ö±½ÓÓÃÓÚÏÔÊ¾º¯Êı
								delay_nms(60);
								display(dis);
								shuju[0]=shuju[0]+2;
								for(i=0;i<6;i++)
								DisplayOneChar(i,3,shuju[i]);	//ÏÔÊ¾×Ö¿âÖĞµÄÖĞÎÄÊı×Ó
								shuju_send[0]=shuju_send[0]+2;
								serial_init();
								serial_send();
//								
								
}
void tran4(void)///¿ÉÍ¨¹ıÉèÖÃKÊıÖµÑ¡ÔñÖ´ĞĞÏàÓ¦µÄI0·¢Éä³¬Éù²¨
{
					uchar i;
					float temp;
						TMOD=0XF0;
						TMOD |=0X01;
						TL0=0X47;
						TH0=0XFF;
						TR0=0;
						ET0=0;//¹Ø±Õtimme0ÖĞ¶Ï£
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
												temp=(TH0*256+TL0)*1.08/58;		//1.08Îª·ÖÆµÖ®ºóµÄÊı
												//·äÃùÆ÷Ïì
							if(temp<20)beef=0;
										delay_nms(60);
													beef=1;
													//												
							if(temp>4000)temp=4000;
												dis =(unsigned int)temp;//dis½«Ö±½ÓÓÃÓÚÏÔÊ¾º¯Êı
								delay_nms(60);
								display(dis);
								shuju[0]=shuju[0]+3;
								for(i=0;i<6;i++)
								DisplayOneChar(i,4,shuju[i]);	//ÏÔÊ¾×Ö¿âÖĞµÄÖĞÎÄÊı×Ó
								shuju_send[0]=shuju_send[0]+3;
								serial_init();
								serial_send();
								
								
}
///////////////////////////////////
void display(uint dat)
{

		uchar i,j,k;//¶¨Òå±äÁ¿
		i=dat/100;//°×
		j=dat%100/10;//Ê®Î»
		k=dat%100%10;//¸ö	
		
//		serial_char(i);
//		delay_nms(40);
//		serial_char(j);
//		delay_nms(40);
//		serial_char(k);
//		delay_nms(40);
		shuju_send[0]=65;
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
		shuju[5]=109;//»òÕßshuju[4]='m'£»
}



////////////////////////////////////////////
//1Ğ´Êı¾İ
void WriteDataLCD(unsigned char WDLCD)
{
 ReadStatusLCD(); //¼ì²âÃ¦ 
 LCD_RS = 1;
 LCD_RW = 0;
 LCD_Data = WDLCD;
 LCD_E = 1;
 LCD_E = 1;
 LCD_E = 1;
 LCD_E = 0;
}

//2Ğ´Ö¸Áî
void WriteCommandLCD(unsigned char WCLCD,BuysC) //BuysCÎª0Ê±ºöÂÔÃ¦¼ì²â
{
 if (BuysC) ReadStatusLCD(); //¸ù¾İĞèÒª¼ì²âÃ¦ 
 LCD_RS = 0;
 LCD_RW = 0; 
 LCD_Data = WCLCD;
 LCD_E = 1; 
 LCD_E = 1;
 LCD_E = 1;
 LCD_E = 0;  
}

//3¶ÁÊı¾İ
unsigned char ReadDataLCD(void)
{
 LCD_RS = 1; 
 LCD_RW = 1;
 LCD_E = 0;
 LCD_E = 0;
 LCD_E = 1;
 return(LCD_Data);
}

//4¶Á×´Ì¬
unsigned char ReadStatusLCD(void)
{
 LCD_Data = 0xFF; 
 LCD_RS = 0;
 LCD_RW = 1; 
 LCD_E = 1;
 while (LCD_Data & Busy); //¼ì²âÃ¦ĞÅºÅ
 LCD_E = 0;
 return(LCD_Data);
}

void LCDInit(void) //5LCM³õÊ¼»¯
{
 WriteCommandLCD(0x30,1); //ÏÔÊ¾Ä£Ê½ÉèÖÃ,¿ªÊ¼ÒªÇóÃ¿´Î¼ì²âÃ¦ĞÅºÅ
 WriteCommandLCD(0x01,1); //ÏÔÊ¾ÇåÆÁ
 WriteCommandLCD(0x06,1); // ÏÔÊ¾¹â±êÒÆ¶¯ÉèÖÃ
 WriteCommandLCD(0x0C,1); // ÏÔÊ¾¿ª¼°¹â±êÉèÖÃ
}
void LCDClear(void) //6ÇåÆÁ
{
 WriteCommandLCD(0x01,1); //ÏÔÊ¾ÇåÆÁ
 WriteCommandLCD(0x34,1); // ÏÔÊ¾¹â±êÒÆ¶¯ÉèÖÃ
 WriteCommandLCD(0x30,1); // ÏÔÊ¾¿ª¼°¹â±êÉèÖÃ
}

void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
 if(Y<1)
 	Y=1;
 if(Y>4)
 	Y=4;
 X &= 0x0F; //ÏŞÖÆX²»ÄÜ´óÓÚ16£¬Y²»ÄÜ´óÓÚ1
 switch(Y){
	case 1:X|=0X80;break;
	case 2:X|=0X90;break;
	case 3:X|=0X88;break;
	case 4:X|=0X98;break;
	}
 WriteCommandLCD(X, 0); //ÕâÀï²»¼ì²âÃ¦ĞÅºÅ£¬·¢ËÍµØÖ·Âë
 WriteDataLCD(DData);
}

//9°´Ö¸¶¨Î»ÖÃÏÔÊ¾Ò»´®×Ö·û
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData)
{
 unsigned char ListLength,X2;
  ListLength = 0;
  X2=X;
 if(Y<1)
 	Y=1;
 if(Y>4)
 	Y=4;
 X &= 0x0F; //ÏŞÖÆX²»ÄÜ´óÓÚ16£¬YÔÚ1-4Ö®ÄÚ
 switch(Y){
	case 1:X2|=0X80;break;	//¸ù¾İĞĞÊıÀ´Ñ¡ÔñÏàÓ¦µØÖ·
	case 2:X2|=0X90;break;
	case 3:X2|=0X88;break;
	case 4:X2|=0X98;break;
	}
 WriteCommandLCD(X2, 1); //10·¢ËÍµØÖ·Âë
  while (DData[ListLength]>=0x20) //Èôµ½´ï×Ö´®Î²ÔòÍË³ö
  {
   if (X <= 0x0F) //X×ø±êÓ¦Ğ¡ÓÚ0xF
    {
     WriteDataLCD(DData[ListLength]); //
     ListLength++;
	 X++;
	 Delay5Ms();
    }
  }
}
//125msÑÓÊ±
void Delay5Ms(void)
{
 unsigned int TempCyc = 5552;
 while(TempCyc--);
}

//13400msÑÓÊ±
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











