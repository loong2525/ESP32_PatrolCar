#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>          // 包含Arduino核心库（解决String问题）
#include <BluetoothSerial.h>  // 包含蓝牙库（解决BluetoothSerial未声明）

extern const int LED;


 // 解析蓝牙接收到的数据并设置目标坐标
void ParseData(const String& data);

//接受到信号 
void Wifiinput();

#endif 