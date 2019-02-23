#include"AD24.h"

void ADC24_Configtion(void)
{
			GPIO_InitTypeDef GPIO_InitStruct;
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	/*          PIN1=ADDO-----in        */
			GPIO_InitStruct.GPIO_Pin=	GPIO_Pin_4;
			GPIO_InitStruct.GPIO_Speed=	GPIO_Speed_50MHz	;
			GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPD;//浮空输入GPIO_Mode_IN_FLOATING
			GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	/*  PIN2=ADSK ----out*/
			GPIO_InitStruct.GPIO_Pin=	GPIO_Pin_5;
			GPIO_InitStruct.GPIO_Speed=	GPIO_Speed_50MHz	;
			GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
			GPIO_Init(GPIOC,&GPIO_InitStruct);
			
}


/*=============================================
	AD24位数值提取函数
	一般提取6、7、8位数据
=============================================*/
//#define ADD0_R GPIO_ResetBits(GPIOA,GPIO_Pin_1)
//#define ADD0_S GPIO_SetBits(GPIOA,GPIO_Pin_1)
//#define ADSK_R GPIO_ResetBits(GPIOA,GPIO_Pin_2)
//#define ADSK_S GPIO_SetBits(GPIOA,GPIO_Pin_2)

unsigned long ADC_ReadCount(void)
{
		 unsigned char i;
			unsigned long Count;
			Count=0;
			GPIO_ResetBits(GPIOC,GPIO_Pin_5);//ADSK=0;		
			delay_us(20);
		while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)==1)// while(ADDO);
						delay_us(20);
		 for (i=0;i<24;i++)
				 {
				 GPIO_SetBits(GPIOC,GPIO_Pin_5);//ADSK=1;
					 			delay_us(20);
				 Count=Count<<1;
					GPIO_ResetBits(GPIOC,GPIO_Pin_5);//ADSK=0;
					 			delay_us(20);
				 if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)==1) Count++;
					 			delay_us(20);
				 }
		 GPIO_SetBits(GPIOC,GPIO_Pin_5);//ADSK=1;
				 			delay_us(20);
		 Count=Count^0x800000;//抑或
		 GPIO_ResetBits(GPIOC,GPIO_Pin_5);//ADSK=0;
				 			delay_us(20);
		 return(Count);
}


/*============================================
求平均值函数
===============================================*/
long averageValue(unsigned char times)
	{
		char i;
		long sum = 0;
	//	long averg=0;
		for (i = 0; i < times; i++)
			{
				sum += ADC_ReadCount();
			}
		return (sum / times);
	}
