#include <Arduino.h>
#include <BluetoothSerial.h> 
//#include "bluetooth.h"
#include "carlib.h"


#define D01 32//左1
#define D02 35//左2
#define D03 34//中间
#define D04 39//右2
#define D05 36//右1
//HIGH 为寻到黑线；LOW为未寻到黑线

#define button 15 //功能切换开关
#define reminder 26 //指示灯

int x = 0;
int y = 0;

void Definition();//定义esp32管脚模式
void line ();//确保车可以走直线
void one();//功能一，单点循迹
void Wifiinput();
void ParseData(const String& data);



BluetoothSerial SerialBT;//初始化蓝牙对象

// 蓝牙目标坐标
int targetX = 0;
int targetY = 0;

int D2=digitalRead(D02);
int D4=digitalRead(D04);
int D3=digitalRead(D03);
int D1=digitalRead(D01);
int D5=digitalRead(D05);









void setup() {
  // put your setup code here, to run once:
  void Definition();
  Serial.begin(115200);
  SerialBT.begin("ESP32_XX"); // 设置蓝牙名称
}

void loop() {
  // put your main code here, to run repeatedly:
  Wifiinput();
  int Button = digitalRead(button);
  if(Button==LOW)
  {
    one();
  } 
  else
  {

  }
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
  D2=digitalRead(D02);
  D4=digitalRead(D04);
  D3=digitalRead(D03);
if(D2==HIGH && D4==LOW && D3==LOW)//右偏纠偏
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
  Forward();
 }
 else if (D2==LOW && D3==LOW && D4==LOW)//脱离地图，亮灯停车
 {
  digitalWrite(reminder,HIGH);
  Stop;
  delay(5000);
 }
}
// 解析蓝牙接收到的数据并设置目标坐标
void ParseData(const String& data)
{

    // 假设数据格式为"X,Y"，这里简单分割获取坐标值
    int commaIndex = data.indexOf(',');
    if (commaIndex!= -1) {
        // 尝试转换为整数并检查转换是否成功
        int xValue = data.substring(0, commaIndex).toInt();
        if (xValue!= 0 || data.substring(0, commaIndex) == "0") {
            targetX = xValue;
        } 
        else {
            Serial.println("Error: Unable to convert X coordinate to integer.");
        }

        int yValue = data.substring(commaIndex + 1).toInt();
        if (yValue!= 0 || data.substring(commaIndex + 1) == "0") {
            targetY = yValue;
        } else {
            Serial.println("Error: Unable to convert Y coordinate to integer.");
        }
    } 
    else {
        Serial.println("Error: Unable to parse data in the expected 'X,Y' format.");
    }
}
//接受到信号
void Wifiinput()
{
  while(1){
    if (!SerialBT.available()){
      return;
    }
    else
    {
        // 发送的数据长度固定为10个字符
        char dataBuffer[11]; // 多留一个字节用于存储字符串结束符'\0'
        SerialBT.readBytes(dataBuffer, 10);
        dataBuffer[10] = '\0';
        String data(dataBuffer);

        ParseData(data);
        Serial.println("Received target coordinates: " + String(targetX) + "," + String(targetY));
        digitalWrite(reminder, HIGH);
        delay(3000);
        digitalWrite(reminder, LOW); // 亮3秒蓝牙接收到数据
        break;
    }
  }
}

void one()
{
  while( x!= targetX )//向右改变x到指定位置
  {
    line();
    D1 = digitalRead(D01);
    D5 = digitalRead(D05);
    if(D1==HIGH || D5==HIGH){x++;}
  }

  while(int i =1)
  {
    Anti();
    D2=digitalRead(D02);
    D4=digitalRead(D04);
    D3=digitalRead(D03);
    if(D4==HIGH || D2==HIGH || D3==HIGH)
    {
      Stop();
      i=0;//跳出循环
    }
  }

  while( y != targetY )
  {
    line();
    D1 = digitalRead(D01);
    D5 = digitalRead(D05);
    if(D1==HIGH || D5==HIGH){y++;}
  }
  delay(2000);//到达指定坐标

  while(int i =1)
  {
    Anti();
    D2=digitalRead(D02);
    D4=digitalRead(D04);
    D3=digitalRead(D03);
    if(D4==HIGH || D2==HIGH || D3==HIGH)
    {
      Stop();
      i=0;//跳出循环
    }
  }
  while( x!= 0)//向右改变x到指定位置
  {
    line();
    D1 = digitalRead(D01);
    D5 = digitalRead(D05);
    if(D1==HIGH || D5==HIGH){x--;}
  }
  while(int i =1)
  {

    Anti();
    D2=digitalRead(D02);
    D4=digitalRead(D04);
    D3=digitalRead(D03);
    if(D4==HIGH || D2==HIGH || D3==HIGH)
    {
      Stop();
      i=0;//跳出循环
    }
  }
  while( y != 0 )
  {
    line();
    D1 = digitalRead(D01);
    D5 = digitalRead(D05);
    if(D1==HIGH || D5==HIGH){y--;}
  }
}
