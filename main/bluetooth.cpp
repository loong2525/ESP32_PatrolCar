#include "bluetooth.h";

const int LED=26;

BluetoothSerial SerialBT;

// 蓝牙目标坐标
int targetX = 0;
int targetY = 0;


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
        digitalWrite(LED, HIGH);
        delay(3000);
        digitalWrite(LED, LOW); // 亮3秒蓝牙接收到数据
        break;
    }
  }
}