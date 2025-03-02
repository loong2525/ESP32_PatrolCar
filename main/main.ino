#include <Arduino.h>
#include "bluetooth.h"
#include "carlib.h"


#define D01 32//左1
#define D02 35//左2
#define D03 34//中间
#define D04 39//右2
#define D05 36//右1
//HIGH 为寻到黑线；LOW为未寻到黑线

#define button 15 //功能切换开关
#define reminder 26 //指示灯

void Definition();//定义esp32管脚模式
void line ();//确保车可以走直线


void setup() {
  // put your setup code here, to run once:
  void Definition();
  Serial.begin(115200);
  SerialBT.begin("ESP32_XX"); // 设置蓝牙名称
}

void loop() {
  // put your main code here, to run repeatedly:
  Wifiinput();
}


void Definition()
{
  //电驱管脚
	pinMode(PWM,OUTPUT);
	pinMode(AIN1_left,OUTPUT);
	pinMode(AIN2_left,OUTPUT);
	pinMode(BIN1_left,OUTPUT);
	pinMode(BIN2_left,OUTPUT);
	pinMode(AIN1_round,OUTPUT);
	pinMode(AIN2_round,OUTPUT);
  pinMode(STBY,OUTPUT);

  //循迹管脚
  pinMode(D01,INPUT);
  pinMode(D02,INPUT);
  pinMode(D03,INPUT);
  pinMode(D04,INPUT);
  pinMode(D05,INPUT);

  //
  pinMode(reminder,OUTPUT);
  digitalWrite (reminder,LOW);
  pinMode(button,INPUT);
 
}
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
