#include<Arduino.h>
#include <BluetoothSerial.h>

// 蓝牙串口对象
BluetoothSerial SerialBT;

// 蓝牙目标坐标
int targetX = 0;
int targetY = 0;
//功能1计数变量
int x=0;
int y=0;

const int DO1=34;  //前左
const int DO2=35;  //前右
const int DO3=32;  //右前
const int DO4=33;  //右后
const int DO5=25;  //后右
const int DO6=26;  //后左
const int DO7=27;  //左后
const int DO8=21;  //左前
int D1,D2,D3,D4,D5,D6,D7,D8;   //循迹返回值

const int PWMA=4;
const int PWMB=19;
const int AIN1=15;
const int AIN2=2;
const int STBY=22;
const int BIN1=5;
const int BIN2=18;

//指示蓝牙
const int reminder=14;
//控制功能切换
const int button1=13;
const int button2=23;

//定义管脚输入输出
void Definition()
{
  //电驱管脚
	pinMode(PWMA,OUTPUT);
	pinMode(AIN1,OUTPUT);
	pinMode(AIN2,OUTPUT);
	pinMode(PWMB,OUTPUT);
	pinMode(BIN1,OUTPUT);
	pinMode(BIN2,OUTPUT);
	pinMode(STBY,OUTPUT);

  //循迹管脚
  pinMode(DO1,INPUT);
  pinMode(DO2,INPUT);
  pinMode(DO3,INPUT);
  pinMode(DO4,INPUT);
  pinMode(DO5,INPUT);
  pinMode(DO6,INPUT);
  //
  pinMode(reminder,OUTPUT);
  pinMode(button1,INPUT);
  pinMode(button2,INPUT);
  digitalWrite (button1,LOW);
  digitalWrite (button1,LOW);
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
    if (!SerialBT.available())
    {
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


/*小车运动函数
*电驱编号1==A 0==B，速度，正反转)   
*mode:正反转；mode==1,则正转；mode==0，则反转；
*pwm：占空比调速度；
*motor：控制一组电桥
*/
void move(int motor,int pwm,int mode)
{
	boolean pin1=HIGH;
	boolean pin2=LOW;//定义高低电平

  digitalWrite(STBY,LOW);

	if(mode!=1)//若要电机反转，更改电平值
	{
    pin1=LOW;
		pin2=HIGH;
	}
	
  if(motor==0)//制动
  {
    digitalWrite(STBY,LOW);
  }
	else if(motor==1)//控制A电驱
	{
    digitalWrite(STBY,HIGH);
		digitalWrite(AIN1,pin1);
		digitalWrite(AIN2,pin2);
		analogWrite(PWMA,pwm);
	}
	else//控制B电驱
	{
    digitalWrite(STBY,HIGH);
		digitalWrite(BIN1,pin1);
		digitalWrite(BIN2,pin2);
		analogWrite(PWMB,pwm);
	}
}
void Forward(){move(1,80,1);move(2,80,1);}//前进
void Backward(){move(1,80,2);move(2,80,2);}//后退
void Left(){move(1,80,2);move(2,80,1);}//左移
void Round(){move(1,80,1);move(2,80,2);}//右移
void Stop(){move(0,0,0);}//制动

void read(void)// 读取红外传感器的值
{
  D1 = digitalRead(DO1);
  D2 = digitalRead(DO2);
  D3 = digitalRead(DO3);
  D4 = digitalRead(DO4);
  D5 = digitalRead(DO5);
  D6 = digitalRead(DO6);
  delay(1000);// 根据需要添加适当的延时，避免过于频繁读取数据
}

//向指定方向走i个格子
void forward(int i)
{
  y = 0;
  while(y<=i)
  {
    Forward();
    read();
    if( D3==HIGH || D4==HIGH || D7==HIGH || D8==HIGH )
    {
      y++;
    }
  }
  Stop();
}
void backward(int i)
{
  y = 0;
  while(y<=i)
  {
    Backward();
    read();
    if( D3==HIGH || D4==HIGH || D7==HIGH || D8==HIGH )
    {
      y++;
    }
  }
  Stop();
}
void left(int i)
{
  x=0;
  while(x<=i)
  {
    Left();
    read();
    if( D1==HIGH || D2==HIGH || D5==HIGH || D6==HIGH )
    {
      x++;
    }
  }
  Stop();
  
}
void round(int i)
{
  x=0;
  while(x<=i)
  {
    Round();
    read();
    if( D1==HIGH || D2==HIGH || D5==HIGH || D6==HIGH )
    {
      x++;
    }
  }
  Stop();
  
}


//fuction1"单点寻踪"
void FunctionOne()
{
  for (int i = 0; i < 3; i++)
  {
    forward(targetY);
    Stop();
    round(targetX);
    Stop();
    delay(2000);
    backward(targetY);
    Stop();
    left(targetX);
    Stop();
  }
}

//"4"
void FunctionTwo()
{
  round(4);   forward(2); round(1);   forward(1);   left(1);    forward(2);   left(1);    backward(2);  
  left(1);    forward(2); left(1);    backward(3);  round(2);   backward(2);  left(3);
}

//"3"
void FunctionThree()
{
  round(4);     forward(5); left(3);      backward(1);  round(2);     backward(1);  left(2);
  backward(1);  round(2);   backward(1);  left(2);      backward(1);  left(1);
}

//"王"
void FunctionFour()
{
  round(5);forward(1);left(2);forward(1);round(1);
  forward(1);left(1);forward(1);round(2);forward(1);
  left(5);backward(1);round(2);backward(1);left(1);
  backward(1);round(1);backward(1);left(2);backward(1);
}

//五点循迹
void FunctionFive()
{
  int X[5] ,Y[5],S[5];
  for(int i=0 ; i<5 ; i++)    //遍历数组，读取5个坐标
  {
    Wifiinput();// 重新给予五组坐标数据
    X[i] = targetX;
    Y[i] = targetY;
    S[i] = targetX+targetY;//用坐标之和衡量其距原点的距离
  }
  //坐标由近到远排序
  int circle = 1;
  int a,b,c;
  while(circle)
  {
    circle = 0;
    for (int i=0;i<4;i++)
    {
      if(S[i] > S[i+1])
      {
        a=X[i];
        b=Y[i];
        c=S[i];
        X[i]=X[i+1];
        Y[i]=Y[i+1];
        S[i]=S[i+1];
        X[i+1]=a;
        Y[i+1]=b;
        S[i+1]=c;
      }
      circle = 1;
    }
  }
//依次走坐标
  int now_X = 0,now_Y = 0;//目前坐标
  int diff_X , diff_Y ;//需要行动的坐标
  for(int i=0;i<5;i++)
  {
    diff_Y = Y[i] - now_Y;
    now_Y = Y[i];
    diff_X = X[i] - now_Y;
    now_Y = Y[i];
    if(diff_Y > 0)
    {
      forward(diff_Y);
    }
    if(diff_Y < 0)
    {
      backward(diff_Y)
    }
    if(diff_X > 0)
    {
     round(diff_X);
    }
    if(diff_X < 0)
    {
      left(diff_X);
    }
    delay(2000);
  }
  left(X[4]);
  backward(Y[4]);
}


void setup()
{
  Serial.begin(115200);
  SerialBT.begin("ESP32_XX"); // 设置蓝牙名称
  Definition();
}



void loop()
{ 
  Wifiinput();
  int Button1 = digitalRead(button1);
  int Button2 = digitalRead(button2);

  if(Button1==LOW)
  {
    FunctionOne();
  }
  //第一次给数据，确定是否继续读取数据 
  if(Button1==HIGH)
  {
    if(targetX==7)
    {
      FunctionTwo();//"4"
    }

    if(targetX==8)
    {
      FunctionThree();//"3"
    }
    if(targetX==9)
    {
      FunctionFour();//"王"
    }
    else
    {
      FunctionFive();//五点循迹
    }
  }
}
