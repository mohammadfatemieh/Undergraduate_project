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
//Mini STM32�����巶������25
//ͼƬ��ʾ ʵ��
//����ԭ��@ALIENTEK
//������̳:www.openedv.com

/******************************************************
���׷���ļ�����Ҫ�ƶ��ļ�ָ�룬f_lseek�����������ô��ˡ��˺��������ļ�ָ����ƶ���
����������������һ������Ϊ�ļ����󣬵ڶ�������Ϊ�ƶ����ֽ�����
��������F_LSEEK(&fsrc��fsrc.fsize)ָ���ļ���β������׷���ļ���
ע��ڶ���������������ļ��Ŀ�ʼ����F_LSEEK(&fsrc��3000)����ʾ�ƶ����ļ���ͷ3000���ֽڴ���
���ʹ�ÿ��Է���Ĳ����ļ���
******************************************************/
/////////////////////////////////////////////////////////////////
uint8_t textFileBuffer[] = "��Ӣ�Ĳ����ַ��� \r\nChinese and English test strings \r\n";
	char ADC_Capture[]="��Ӣ�Ĳ����ַ���1\r\nChinese and English test strings1 \r\n";
	char ADC_Capture_Data[14];

void writetext(char file_dir[])
	{
			FRESULT res;
			FIL  fdst;
	/////////////д�ļ�///////////////////
			res = f_open(&fdst, file_dir, FA_OPEN_ALWAYS | FA_WRITE);

			if(res != FR_OK){
			//printf("open file error : %d\n\r",res);//���Է��͵��Զ˲鿴
			}else{
			f_lseek(&fdst,fdst.fsize);                    //�ƶ�ָ��
	    //res = f_write(&fdst, ADC_Capture, sizeof(ADC_Capture), &bw);               /* Write it(data) to the dst file */
			res = f_write(&fdst, ADC_Capture_Data, sizeof(ADC_Capture_Data), &bw);
				if(res == FR_OK){
				//printf("write data ok! %d\n\r",bw);//���Է��͵��Զ˲鿴
			}else{
				//printf("write data error : %d\n\r",res);//���Է��͵��Զ˲鿴
			}
			/*close file */
			f_close(&fdst);
	}
			f_close(&fdst);
		
		
	}
	
////////////////////////////////////////////////////////////////	
void readtext(char i,char j, char file_dir[])//Ĭ�������С��16
{
	  FIL  fsrc;
		char buffer[4096];   /* file copy buffer */
	  FRESULT res;
   //���ļ�����
	  //printf("read file test......\n\r");//���Է��͵��Զ˲鿴
    res = f_open(&fsrc, file_dir, FA_OPEN_EXISTING | FA_READ);
    if(res != FR_OK){
		//printf("open file error : %d\n\r",res);//���Է��͵��Զ˲鿴
		}else{
			//f_lseek(&fsrc,fsrc.fsize-sizeof(textFileBuffer));   
	    res = f_read(&fsrc, buffer, fsrc.fsize, &br);     /* Read a chunk of src file */
		if(res==FR_OK){
			//printf("read data num : %d\n\r",br);//���Է��͵��Զ˲鿴
			//printf("%s\n\r",buffer);//���Է��͵��Զ˲鿴
	/////////////////////
			//LCD_Clear(WHITE);
			 LCD_ShowString(i,j,"                              ");
	    Show_Str(i,j,(u8*)buffer,16,1);
	//////////////////////
		}else{
			//printf("read file error : %d\n\r",res);//���Է��͵��Զ˲鿴
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
	 unsigned long  AD_num;//���ݲɼ��ı���
	// unsigned char s[10];
    char pan[]="0:/longfilenametest0.txt";
	 //////////////////////
		FATFS fs;
		SystemInit();
		delay_init(72);	     //��ʱ��ʼ��
		NVIC_Configuration();
		uart_init(9600);
	 ///////////////////////
	 // Adc_Init();
	 	ADC_Capture_Data[12]='\r';
		ADC_Capture_Data[13]='\n';
	 //////////////////////
		LED_Init();
  	KEY_Init();	
    
	 
		SPI_Flash_Init();	//SPI FLASH��ʼ��
		LCD_Init();
	 	ADC24_Configtion();//���ݲɼ����ų�ʼ������
		LCD_ShowString(60,50,"Mini STM32");	
		f_mount(0, &fs);//�ļ�����
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
			while(update_font())//��SD������
				{   
						LCD_ShowString(60,90,"FAT SYS ERROR.      ");	 
						delay_ms(200);     
						LED0=!LED0;	
				}
	}

	  LCD_Clear(LIGHTGREEN);
		POINT_COLOR=RED;      
		Show_Str(60,220,"ADC24",16,0);				    	 			    	 
		Show_Str(60,200,"2014 �� 09 �� 27 ��",16,0);  
		delay_ms(1000);
	////////////////////////
		POINT_COLOR=BLUE;//��������Ϊ��ɫ
		LCD_ShowString(60,260,"ADC_CH0_VAL:");	      
		LCD_ShowString(60,280,"ADC_CH0_VOL:0.000V");	
    AD_num=0;
	
	f_unlink(pan);//ɾ������Ŀ¼	
///////////////////////////////////////////////////
	while(1)
	{
		
			/* =============  ���ݲɼ�����  ============ */			
			AD_num=averageValue(20);//Get_Adc(ADC_Channel_0);
			AD_num=AD_num-8650000;			
		  AD_num=AD_num*416;
			AD_num=(AD_num-466044782)/27335.42601;
			LCD_ShowNum(0,300,AD_num,11,16);//���ɼ�����������LCD����ʾ
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
					printf("%s",ADC_Capture_Data);//���ɼ������ݴ��ڷ��͵����ԡ�
					/* =============  ���ݲɼ����� ��������������SD����������ѧϰ���Ϲ������ã����ھ������ݲɼ����������������ˡ� ============ */
	//		LCD_ShowNum(172,300,ADC_temp,3,16);
			//sprintf(ADC_Capture1,"%s%f%s","ADC_CH0_VOL:",ADC_temp,"V");	
			//LCD_ShowString(60,260,s);	
			
			LED0=!LED0; 	
				
			writetext( pan);//д������
		  readtext(0,0,"0:/longfilenametest0.txt");
		//	delay_ms(250);
	}
	
 }
