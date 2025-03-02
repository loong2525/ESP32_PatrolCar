/*这个代码用于测试电机连接及相关硬件的正常使用
*/

#include<Arduino.h>

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


const int LED=26;

#define D01 32//左1
#define D02 35//左2
#define D03 34//中间
#define D04 39//右2
#define D05 36//右1
//HIGH 为寻到黑线；LOW为未寻到黑线


/*电驱控制函数  
*mode:正反转；mode==1,则正转；mode==2，则反转；
*pwm：占空比调速度；
*side：选择1，2号电驱，1控制左轮，2控制右轮
*motor：控制一组电桥，1为前轮，2为后轮，0为停止
*/
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
void Stop(){move(0,0,0,0);}//制动


void line ()
{ 
  Forward();
  int D2=digitalRead(D02);
  int D4=digitalRead(D04);
  int D3=digitalRead(D03);
 if(D2==HIGH && D4==LOW $$ D3==LOW)//右偏纠偏
 {
  Stop;
  while(D3==LOW)
  {
    Anti();
    D3=digitalRead(D03);
  }
  Stop();
  Forward();
 }
 else if(D4==HIGH && D2==LOW && D3==LOW)//左偏纠偏
 {
  Stop();
  while(D3==LOW)
  {
    Clock();
    D3=digitalRead(D03);
  }
  Stop();
  Forward()
 }
 else if (D2==LOW && D3==LOW && D4==LOW)//脱离地图，亮灯停车
 {
  digitalWrite(reminder,HIGH);
  Stop;
  delay(5000);
 }
 
};


void setup()
{
  Serial.begin(115200);
  pinMode(PWM,OUTPUT); 
	pinMode(AIN1_left,OUTPUT);
	pinMode(AIN2_left,OUTPUT);
	pinMode(BIN1_left,OUTPUT);
	pinMode(BIN1_left,OUTPUT);
	pinMode(AIN1_round,OUTPUT);
  pinMode(BIN2_round,OUTPUT);
  pinMode(AIN1_round,OUTPUT);
  pinMode(BIN2_round,OUTPUT);
	pinMode(STBY,OUTPUT);


  pinMode(LED,OUTPUT);
}
void loop()
{
  Forward();
  delay(3000);
  Serial.println("前进");

  Backward();
  delay(3000);
  
  digitalWrite(LED,LOW);
  Serial.println("后退");
  Left();
  delay(3000);
  Serial.println("向左");
  Round();
  delay(3000);
  Serial.println("向右");
  Clock();
  delay(3000);
  Serial.println("顺时针");
  Anti();
  delay(3000);
  Serial.println("逆时针");
  Stop();
  delay(3000);
  line();
}