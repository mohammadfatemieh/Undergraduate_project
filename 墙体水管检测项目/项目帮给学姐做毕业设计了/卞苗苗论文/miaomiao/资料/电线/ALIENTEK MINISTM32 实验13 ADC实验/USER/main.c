#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "usart.h"
#include "wdg.h"
#include "timer.h"
#include "pwm.h"
#include "lcd.h"
#include "adc.h"

//ALIENTEK Mini STM32开发板范例代码13
//ADC 实验
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
 
 int main(void)
 {
	u16 adcx;
	u16 a[20];
	u16 val;
	float temp;
	SystemInit();
	delay_init(72);	     //延时初始化
	NVIC_Configuration();//-----中断配置函数
 	uart_init(9600);//串口初始化函数
 	LED_Init();
 	KEY_Init();
	LCD_Init();
	Adc_Init();
	POINT_COLOR=RED;//设置字体为红色 
	LCD_ShowString(60,50,"Mini STM32");	
	LCD_ShowString(60,70,"ADC TEST");	
	LCD_ShowString(60,90,"ATOM@ALIENTEK");
	LCD_ShowString(60,110,"2010/12/30");	
	//显示提示信息
	POINT_COLOR=BLUE;//设置字体为蓝色
	LCD_ShowString(60,130,"ADC_CH0_VAL:");	      
	LCD_ShowString(60,150,"ADC_CH0_VOL:0.0000V");	      
	while(1)
	{
		u16 k,i;
		k=20;
		i=0;
		val=0;
	
		adcx=Get_Adc(ADC_Channel_1);//PA0  ADC1
		LCD_ShowNum(156,130,adcx,4,16);//显示ADC的值
		temp=(float)adcx*(3.3/4096);
		adcx=temp;
		LCD_ShowNum(156,150,adcx,1,16);//显示电压值
		temp-=adcx;
		temp*=1000;
		LCD_ShowNum(172,150,temp,4,16);

		if(temp>150)
		{
			LCD_Clear(RED);
			POINT_COLOR=BLACK;
			LCD_ShowString(60,50,"Mini STM32");	
			LCD_ShowString(60,70,"ADC TEST");	
			LCD_ShowString(60,90,"ATOM@ALIENTEK");
			LCD_ShowString(60,110,"2010/12/30");	
			//显示提示信息
			POINT_COLOR=BLUE;//设置字体为蓝色
			LCD_ShowString(60,130,"ADC_CH1_VAL:");	      
			LCD_ShowString(60,150,"ADC_CH1_VOL:0.0000V");	
			LCD_ShowNum(172,150,temp,4,16);
			LCD_ShowString(60,200,"There is wire!");
			delay_ms(550);
		}
		else
		{
			
			LCD_Clear(WHITE);
			POINT_COLOR=RED;//设置字体为红色 
			LCD_ShowString(60,50,"Mini STM32");	
			LCD_ShowString(60,70,"ADC TEST");	
			LCD_ShowString(60,90,"ATOM@ALIENTEK");
			LCD_ShowString(60,110,"2010/12/30");	
			//显示提示信息
			POINT_COLOR=BLUE;//设置字体为蓝色
			LCD_ShowString(60,130,"ADC_CH1_VAL:");	      
			LCD_ShowString(60,150,"ADC_CH1_VOL:0.0000V");	
			LCD_ShowNum(172,150,temp,4,16);
			LCD_ShowString(60,200,"There is no wire!");
			delay_ms(550);
		}
		LED0=!LED0;
		delay_ms(550);

	}

 }

