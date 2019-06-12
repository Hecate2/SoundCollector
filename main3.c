#include <stc15fxxxx.h> //调用stc15f系列头文件,下载地址: http://pan.baidu.com/s/1eRUbjLS

//串口部分-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/*************常量定义****************/


#define MAIN_Fosc 22118400L //主时钟,晶振频率不同的直接在这里修改频率
#define serial_one_read_max 32 //定义缓存区的长度
#define serial_one_baud_rate 115200L //波特率,只需在这里修改波特率就可以了
#define Timer1_Reload (65536UL -(MAIN_Fosc / 4 / serial_one_baud_rate))  //用定时器1做波特率发生器，计算定时器1的重装值 


/*************变量定义****************/
u8 serial_one_read_data[serial_one_read_max]; //定义串口接受数据缓存区
u8 serial_one_read_count=0; //接受数据的个数


//初始化串口1
void serial_one_init()
{
SCON |= 0x40; //8位数据
P_SW1 &= ~0xc0; //UART1 使用P30 P31口 默认
TR1 = 0; //关闭定时器
AUXR &= ~0x01; //S1 BRT Use Timer1;
TMOD &= ~(1<<6); //Timer1 set As Timer
TMOD &= ~0x30; //Timer1_16bitAutoReload;2
AUXR |= (1<<6); //Timer1 set as 1T mode
TH1 = (u8)(Timer1_Reload >> 8);
TL1 = (u8)Timer1_Reload;
TR1 = 1; //打开定时器
PS = 1; //高优先级中断
REN = 1; //允许接收
ES = 1; //允许中断


EA = 1; //允许全局中断
}


//延时10毫秒
void Delay10ms() //@22.1184MHz
{
unsigned char i, j, k;


_nop_();
_nop_();
i = 1;
j = 216;
k = 35;
do
{
do
{
while (--k);
} while (--j);
} while (--i);
}


//串口1发送一个字节
void serial_one_send_byte(u8 dat)
{
SBUF = dat;//发送数据
while(!TI);//等待发送完毕
TI=0; //清除发送标志
}


//串口1发送字符串
void serial_one_send_string(u8 *dat)
{
while(*dat)
serial_one_send_byte(*dat++);
}


//串口1发送指定指定长度数据
void serial_one_send_length_string(u8 *dat,u8 length)
{
u8 i=0;
for(;i < length;i++)
serial_one_send_byte(dat[i]);
}

/********************* 串口1中断函数************************/
void UART1_int (void) interrupt UART1_VECTOR
{
if(RI)
{
RI = 0;//清除接受标志
serial_one_read_data[serial_one_read_count] = SBUF;//将数据存入数组
if(++serial_one_read_count > serial_one_read_max) //避免溢出
serial_one_read_count = 0;
}


// if(TI)
// {
// }
}


//ADC部分----------------------------------------------------------------------------------------------------------------------------------------------------

//#include "reg51.h"
//#include "intrins.h"

#define FOSC    11059200L
#define BAUD    9600

typedef unsigned char BYTE;
typedef unsigned int WORD;

#define     URMD    0           //0:使用定时器2作为波特率发生器
                                //1:使用定时器1的模式0(16位自动重载模式)作为波特率发生器
                                //2:使用定时器1的模式2(8位自动重载模式)作为波特率发生器

#define ADC_POWER   0x80            //ADC电源控制位
#define ADC_FLAG    0x10            //ADC完成标志
#define ADC_START   0x08            //ADC起始控制位
#define ADC_SPEEDLL 0x00            //540个时钟
#define ADC_SPEEDL  0x20            //360个时钟
#define ADC_SPEEDH  0x40            //180个时钟
#define ADC_SPEEDHH 0x60            //90个时钟

void InitUart();
void InitADC();
void SendData(BYTE dat);
BYTE GetADCResult(BYTE ch);
void Delay(WORD n);
void ShowResult(BYTE ch);

void main()
{
    P0M0 = 0x00;
    P0M1 = 0x00;
    P1M0 = 0x00;
    P1M1 = 0x00;
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P4M0 = 0x00;
    P4M1 = 0x00;
    P5M0 = 0x00;
    P5M1 = 0x00;
    P6M0 = 0x00;
    P6M1 = 0x00;
    P7M0 = 0x00;
    P7M1 = 0x00;

Delay10ms();//延时10毫秒
Delay10ms();//延时10毫秒
serial_one_init();//串口初始化

//if(serial_one_read_count > 0)//如果接受到数据
//{
//Delay10ms();//延时10毫秒，等待数据全部读入
//serial_one_send_length_string(serial_one_read_data,serial_one_read_count);//发送接收到的数据
//serial_one_read_count = 0;//接收计数清0
//}

    InitADC();                      //初始化ADC
    while (1)
    {
        //ShowResult(0);              //显示通道0
        //ShowResult(1);              //显示通道1
        //ShowResult(2);              //显示通道2
        //ShowResult(3);              //显示通道3
        //ShowResult(4);              //显示通道4
        //ShowResult(5);              //显示通道5
        //ShowResult(6);              //显示通道6
        //ShowResult(7);              //显示通道7
			u8 dat=GetADCResult(7);
			serial_one_send_length_string(&dat,1);
    }
}

/*----------------------------
读取ADC结果
----------------------------*/
BYTE GetADCResult(BYTE ch)
{
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
    _nop_();                        //等待4个NOP
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//等待ADC转换完成
    ADC_CONTR &= ~ADC_FLAG;         //Close ADC

    return ADC_RES;                 //返回ADC结果
}

/*----------------------------
初始化ADC
----------------------------*/
void InitADC()
{
    P1ASF = 0xff;                   //设置P1口为AD口
    ADC_RES = 0;                    //清除结果寄存器
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
    Delay(2);                       //ADC上电并延时
}


/*----------------------------
软件延时
----------------------------*/
void Delay(WORD n)
{
    WORD x;

    while (n--)
    {
        x = 5000;
        while (x--);
    }
}


