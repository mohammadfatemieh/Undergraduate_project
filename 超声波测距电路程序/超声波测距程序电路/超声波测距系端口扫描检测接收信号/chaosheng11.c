#include<AT89X51.H>
#include<intrins.h>


sbit in=P3^2;//超声波接收到
sbit csb=P3^3;//40KHz超声波发射脚
sbit Wela=P2^7;
sbit Dula=P2^6;
sbit di=P0^7;

#define uchar unsigned char
#define uint unsigned int
#define nop _nop_()


uchar shumaguan_We[]=
{
	0xfe,//第一个数码管选中；
	0xfd,//第二个数码管选中
	0xfb,//第三个数码管选中
	0xf7,//第四个数码管选中
	0xef,//第五个数码管选中
	0xdf,//第六个数码管选中
	0xbf,//第七个数码管选中
	0x7f//第八个数码管选中
};
uchar shumaguan_Du[]={ 
0x3F, //"0"
0x06, //"1"
0x5B, //"2"
0x4F, //"3"
0x66, //"4"
0x6D, //"5"
0x7D, //"6"
0x07, //"7"
0x7F, //"8"
0x6F, //"9"
0x77, //"A"
0x7C, //"B"
0x39, //"C"
0x5E, //"D"
0x79, //"E"
0x71, //"F"
0x76, //"H"
0x38, //"L"
0x37, //"n"
0x3E, //"u"
0x73, //"P"
0x5C, //"o"
0x40, //"-"
0x00, //??
0x00 //???
};

//sbit in=P3^2;//超声波接收到
//sbit csb=P3^3;//40KHz超声波发射脚
//sbit Wele=P2^7;
//sbit Dule=P2^6;

//#define uchar unsigned char
//#define uint unsigned int
//#define nop _nop_()
/****************************/
void init(void);//???
void delay_nms(uint n);//??nms
void delay100us();//??us
void display(uint dat);//4????????,????
void tran(void);//???????
/***************************/
uint dis,H=100,L=20;
uchar flag=0,high_time,low_time,m=0;

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
init();
while(1) //???????
{
tran();//?????????
display(dis);//????
delay_nms(10);	
}
}
/**********************************
初始化
**********************************/
void init(void)
{
	P0=0XFF;
	P1=0XFF;
	P2=0XFF;
	P3=0XFF;
TMOD=0x01;//计时
TH0=0;
TL0=0; //
EA=1;
IT0=1;//
}
/**********************************
延时1
**********************************/
void delay_nms(uint n)
{
uchar i; 
while(n--)
{
for(i=123;i>0;i--);
}
}
/**********************************
数码管显示
**********************************/
void display(uint dat)
{
uchar i,j,k;//??????????
i=dat/100;//白
j=dat%100/10;//十位
k=dat%100%10;//个
	Wela=1;
	P0=shumaguan_We[0];
	Wela=0;
	Dula=1;
	P0=shumaguan_Du[i];
	Dula=0;
	delay_nms(2);

	Wela=1;
	P0=shumaguan_We[1];
	Wela=0;
	Dula=1;
	P0=~0X7f;//小数点
	Dula=0;

delay_nms(2);
//	di=1;
	Wela=1;
	P0=shumaguan_We[2];
	Wela=0;
	Dula=1;
	P0=shumaguan_Du[j];
	Dula=0;
	
delay_nms(2);
	Wela=1;
	P0=shumaguan_We[3];
	Wela=0;
	Dula=1;
	P0=shumaguan_Du[k];
	Dula=0;

delay_nms(2);

}
/**********************************
超声波发射
**********************************/
void tran(void)
{
uchar i;
float temp;
TH0=0;
TL0=0;//???
TR0=1;//???
	delay_nms(10);	
for(i=8;i>0;i--)
{
csb=!csb;
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
csb=1;
delay_nms(1);
EX0=1;//???
if(flag==1) //??????,?????
{ //???????
temp=high_time*256+low_time;
temp=(temp/1000)/2;
temp*=340;
temp=temp/10;
dis=(unsigned int)temp;
flag=0;
}
}
/**********************************
????:????
????:
????:?
???: ?
**********************************/
void TT() interrupt 0
{
uint tmp;
TR0=0;//????
ET0=0;//?????
flag=1; //?????
tmp=TH0*256+TL0; //???????
if((tmp>0)&&(tmp<60000))//????????,?????????,???????
{
high_time=TH0;//????????
low_time=TL0;

}
else //?????????
{
high_time=0;
low_time=0; 
} 
}
