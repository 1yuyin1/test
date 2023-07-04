#include <REGX52.H>
#include "Delay.h"
#include "key.h"
#include "LCD.h"
#include "DS18B20.h"
#include "AT24C02.h"


float T = 0,H = 0;
unsigned int KeyNum = 0,MAX = 99,MIN = 20;
int flag = 0,select = 0;


void data_init();
void show();

void main()
{
    DS18B20_ConvertT();		//�ϵ���ת��һ���¶ȣ���ֹ��һ�ζ����ݴ���
    Delay(1000);			//�ȴ�ת�����
    data_init();
	
    LCD_init();
	
    LCD_ShowString(1,1,"T:");
    LCD_ShowString(2,1,"H:");
    LCD_ShowString(1,10,"MAX:");
    LCD_ShowString(2,10,"MIN:");
    
    while(1) {
        show();
        KeyNum = mkey();
        //δ��������
				if(flag == 0) {
            if(KeyNum==1) {

            }
            else if(KeyNum==2) {

            }
            else if(KeyNum==3) {
                flag = 1;
            }
            else if(KeyNum==4) {
                AT24C02_WriteByte(0,MAX%256);
                Delay(5);
                AT24C02_WriteByte(1,MAX/256);
                Delay(5);
                AT24C02_WriteByte(2,MIN%256);
                Delay(5);
                AT24C02_WriteByte(3,MIN/256);
                Delay(5);
            }
        }
				//��������
				else if(flag == 1) {
            if(KeyNum==1 && flag == 1) {
                if(select == 0) {
                    MAX++;
                } else if(select == -1) {
                    MIN++;
                }
            }
            else if(KeyNum==2 && flag == 1) {
                if(select == 0) {
                    MAX--;
                } else if(select == -1) {
                    MIN--;
                }
            }
            else if(KeyNum==3 && flag == 1) {
                flag = 0;
            }
            else if(KeyNum==4 && flag == 1) {
                select = ~select;
            }
        }
        
		}
}



void data_init() {
    MAX=AT24C02_ReadByte(0);
    MAX|=AT24C02_ReadByte(1)<<8;
    MIN=AT24C02_ReadByte(2);
    MIN|=AT24C02_ReadByte(3)<<8;
}
void show()
{
    DS18B20_ConvertT();	//ת���¶�
    T=DS18B20_ReadT();	//��ȡ�¶�
    if(T<0)				//����¶�С��0
    {
        LCD_ShowChar(1,3,'-');	//��ʾ����
        T=-T;			//���¶ȱ�Ϊ����
    }
    else				//����¶ȴ��ڵ���0
    {
        LCD_ShowChar(1,3,'+');	//��ʾ����
    }
    LCD_ShowNum(1,4,T,2);		//��ʾ�¶���������
    LCD_ShowChar(1,6,'.');		//��ʾС����
    LCD_ShowNum(1,7,(unsigned long)(T*10000)%10000,3);//��ʾ�¶�С������
    //

    LCD_ShowNum(1,15,MAX,2);
    LCD_ShowNum(2,15,MIN,2);
}