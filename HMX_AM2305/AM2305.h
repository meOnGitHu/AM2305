#ifndef AM2304
#define AM2305
#include <Arduino.h>
#include "HardwareSerial.h"
typedef struct 
{
  float Humi;
  float Temp;
}temp_humi;
/****************************************************/
void Get_TImeMicroSecond(long int Time);
void Set_Pin(int pin_no);
/****************************************************/
void Sensor_InIt();
/****************************************************/
temp_humi  Get_Data_AM2305();
/****************************************************/
int Check_parity(byte *array1);
/****************************************************/
temp_humi Get_Humi(byte *array_data);
/****************************************************/
temp_humi Get_Temp(byte *array_data);
/****************************************************/
int Bin_to_Dec(byte s[]);
/****************************************************/
void addBinary(byte a[], byte b[], byte temp[]);
#endif
