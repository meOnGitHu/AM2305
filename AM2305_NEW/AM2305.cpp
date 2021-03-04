
#include "AM2305.h"
 
int Sensor_PIN;

 temp_humi temp_data;
/****************************************************/
void Set_Pin(int pin_no)
{
  Sensor_PIN= pin_no;
}
/****************************************************/
void Get_TImeMicroSecond(long int Time)
{
      long int DelayTime;
      DelayTime = micros();
      while (micros()-DelayTime < Time);
}
/****************************************************/
/****************************************************/
void Sensor_InIt()
{
  pinMode(Sensor_PIN,OUTPUT);
  digitalWrite(Sensor_PIN,HIGH);
  digitalWrite(Sensor_PIN,LOW);
  Get_TImeMicroSecond(1000);
  digitalWrite(Sensor_PIN,HIGH);
  
}
/****************************************************/
temp_humi  Get_Data_AM2305(void)
{
  int i=0;
  
  float Humi,Temp;
  unsigned long duration[41];
  byte Result[41];
  pinMode(Sensor_PIN, OUTPUT);
  digitalWrite(Sensor_PIN,HIGH);
  delay(1000);
  digitalWrite(Sensor_PIN,LOW);
  delay(1);
  digitalWrite(Sensor_PIN,HIGH);
  pinMode(Sensor_PIN, INPUT);
  while(i < 41)
  {
    duration[i] = pulseIn(Sensor_PIN, HIGH);
    i++;
  }
  i=0;
  int k;
  for(i = 1 ;i<41; i++,k++)
  {
    
      if(duration[i] < 30)
      {
        Result[k]= 0;
      }
      else
      {
        Result[k]= 1;
      }
  }
   for( i = 0; i<40; i++)
  {
    Serial.print(Result[i] );
  }
  Check_parity(Result);
  Get_Temp(Result); 
  Get_Humi(Result);
  memset(Result,0,40);
  Serial.println();
  i=0;
  return temp_data;
}
/****************************************************/
int Check_parity(byte *array_data)
{
  byte Parity[8]={0};
  byte Parity1[8]={0};
  byte Result[8]={0};
  int i=0;
  byte High_RH[8];
  byte LOW_RH[8];
  byte High_TEMP[8];
  byte LOW_TEMP[8];
  byte Origenal_Parity[8];
  memcpy(High_RH,array_data,8);
  memcpy(LOW_RH,array_data+8,8);
  memcpy(High_TEMP,array_data+16,8);
  memcpy(LOW_TEMP,array_data+24,8);
  memcpy(Origenal_Parity,array_data+32,8);
  Serial.println();

  addBinary(High_RH,LOW_RH,Parity);
  addBinary(High_TEMP,LOW_TEMP,Parity1);
  return 0;
}
/****************************************************/
temp_humi Get_Temp(byte *array_data)
{
  float val=0.00;
  int o,p,q,r;
  float ret;
  o=p=q=r=0;
  byte High_TEMP_U[4];
  byte High_TEMP_L[4];
  byte LOW_TEMP_U[4];
  byte LOW_TEMP_L[4];
  memcpy(High_TEMP_U,array_data+16,4);
  memcpy(High_TEMP_L,array_data+20,4);
  memcpy(LOW_TEMP_U,array_data+24,4);
  memcpy(LOW_TEMP_L,array_data+28,4);
  o = Bin_to_Dec(High_TEMP_U);
  p = Bin_to_Dec(High_TEMP_L);
  q = Bin_to_Dec(LOW_TEMP_U);
  r = Bin_to_Dec(LOW_TEMP_L);
  ret = (p*256+q*16+r);
  val = ret/10;
  temp_data.Temp = val;
  return temp_data;

}
/****************************************************/
temp_humi Get_Humi(byte *array_data)
{
  float val=0.00;
  int k,l,m,n;
  float ret;
  k=l=m=n=0;
  byte High_RH_U[4];
  byte High_RH_L[4];
  byte LOW_RH_U[4];
  byte LOW_RH_L[4];
  memcpy(High_RH_U,array_data,4);
  memcpy(High_RH_L,array_data+4,4);
  memcpy(LOW_RH_U,array_data+8,4);
  memcpy(LOW_RH_L,array_data+12,4);
  k = Bin_to_Dec(High_RH_U);
  l = Bin_to_Dec(High_RH_L);
  m = Bin_to_Dec(LOW_RH_U);
  n = Bin_to_Dec(LOW_RH_L);
  ret = (l*256+m*16+n);
  val = ret/10;
  temp_data.Humi = val;
  return temp_data;
}
/****************************************************/
void Print_Data(byte *array_data)
{
  int k=0;
  for(k=0;k<sizeof(array_data);k++)
  {
    Serial.print(array_data[k]);
  }
  Serial.print(" ");
  
}
/****************************************************/
int Bin_to_Dec(byte s[])
{
    int n = 0;
    int i;

    for (i = 0; i < 4; ++i)
    {
        n <<= 1;
        n += s[i];
    }

    return n;
}
/****************************************************/

void addBinary(byte a[], byte b[], byte sum[]){
    int i, c = 0;
    for(i = 0; i < 8 ; i++){
       sum[i] = ((a[i] ^ b[i]) ^ c); // c is carry
       c = ((a[i] & b[i]) | (a[i] & c)) | (b[i] & c);
    }
   for( i = 0; i<8; i++)
  {
    Serial.print(sum[i] );
  }
  Serial.println();
    sum[i] = c;
}
