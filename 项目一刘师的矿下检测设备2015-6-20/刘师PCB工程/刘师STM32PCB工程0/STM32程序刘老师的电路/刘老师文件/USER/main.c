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
#include "integer.h"
#include "diskio.h"
#include "text.h"
//#include "24cxx.h"
#include "fontupd.h"
#include "stdio.h"
#include "string.h"
#include "picdecoder.h"
#include "adc.h"
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
			 LCD_ShowString(i,j,"                              ");
	    Show_Str(i,j,(u8*)buffer,16,1);
	//////////////////////
		}else{
			//printf("read file error : %d\n\r",res);//调试发送电脑端查看
		}
		/*close file */
		 f_close(&fsrc);
	}

	
}	

	///////////////////////////////////////
 int main(void)
 {
	 /////////AD////////////  
	 unsigned char S_i;
	 unsigned long  AD_num;//数据采集的变量
	// unsigned char s[10];
    char pan[]="0:/longfilenametest0.txt";
	 //////////////////////
		FATFS fs;
		SystemInit();
		delay_init(72);	     //延时初始化
		NVIC_Configuration();
		uart_init(9600);
	 ///////////////////////
	 // Adc_Init();
	 	ADC_Capture_Data[12]='\r';
		ADC_Capture_Data[13]='\n';
	 //////////////////////
		LED_Init();
  	KEY_Init();	
    
	 
		SPI_Flash_Init();	//SPI FLASH初始化
		LCD_Init();
	 	ADC24_Configtion();//数据采集引脚初始化配置
		LCD_ShowString(60,50,"Mini STM32");	
		f_mount(0, &fs);//文件挂载
	  ////////////////////////////////////////
		for(S_i=10;S_i>0;S_i--)
		{
				ADC_Capture_Data[S_i-1]='0';
		
		}
		
		
		//////////////////////////////////////////
		
		
	while(font_init())
	{
    	POINT_COLOR=RED;      
			LCD_Clear(WHITE);
			LCD_ShowString(60,50,"Mini STM32");	
			LCD_ShowString(60,70,"Font Updating..."); 	 
			while(update_font())//从SD卡更新
				{   
						LCD_ShowString(60,90,"FAT SYS ERROR.      ");	 
						delay_ms(200);     
						LED0=!LED0;	
				}
	}

	  LCD_Clear(LIGHTGREEN);
		POINT_COLOR=RED;      
		Show_Str(60,220,"ADC24",16,0);				    	 			    	 
		Show_Str(60,200,"2014 年 09 月 27 日",16,0);  
		delay_ms(1000);
	////////////////////////
		POINT_COLOR=BLUE;//设置字体为蓝色
		LCD_ShowString(60,260,"ADC_CH0_VAL:");	      
		LCD_ShowString(60,280,"ADC_CH0_VOL:0.000V");	
    AD_num=0;
	
	f_unlink(pan);//删除现有目录	
///////////////////////////////////////////////////
	while(1)
	{
		
			/* =============  数据采集处理  ============ */			
			AD_num=averageValue(20);//Get_Adc(ADC_Channel_0);
			AD_num=AD_num-8650000;			
		  AD_num=AD_num*416;
			AD_num=(AD_num-466044782)/27335.42601;
			LCD_ShowNum(0,300,AD_num,11,16);//将采集出的数据在LCD上显示
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
			
			LED0=!LED0; 	
				
			writetext( pan);//写入数据
		  readtext(0,0,"0:/longfilenametest0.txt");
		//	delay_ms(250);
	}
	
 }
