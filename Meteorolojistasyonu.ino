char                 databuffer[35];
//double               temp;
int windDir= 0, humidity= 0, chk = 0;
float windSpeAvg = 0, windSpeMax= 0, temp= 0, rainfallhour= 0, rainfallday= 0, barPres= 0;
char valA[10], valB[10], valC[10],valD[10], valE[10], valF[10],valG[10], valP[10], valR[10],valS[10], valA1[10], valA2[10],valA3[10], valA4[10], valA5[10],valA6[10];
char urlData[600];
#include <dht.h>
dht DHT;
#define DHT11_PIN 31
unsigned long lastTime = 0, sendMessagelastTime= 0;

void getBuffer()                                                                    //Get weather status data
{
  int index;
  for (index = 0;index < 35;index ++)
  {
    if(Serial3.available())
    {
      databuffer[index] = Serial3.read();
      if (databuffer[0] != 'c')
      {
        index = -1;
      }
    }
    else
    {
      index --;
    }
  }
}

int transCharToInt(char *_buffer,int _start,int _stop)                               //char to int）
{
  int _index;
  int result = 0;
  int num = _stop - _start + 1;
  int _temp[num];
  for (_index = _start;_index <= _stop;_index ++)
  {
    _temp[_index - _start] = _buffer[_index] - '0';
    result = 10*result + _temp[_index - _start];
  }
  return result;
}

int WindDirection()                                                                  //Wind Direction
{
  windDir = transCharToInt(databuffer,1,3);
  return windDir;
}

/*float WindSpeedAverage()                                                             //air Speed (1 minute)
{
  windSpeAvg = 0.44704 * transCharToInt(databuffer,5,7);
  return windSpeAvg;
}*/

float WindSpeedMax()                                                                 //Max air speed (5 minutes)
{
  windSpeMax = 0.44704 * transCharToInt(databuffer,9,11);
  return windSpeMax;
}

float Temperature()                                                                  //Temperature ("C")
{
  temp = (transCharToInt(databuffer,13,15) - 32.00) * 5.00 / 9.00;
  return temp;
}

float RainfallOneHour()                                                              //Rainfall (1 hour)
{
  rainfallhour = transCharToInt(databuffer,17,19) * 25.40 * 0.01;
  return rainfallhour;
}

float RainfallOneDay()                                                               //Rainfall (24 hours)
{
  rainfallday = transCharToInt(databuffer,21,23) * 25.40 * 0.01;
  return rainfallday;
}

int Humidity()                                                                       //Humidity
{
  humidity = transCharToInt(databuffer,25,26);
  return humidity;
}

