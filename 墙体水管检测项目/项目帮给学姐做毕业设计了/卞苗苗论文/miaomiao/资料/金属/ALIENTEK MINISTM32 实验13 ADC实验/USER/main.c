#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "usart.h"
#include "wdg.h"
#include "timer.h"
#include "pwm.h"
#include "lcd.h"


//ALIENTEK Mini STM32开发板范例代码13
//ADC 实验
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
 
 int main(void)
 {
	u16 adcx;
	u16 a[20];
	u16 val,t;
	float temp;
	SystemInit();
	delay_init(72);	     //延时初始化
	NVIC_Configuration();//-----中断配置函数
 	uart_init(9600);//串口初始化函数
 	LED_Init();
 	KEY_Init();
	LCD_Init();

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
		t=KEY_Scan();//得到键值
	   	if(t==1)
					{						   
			
							LCD_Clear(RED);
							POINT_COLOR=BLACK;//设置字体为红色 
							LCD_ShowString(60,150,"metal detected!");
							delay_ms(550);
							delay_ms(550);
					}
			else 
					{
							LCD_Clear(WHITE);
						POINT_COLOR=BLUE;//设置字体为红色 
							LCD_ShowString(60,150,"No metal!");
					}
					

		LED0=!LED0;
		delay_ms(550);
//		delay_ms(550);
	}

 }

