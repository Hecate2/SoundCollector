#include <stc15fxxxx.h> //����stc15fϵ��ͷ�ļ�,���ص�ַ: http://pan.baidu.com/s/1eRUbjLS

//���ڲ���-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/*************��������****************/


#define MAIN_Fosc 22118400L //��ʱ��,����Ƶ�ʲ�ͬ��ֱ���������޸�Ƶ��
#define serial_one_read_max 32 //���建�����ĳ���
#define serial_one_baud_rate 115200L //������,ֻ���������޸Ĳ����ʾͿ�����
#define Timer1_Reload (65536UL -(MAIN_Fosc / 4 / serial_one_baud_rate))  //�ö�ʱ��1�������ʷ����������㶨ʱ��1����װֵ 


/*************��������****************/
u8 serial_one_read_data[serial_one_read_max]; //���崮�ڽ������ݻ�����
u8 serial_one_read_count=0; //�������ݵĸ���


//��ʼ������1
void serial_one_init()
{
SCON |= 0x40; //8λ����
P_SW1 &= ~0xc0; //UART1 ʹ��P30 P31�� Ĭ��
TR1 = 0; //�رն�ʱ��
AUXR &= ~0x01; //S1 BRT Use Timer1;
TMOD &= ~(1<<6); //Timer1 set As Timer
TMOD &= ~0x30; //Timer1_16bitAutoReload;2
AUXR |= (1<<6); //Timer1 set as 1T mode
TH1 = (u8)(Timer1_Reload >> 8);
TL1 = (u8)Timer1_Reload;
TR1 = 1; //�򿪶�ʱ��
PS = 1; //�����ȼ��ж�
REN = 1; //�������
ES = 1; //�����ж�


EA = 1; //����ȫ���ж�
}


//��ʱ10����
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


//����1����һ���ֽ�
void serial_one_send_byte(u8 dat)
{
SBUF = dat;//��������
while(!TI);//�ȴ��������
TI=0; //������ͱ�־
}


//����1�����ַ���
void serial_one_send_string(u8 *dat)
{
while(*dat)
serial_one_send_byte(*dat++);
}


//����1����ָ��ָ����������
void serial_one_send_length_string(u8 *dat,u8 length)
{
u8 i=0;
for(;i < length;i++)
serial_one_send_byte(dat[i]);
}

/********************* ����1�жϺ���************************/
void UART1_int (void) interrupt UART1_VECTOR
{
if(RI)
{
RI = 0;//������ܱ�־
serial_one_read_data[serial_one_read_count] = SBUF;//�����ݴ�������
if(++serial_one_read_count > serial_one_read_max) //�������
serial_one_read_count = 0;
}


// if(TI)
// {
// }
}


//ADC����----------------------------------------------------------------------------------------------------------------------------------------------------

//#include "reg51.h"
//#include "intrins.h"

#define FOSC    11059200L
#define BAUD    9600

typedef unsigned char BYTE;
typedef unsigned int WORD;

#define     URMD    0           //0:ʹ�ö�ʱ��2��Ϊ�����ʷ�����
                                //1:ʹ�ö�ʱ��1��ģʽ0(16λ�Զ�����ģʽ)��Ϊ�����ʷ�����
                                //2:ʹ�ö�ʱ��1��ģʽ2(8λ�Զ�����ģʽ)��Ϊ�����ʷ�����

#define ADC_POWER   0x80            //ADC��Դ����λ
#define ADC_FLAG    0x10            //ADC��ɱ�־
#define ADC_START   0x08            //ADC��ʼ����λ
#define ADC_SPEEDLL 0x00            //540��ʱ��
#define ADC_SPEEDL  0x20            //360��ʱ��
#define ADC_SPEEDH  0x40            //180��ʱ��
#define ADC_SPEEDHH 0x60            //90��ʱ��

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

Delay10ms();//��ʱ10����
Delay10ms();//��ʱ10����
serial_one_init();//���ڳ�ʼ��

//if(serial_one_read_count > 0)//������ܵ�����
//{
//Delay10ms();//��ʱ10���룬�ȴ�����ȫ������
//serial_one_send_length_string(serial_one_read_data,serial_one_read_count);//���ͽ��յ�������
//serial_one_read_count = 0;//���ռ�����0
//}

    InitADC();                      //��ʼ��ADC
    while (1)
    {
        //ShowResult(0);              //��ʾͨ��0
        //ShowResult(1);              //��ʾͨ��1
        //ShowResult(2);              //��ʾͨ��2
        //ShowResult(3);              //��ʾͨ��3
        //ShowResult(4);              //��ʾͨ��4
        //ShowResult(5);              //��ʾͨ��5
        //ShowResult(6);              //��ʾͨ��6
        //ShowResult(7);              //��ʾͨ��7
			u8 dat=GetADCResult(7);
			serial_one_send_length_string(&dat,1);
    }
}

/*----------------------------
��ȡADC���
----------------------------*/
BYTE GetADCResult(BYTE ch)
{
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
    _nop_();                        //�ȴ�4��NOP
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//�ȴ�ADCת�����
    ADC_CONTR &= ~ADC_FLAG;         //Close ADC

    return ADC_RES;                 //����ADC���
}

/*----------------------------
��ʼ��ADC
----------------------------*/
void InitADC()
{
    P1ASF = 0xff;                   //����P1��ΪAD��
    ADC_RES = 0;                    //�������Ĵ���
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
    Delay(2);                       //ADC�ϵ粢��ʱ
}


/*----------------------------
�����ʱ
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


