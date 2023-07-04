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
    DS18B20_ConvertT();		//上电先转换一次温度，防止第一次读数据错误
    Delay(1000);			//等待转换完成
    data_init();
	
    LCD_init();
	
    LCD_ShowString(1,1,"T:");
    LCD_ShowString(2,1,"H:");
    LCD_ShowString(1,10,"MAX:");
    LCD_ShowString(2,10,"MIN:");
    
    while(1) {
        show();
        KeyNum = mkey();
        //未进入设置
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
				//进入设置
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
    DS18B20_ConvertT();	//转换温度
    T=DS18B20_ReadT();	//读取温度
    if(T<0)				//如果温度小于0
    {
        LCD_ShowChar(1,3,'-');	//显示负号
        T=-T;			//将温度变为正数
    }
    else				//如果温度大于等于0
    {
        LCD_ShowChar(1,3,'+');	//显示正号
    }
    LCD_ShowNum(1,4,T,2);		//显示温度整数部分
    LCD_ShowChar(1,6,'.');		//显示小数点
    LCD_ShowNum(1,7,(unsigned long)(T*10000)%10000,3);//显示温度小数部分
    //

    LCD_ShowNum(1,15,MAX,2);
    LCD_ShowNum(2,15,MIN,2);
}
