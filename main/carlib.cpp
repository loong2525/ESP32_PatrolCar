


#include "carlib.h";

const int PWM=22;
const int STBY=11;

const int AIN1_left=6;
const int AIN2_left=7;
const int BIN1_left=9;
const int BIN2_left=10;
const int AIN1_round=13;
const int AIN2_round=11;
const int BIN1_round=10;
const int BIN2_round=8;




void move(int mode,int pwm,int side,int motor)
{
	boolean pin1=HIGH;
	boolean pin2=LOW;
  //定义高低电平

	if(mode!=1)//若要电机反转，更改电平值
	{
    pin1=LOW;
		pin2=HIGH;
	}
  digitalWrite(STBY,LOW);
	
  if(motor==0)//制动
  {
    digitalWrite(STBY,LOW);
  }

  else
  {
    digitalWrite(STBY,HIGH);
    if(side==1)
    {
      if(motor==1)//控制左前轮
      {
        digitalWrite(AIN1_left,pin1);
        digitalWrite(AIN2_left,pin2);
      }
      else//控制左后轮
      {
      digitalWrite(BIN1_left,pin1);
      digitalWrite(BIN2_left,pin2);
      }
    }
    else
    {
      if(motor==1)//右前轮
      {
        digitalWrite(AIN1_round,pin1);
        digitalWrite(AIN2_round,pin2);
      }
      else//右后轮
      {
        digitalWrite(BIN1_round,pin1);
        digitalWrite(BIN2_round,pin2);
      }
    }
  }
  analogWrite(PWM,pwm);
}


void Forward(){move(1,50,1,1);move(1,50,1,2);move(1,50,2,1);move(1,50,2,2);}//前进
void Backward(){move(2,50,1,1);move(2,50,1,2);move(2,50,2,1);move(2,50,2,2);}//后退
void Left(){move(2,50,1,1);move(1,50,1,2);move(1,50,2,1);move(2,50,2,2);}//左移
void Round(){move(1,50,1,1);move(2,50,1,2);move(2,50,2,1);move(1,50,2,2);}//右移
void Clock(){move(1,50,1,1);move(1,50,1,2);move(2,50,2,1);move(2,50,2,2);}//clockwise顺时针旋转
void Anti(){move(2,50,1,1);move(2,50,1,2);move(1,50,2,1);move(1,50,2,2);}//anticlockwise逆时针旋转
void Stop(){move(0,0,0,0);}