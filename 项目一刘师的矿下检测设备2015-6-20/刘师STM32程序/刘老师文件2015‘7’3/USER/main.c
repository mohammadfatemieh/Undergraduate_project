#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "spi.h"
#include "flash.h"
#include "mmc_sd.h"
#include "ff.h"
//#include "integer.h"
#include "diskio.h"
#include "text.h"
//#include "24cxx.h"
#include "fontupd.h"
#include "stdio.h"
#include "string.h"
#include "picdecoder.h"
//#include "adc.h"
#include "AD24.h"
//Mini STM32开发板范例代码25
//图片显示 实验
//正点原子@ALIENTEK
//技术论坛:www.openedv.com

/******************************************************
如果追加文件则需要移动文件指针，f_lseek函数则派上用处了。此函数用于文件指针的移动，
共有两个参数，第一个参数为文件对象，第二个参数为移动的字节数。
在这里则F_LSEEK(&fsrc，fsrc.fsize)指向文件结尾，即可追加文件。
注意第二个参数是相对于文件的开始，如F_LSEEK(&fsrc，3000)，表示移动到文件开头3000个字节处。
灵活使用可以方便的操作文件。
******************************************************/
/////////////////////////////////////////////////////////////////
uint8_t textFileBuffer[] = "中英文测试字符串 \r\nChinese and English test strings \r\n";
	char ADC_Capture[]="中英文测试字符串1\r\nChinese and English test strings1 \r\n";
	char ADC_Capture_Data[14];
void viewPictures(const char *fileDir);
 void zuobiao(void);


void writetext(char file_dir[])
	{
			FRESULT res;
			FIL  fdst;
	/////////////写文件///////////////////
			res = f_open(&fdst, file_dir, FA_OPEN_ALWAYS | FA_WRITE);

			if(res != FR_OK){
			//printf("open file error : %d\n\r",res);//调试发送电脑端查看
			}else{
			f_lseek(&fdst,fdst.fsize);                    //移动指针
	    //res = f_write(&fdst, ADC_Capture, sizeof(ADC_Capture), &bw);               /* Write it(data) to the dst file */
			res = f_write(&fdst, ADC_Capture_Data, sizeof(ADC_Capture_Data), &bw);
				if(res == FR_OK){
				//printf("write data ok! %d\n\r",bw);//调试发送电脑端查看
			}else{
				//printf("write data error : %d\n\r",res);//调试发送电脑端查看
			}
			/*close file */
			f_close(&fdst);
	}
			f_close(&fdst);
		
		
	}
	
