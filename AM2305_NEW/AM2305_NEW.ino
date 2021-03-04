#include "AM2305.h"
/****************************************************/
#define Sensor_PIN 7
/****************************************************/
temp_humi Temp_Humi1 ;
void setup()
{
    Set_Pin(Sensor_PIN);
    Serial.begin(9600);   
    pinMode(Sensor_PIN,OUTPUT);
    digitalWrite(Sensor_PIN,HIGH);
}

void loop() 
{
    Serial.println("Wel-come");  
    Get_TImeMicroSecond(5000000);
    Temp_Humi1 = Get_Data_AM2305();
    Serial.print("Temperature retun:");
    Serial.println(Temp_Humi1.Temp);
    Serial.print("Humi retun:");
    Serial.println(Temp_Humi1.Humi);
    Temp_Humi1.Temp =0.0;
    Temp_Humi1.Humi =0.0;
}
/****************************************************/
