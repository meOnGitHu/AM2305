#include <RH_RF95.h>
#include "AM2305.h"
/******************************************************/
/*
 * Here we define pin and other configurations like
 * which AM2305 we are using and their 
 * working frequency etc
 * Note : all pins are configurable 
 */
#define AM2305    7     
#define RFM95_CS  10
#define RFM95_RST 9
#define RFM95_INT 2
#define BUF_SIZE  40
#define RF95_FREQ 868.0
/********************************************************/
RH_RF95 rf95(RFM95_CS, RFM95_INT);
/*******************************************************/
/* final
 * Here we declare a global variable
 * Global variable are used for receiving values of temperature, humidity etc 
 * These values are used for further calculations.   
 */
char Device_ID[]={'H','M','X',',','5','0','1',','};
char radiopacket[BUF_SIZE]={0};
temp_humi Temp_Humi1;
/*******************************************************/
/*
 * Inside void setup, set all parameters 
 * Parameters which need a one-time setup  like 
 * RF initialization etc...
 */
void setup() 
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  Set_Pin(AM2305);
  Serial.begin(9600);   
  pinMode(AM2305,OUTPUT);
  digitalWrite(AM2305,HIGH);
  delay(100);  
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
 
  while (!rf95.init())
  {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("Tx init OK!");
  Serial.println(Device_ID);
  if (!rf95.setFrequency(RF95_FREQ)) 
  {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("set Frequency:");
  Serial.println(RF95_FREQ);
  rf95.setTxPower(23, false);
}
/*******************************************************/
/*
 * void loop is used for tasks which need to be performed continously.
 * Using the following APIs, get and calcluate values of temp and RH
 * hum = dht.readHumidity();
 * temp= dht.readTemperature();
 * Values which are read from DHT sensor are sent over LoRa 
 * Using memcpy(); --> copy the data in buffer 
 * rf95.send(); --> using this function send the data over LoRa 
 * 
 */
void loop()
{
  delay(2000);
  Temp_Humi1 = Get_Data_AM2305();
  Serial.print("temp:");
  Serial.print(Temp_Humi1.Temp);
  Serial.print("\thumi:");
  Serial.println(Temp_Humi1.Humi);
  memcpy(radiopacket+0,Device_ID,sizeof(Device_ID));
  dtostrf(Temp_Humi1.Temp,5,2,radiopacket+strlen(radiopacket));
  memcpy(radiopacket+strlen(radiopacket),",",1);
  dtostrf(Temp_Humi1.Humi,5,2,radiopacket+strlen(radiopacket));
  radiopacket[39] = 0;
  rf95.send((uint8_t *)radiopacket, BUF_SIZE);
  delay(10);
  rf95.waitPacketSent();
  delay(10);
  //Serial.println(radiopacket);
  memset(radiopacket,0,BUF_SIZE);
  delay(300000);
}

/*******************************************************/