////////////////////////////////////////////////////////////////	
void readtext(char i,char j, char file_dir[])//默认字体大小是16
{
	  FIL  fsrc;
		char buffer[4096];   /* file copy buffer */
		char buf[14];
	  FRESULT res;
   //读文件测试
	  //printf("read file test......\n\r");//调试发送电脑端查看
    res = f_open(&fsrc, file_dir, FA_OPEN_EXISTING | FA_READ);
    if(res != FR_OK){
		//printf("open file error : %d\n\r",res);//调试发送电脑端查看
		}else{
			//f_lseek(&fsrc,fsrc.fsize-sizeof(textFileBuffer));   
	    res = f_read(&fsrc, buffer, fsrc.fsize, &br);     /* Read a chunk of src file */
		if(res==FR_OK){
			//printf("read data num : %d\n\r",br);//调试发送电脑端查看
			//printf("%s\n\r",buffer);//调试发送电脑端查看
	/////////////////////
			//LCD_Clear(WHITE);
		//	 LCD_ShowString(i,j,"                              ");
			//sscanf((u8*)buffer, "%*[^G]/%[^G]", buf); 
			strcpy( buf,(u8*)buffer+strlen((u8*)buffer)-180);
	    Show_Str(i,j,(u8*)buf,16,1);
	//////////////////////
		}else{
			//printf("read file error : %d\n\r",res);//调试发送电脑端查看
		}
		/*close file */
		 f_close(&fsrc);
	}

	
}	
const char filedir[]="0:/PICTURE";
const char filedir1[]="0:/BKPICTURE";
	///////////////////////////////////////
 int main(void)
 {
	 /////////AD////////////  
	 unsigned char S_i;
	 unsigned char t,len;
	 unsigned long  AD_num,temp_AD_num;//数据采集的变量
	 u8 point_y;
	// unsigned char s[10];
    char pan[]="0:/longfilenametest0.txt";
	 //////////////////////
		FATFS fs;
		SystemInit();
		delay_init(72);	     //延时初始化
		NVIC_Configuration();
		uart_init(9600);//chuankou 2400
	 ///////////////////////
	 // Adc_Init();
	 	ADC_Capture_Data[12]='\r';
		ADC_Capture_Data[13]='\n';
	 //////////////////////

    
	 
		SPI_Flash_Init();	//SPI FLASH初始化
		LCD_Init();
	 	ADC24_Configtion();//数据采集引脚初始化配置
		//LCD_ShowString(60,50,"Mini STM32");	
		f_mount(0, &fs);//文件挂载
	  ////////////////////////////////////////
		LED_Init();
  	KEY_Init();	
		
		
/////////////SD卡更新/////////////////////////////		
//	while(font_init())
//	{
//    	POINT_COLOR=RED;      
//			LCD_Clear(WHITE);
//			LCD_ShowString(60,50,"STM32 waiting");	
//			LCD_ShowString(60,70,"Font Updating..."); 	 
//			while(update_font())//从SD卡更新
//				{   
//						LCD_ShowString(60,90,"FAT SYS ERROR.      ");	 
//						delay_ms(200);     
//						LED0=!LED0;	
//				}
//	}
////////////////////////////////////////////////////////////
    

	////////////////////////



LCD_ShowString(60,64,"Pleas Watting ......");	 
////////////////变量初始化///////////////////////////////////////
    AD_num=0;len=32;
		for(S_i=10;S_i>0;S_i--)
			{
					ADC_Capture_Data[S_i-1]='0';			
			}
		f_unlink(pan);//删除现有目录	
////////////////////////////////////////////////////////////////		
//去初值			
	temp_AD_num=0;
			viewPictures(filedir1);	//一级目录浏览
for(S_i=0;S_i<10;S_i++)
{
		AD_num=averageValue(20);
			AD_num=AD_num-8650000;			
		  AD_num=AD_num*416;
			AD_num=(AD_num-466044782)/27335.42601;
			AD_num=AD_num*1.15647;
			temp_AD_num=temp_AD_num+AD_num;

}
temp_AD_num=temp_AD_num/10.0;		
			
			
////////////////////////////////////////////////////////
		LCD_ShowString(60,64,"Pleas Watting ......");	 
		delay_ms(1000);

//		delay_ms(1000);
		LCD_Clear(LIGHTGREEN);
		delay_ms(20);
		POINT_COLOR=RED;
		zuobiao();
		point_y=192;
///////////////////////////////////////////////////////

	while(1)
	{
		len++;
		if(len>240)
		{
				len=32;
				zuobiao();
		}
		
			/* =============  数据采集处理  ============ */			
			AD_num=averageValue(20);//Get_Adc(ADC_Channel_0);
			AD_num=AD_num-8650000;			
		  AD_num=AD_num*416;
			AD_num=(AD_num-466044782)/27335.42601;
			AD_num=AD_num*1.15647-temp_AD_num;
			LCD_ShowNum(0,220,AD_num,11,16);//将采集出的数据在LCD上显示
//			display_dot(len,184-AD_num/10,2);
			LCD_DrawLine(len-1, point_y, len,192-AD_num*0.6);
			point_y=192-AD_num*0.6;
		   for(S_i=11;S_i>0;S_i--)
					{
								ADC_Capture_Data[S_i-1]=(unsigned char)AD_num%10+0x30;
						    if(AD_num<1)
									ADC_Capture_Data[S_i-1]=0x30;
								AD_num=AD_num /10;
					}
					ADC_Capture_Data[11]='G';
//					if(ADC_Capture_Data[1]=='6')
//						if(ADC_Capture_Data[2]<'7')continue;
					printf("%s",ADC_Capture_Data);//将采集的数据串口发送到电脑。
					/* =============  数据采集处理 结束，其他的是SD卡和其他的学习班上功能配置，现在就是数据采集不对其他都正常了。 ============ */
	//		LCD_ShowNum(172,300,ADC_temp,3,16);
			//sprintf(ADC_Capture1,"%s%f%s","ADC_CH0_VOL:",ADC_temp,"V");	
			//LCD_ShowString(60,260,s);	
			
				
				
			writetext( pan);//写入数据
		 // readtext(0,0,"0:/longfilenametest0.txt");
			//delay_ms(250);
//		if(KEY_Scan()==1)
//		{	
//			zuobiao();	
//			LCD_Clear(LIGHTGREEN);			
//			while(1)
//			{		
//					readtext(0,0,"0:/longfilenametest0.txt");
//					//delay_ms(250);
//					if(KEY_Scan()==2)break;					
//			}
//					zuobiao();
//		}
//		
//		if(KEY_Scan()==2)
//		{			
//					//delay_ms(10);
//					viewPictures(filedir);	//一级目录浏览
//			
//					LCD_Clear(LIGHTGREEN);
//					zuobiao();		
//		}
		LED0=!LED0; 
								
	}
	
 }
 
