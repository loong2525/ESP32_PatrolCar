
#ifndef CARLIB_H
#define CARLIB_H

#include <Arduino.h>          // 包含Arduino核心库（解决String问题）
#include <BluetoothSerial.h>  // 包含蓝牙库（解决BluetoothSerial未声明）


extern const int PWM;
extern const int STBY;

extern const int AIN1_left;
extern const int AIN2_left;
extern const int BIN1_left;
extern const int BIN2_left;
extern const int AIN1_round;
extern const int AIN2_round;
extern const int BIN1_round;
extern const int BIN2_round;



/*电驱控制函数  
*mode:正反转；mode==1,则正转；mode==2，则反转；
*pwm：占空比调速度；
*side：选择1，2号电驱，1控制左轮，2控制右轮
*motor：控制一组电桥，1为前轮，2为后轮，0为停止
*/
void move(int mode,int pwm,int side,int motor);
void Forward();
void Backward();
void Left();
void Round();
void Clock();
void Anti();
void Stop();



#endif 