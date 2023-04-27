#include <SoftwareSerial.h>
#include <DHT.h>
#include "MQ135.h"

SoftwareSerial mySerial(5, 6);

#define DHTPIN 2
#define DHTTYPE DHT11
#define ANALOGPIN A0

DHT dht = DHT(DHTPIN, DHTTYPE); 
MQ135 gasSensor = MQ135(ANALOGPIN);
int aqValue;

void setup()
{
  mySerial.begin(115200);
  Serial.begin(115200);

  float rzero = gasSensor.getRZero();
  delay(1000);

  dht.begin();
}

void loop()
{
  float ppm = gasSensor.getPPM();
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  float heatIndex = dht.computeHeatIndex(temperature, humidity, false);

  Serial.print(ppm);
  Serial.print(",");
  Serial.print(temperature);
  Serial.print(",");
  Serial.print(humidity);
  Serial.print(",");
  Serial.println(heatIndex);

  mySerial.print(ppm);
  mySerial.print(",");
  mySerial.print(temperature);
  mySerial.print(",");
  mySerial.print(humidity);
  mySerial.print(",");
  mySerial.println(heatIndex);
  delay(30000);
}