float BarPressure()                                                                  //Barometric Pressure
{
  barPres = transCharToInt(databuffer,28,32) / 10.00;
  return barPres;
}
void sendData(float inTemp,float inHumid){
  
  Serial1.println("AT+SAPBR=3,1,\"APN\",\"internet\"");
  delay(1000);
  Serial1.println("AT+SAPBR=1,1");
  delay(1500);
  Serial1.println("AT+HTTPINIT");
  delay(1000);
  Serial1.println("AT+HTTPPARA=\"CID\",1");
  delay(1000);
  //Serial1.println("AT+HTTPPARA=\"URL\",\"http://www.unalkizil.com/index.php?sicaklik=21&nem=22&sni=23\""); Örnek internet isteğib
  //Serial1.println("AT+HTTPACTION=0");
  
  dtostrf(windDir, 3, 3, valA);
  //dtostrf(windSpeAvg, 3, 3, valB);
  dtostrf(windSpeMax, 3, 3, valC);
  dtostrf(temp, 3, 3, valD);
  dtostrf(rainfallhour, 3, 3, valE);
  dtostrf(rainfallday, 3, 3, valF);
  dtostrf(humidity, 3, 3, valG);
  dtostrf(barPres, 3, 3, valR);
  dtostrf(inTemp, 3, 3, valP);
  dtostrf(inHumid, 3, 3, valS);
  dtostrf(analogRead(A1), 3, 3, valA1);
  dtostrf(analogRead(A2), 3, 3, valA2);
  dtostrf(analogRead(A3), 3, 3, valA3);
  dtostrf(analogRead(A4), 3, 3, valA4);
  dtostrf(analogRead(A5), 3, 3, valA5);
  dtostrf(analogRead(A6), 3, 3, valA6);
  
  strcpy(urlData, "AT+HTTPPARA=\"URL\",\"http://www.unalkizil.com/sefa.php?");
  strcat(urlData, "RuzgarYonu=");
  strcat(urlData, valA);
  //strcat(urlData, "&OrtRuzgarHizi=");
  //strcat(urlData, valB);
  strcat(urlData, "&MaxRuzgarHizi=");
  strcat(urlData, valC);
  strcat(urlData, "&Sicaklik=");
  strcat(urlData, valD);
  strcat(urlData, "&SaatlikYagis=");
  strcat(urlData, valE);
  strcat(urlData, "&GunlukYagis=");
  strcat(urlData, valF);
  strcat(urlData, "&BagilNem=");
  strcat(urlData, valG);
  strcat(urlData, "&AtmBasinci=");
  strcat(urlData, valR);
  strcat(urlData, "&IcSicaklik=");
  strcat(urlData, valP);
  strcat(urlData, "&IcNem=");
  strcat(urlData, valS);
  strcat(urlData, "&S1=");
  strcat(urlData, valA1);
  strcat(urlData, "&S2=");
  strcat(urlData, valA2);
  strcat(urlData, "&S3=");
  strcat(urlData, valA3);
  strcat(urlData, "&S4=");
  strcat(urlData, valA4);
  strcat(urlData, "&S5=");
  strcat(urlData, valA5);
  strcat(urlData, "&S6=");
  strcat(urlData, valA6);
  strcat(urlData, "\"");
  
  Serial.print("Payload: ");      //
  Serial.print(urlData);          // Print to serial console to see what is inside the payload
  Serial.println(" EOL");
  delay(1000);
  Serial1.println(urlData);
  delay(1000);
  Serial1.println("AT+HTTPACTION=0");
  Serial.println("Data sent!");
}
void sendSms(){
  Serial1.print("AT+CMGF=1\r"); // Set the shield to SMS mode
  delay(100);
  Serial1.println("AT+CMGS=\"+90TELNO\"");
  delay(100);
  Serial1.println(windSpeMax);
  delay(100);
  Serial1.print((char)26);//the ASCII code of the ctrl+z is 26 (required according to the datasheet)
  delay(100);
  Serial1.println();
  Serial.println("Sms Sent.");
}
void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial3.begin(9600);
  delay(500);
}
void loop()
{
  getBuffer();                                                                      //Begin!
  if (millis() - lastTime > 300000)//5dk
  {
    Serial.print("Wind Direction: ");
    Serial.print(WindDirection());
    Serial.println("  ");
    //Serial.print("Average Wind Speed (One Minute): ");
    //Serial.print(WindSpeedAverage());
    //Serial.println("m/s  ");
    Serial.print("Max Wind Speed (Five Minutes): ");
    Serial.print(WindSpeedMax());
    Serial.println("m/s");
    Serial.print("Rain Fall (One Hour): ");
    Serial.print(RainfallOneHour());
    Serial.println("mm  ");
    Serial.print("Rain Fall (24 Hour): ");
    Serial.print(RainfallOneDay());
    Serial.println("mm");
    Serial.print("Temperature: ");
    Serial.print(Temperature());
    Serial.println("C  ");
    Serial.print("Humidity: ");
    Serial.print(Humidity());
    Serial.println("%  ");
    Serial.print("Barometric Pressure: ");
    Serial.print(BarPressure());
    Serial.println("hPa");
    chk = DHT.read11(DHT11_PIN);
    Serial.print("DhtTemperature = ");
    Serial.println(DHT.temperature);
    Serial.print("DhtHumidity = ");
    Serial.println(DHT.humidity);
    Serial.println("");
    sendData(DHT.temperature, DHT.humidity);
    lastTime = millis();
  }
  if(millis() - sendMessagelastTime > 3600000){//60dk
    sendSms();
    sendMessagelastTime = millis();
  }
  
}