void viewPictures(const char *fileDir){
    DIR dir;
	BOOL result;
	FRESULT res;
	FILINFO fileInfo;
	char *filename,kk,k=2;
	char  temp[256];
#if _USE_LFN
    static char lfn[_MAX_LFN + 1];
    fileInfo.lfname = lfn;
    fileInfo.lfsize = sizeof(lfn);
#endif
	delay_ms(300);
	LCD_Clear(BRRED);
 	while(k--)
	{
  	   res= f_opendir(&dir,  fileDir);
	   if(res==FR_OK)
       {  	
	     for(kk=0;kk<7;kk++)
		 {
            res =f_readdir(&dir,&fileInfo);
	        if(res!=0||fileInfo.fname[0]==0)break;
	        if(fileInfo.fname[0]=='.')continue  ;
#if _USE_LFN
            filename = *fileInfo.lfname ? fileInfo.lfname : fileInfo.fname;
#else
            filename = fileInfo.fname;
#endif
            if ( !(fileInfo.fattrib & AM_DIR) ) 
		    {
 		       strcpy(temp,fileDir);
			   strcat(temp,"/");
			   LCD_Clear(WHITE);//清屏,加载下一幅图片的时候,一定清屏
	           result=AI_LoadPicFile(((u8*)strcat(temp,filename)),0,0,320,240);//横屏和竖屏需要设置24/320
	           if(result==FALSE) continue;
		       POINT_COLOR=RED;
		       Show_Str(0,0,(u8 *)filename,16,1);//显示图片名字
	           delay_ms(200); 
	        }
//				LCD_Clear(LIGHTGREEN);	
	      } 
	   }	 
	 }
	k=7;
  }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void zuobiao(void)
{
	LCD_Clear(LIGHTGREEN);
	POINT_COLOR=RED;
	//y轴
Show_Str(32,32,"|",16,1);Show_Str(32,24,"-",16,1);Show_Str(0,24," 300",16,1);	
Show_Str(32,48,"|",16,1);Show_Str(32,40,"-",16,1);Show_Str(0,40," 270",16,1);
Show_Str(32,64,"|",16,1);Show_Str(32,56,"-",16,1);Show_Str(0,56," 240",16,1);	
Show_Str(32,80,"|",16,1); Show_Str(32,72,"-",16,1);Show_Str(0,72," 210",16,1);
Show_Str(32,96,"|",16,1);Show_Str(32,88,"-",16,1);Show_Str(0,88," 180",16,1);														
Show_Str(32,112,"|",16,1);Show_Str(32,104,"-",16,1);Show_Str(0,104," 150",16,1);
Show_Str(32,128,"|",16,1);Show_Str(32,120,"-",16,1);Show_Str(0,120," 120",16,1);
Show_Str(32,136,"|",16,1); 
Show_Str(32,144,"|",16,1);Show_Str(32,136,"-",16,1);Show_Str(0,136," 90",16,1);
Show_Str(32,152,"|",16,1);
Show_Str(32,160,"|",16,1);Show_Str(32,152,"-",16,1);Show_Str(0,152," 60",16,1);
Show_Str(32,168,"|",16,1); 
Show_Str(32,176,"|",16,1);Show_Str(32,168,"-",16,1);Show_Str(0,168," 30",16,1);
Show_Str(32,184,"|",16,1);
Show_Str(32,192,"|",16,1); Show_Str(32,184,"-",16,1);Show_Str(0,192,"  0",16,1);
Show_Str(32,16,"|",16,1);Show_Str(32,16,"A",16,1);Show_Str(0,8,"  N",16,1);
	//x轴
Show_Str(32,184,"-",16,1);Show_Str(40,184,"-",16,1);Show_Str(40,184,"|",16,1);Show_Str(40,184,"|",16,1);
Show_Str(48,184,"-",16,1);Show_Str(56,184,"-",16,1);Show_Str(56,184,"|",16,1);
Show_Str(64,184,"-",16,1);Show_Str(72,184,"-",16,1);Show_Str(72,184,"|",16,1);
Show_Str(80,184,"-",16,1);Show_Str(88,184,"-",16,1);Show_Str(88,184,"|",16,1);
Show_Str(96,184,"-",16,1);Show_Str(104,184,"-",16,1);Show_Str(104,184,"|",16,1);
Show_Str(112,184,"-",16,1);Show_Str(120,184,"-",16,1);Show_Str(120,184,"|",16,1);
Show_Str(128,184,"-",16,1);Show_Str(136,184,"-",16,1);Show_Str(136,184,"|",16,1);
Show_Str(144,184,"-",16,1);Show_Str(152,184,"-",16,1);Show_Str(152,184,"|",16,1);
Show_Str(160,184,"-",16,1);Show_Str(168,184,"-",16,1);Show_Str(168,184,"|",16,1);
Show_Str(176,184,"-",16,1);Show_Str(184,184,"-",16,1);Show_Str(184,184,"|",16,1);
Show_Str(192,184,"-",16,1);Show_Str(200,184,"-",16,1);Show_Str(200,184,"|",16,1);
Show_Str(208,184,"-",16,1);Show_Str(216,184,"-",16,1);Show_Str(216,184,"|",16,1);
Show_Str(224,184,"-",16,1);Show_Str(232,184,"-",16,1);//Show_Str(232,184,"|",16,1);
Show_Str(240,184,"-",16,1);Show_Str(248,184,"-",16,1);//Show_Str(248,184,"|",16,1);
Show_Str(256,184,"-",16,1);Show_Str(264,184,"-",16,1);//Show_Str(264,184,"|",16,1);
Show_Str(272,184,"-",16,1);Show_Str(280,184,"-",16,1);//Show_Str(280,184,"|",16,1);
Show_Str(288,184,"-",16,1);Show_Str(296,184,"-",16,1);//Show_Str(296,184,"|",16,1);
Show_Str(304,184,"-",16,1);Show_Str(312,184,"-",16,1);//Show_Str(312,184,"|",16,1);
Show_Str(320,184,"-",16,1);Show_Str(240,184,">",16,1);Show_Str(240,200,"t",16,1);

//Show_Str(0,254,"-",16,1);
//Show_Str(8,254,"-",16,1);
//Show_Str(16,254,"-",16,1);
//Show_Str(24,254,"-",16,1);
//Show_Str(32,254,"-",16,1);
//Show_Str(40,254,"-",16,1);
//Show_Str(48,254,"-",16,1);
//Show_Str(56,254,"-",16,1);
//Show_Str(64,254,"-",16,1);
//Show_Str(72,254,"-",16,1);
//Show_Str(80,254,"-",16,1);
//Show_Str(88,254,"-",16,1);
//Show_Str(96,254,"-",16,1);
		Show_Str(200,220,"ADC24",16,1);				    	 			    	 
		Show_Str(270,220,"2015",16,0);  
		delay_ms(10);
		POINT_COLOR=BLUE;//设置字体为蓝色
//		LCD_ShowString(60,260,"ADC_CH0_VAL:");	      
		LCD_ShowString(130,220,"Pertor");	
		LCD_ShowString(90,220,"N");
}



 

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
