 #include <DebugSerial.h>
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT22

#define BRIGHTPIN A1

DHT dht(DHTPIN, DHTTYPE);


 yz
void setup() {
  // put your setup code here, to run once:
  // �Ƶ��̳�� UART ���
  Serial.begin(115200);
  Serial.println("UART");

  // ARTIK cmd 
  DebugSerial.begin(115200);
  DebugSerial.println("START");
  
  // �½�������
  dht.begin();
}

int hum, temp, brightness;
float duration;

void loop() {
  // put your main code here, to run repeatedly:
  // DHT22�� ����, �µ��� �޾ƿ���

  hum = dht.readHumidity();
  temp = dht.readTemperature();

  // �������� �� �޾ƿ���
  brightness = analogRead(BRIGHTPIN);
  
  DebugSerial.print(hum);
  DebugSerial.print("\t");
  DebugSerial.print(temp);
  DebugSerial.print("\t");
  DebugSerial.println(brightness);
  
  if(isnan(hum) || isnan(temp)) {
    DebugSerial.println("Failed to read from DHT");
  }
  if(isnan(brightness)) {
    DebugSerial.println("Failed to read from BRIGHTNESS");
  }
  else {
    Serial.print("->");

    if(hum < 10) {
      Serial.print(0);
      Serial.print(0);
      Serial.print(hum);
    }
    else if(hum < 100) {
      Serial.print(0);
      Serial.print(hum/10);
      Serial.print(hum%10);      
    }
    else if(hum < 1000) {
      Serial.print(hum/100);
      Serial.print(hum%100/10);
      Serial.print(hum%10);
    }
    
    if(temp < 10) {
      Serial.print(0);
      Serial.print(0);
      Serial.print(temp);
    }
    else if(temp < 100) {
      Serial.print(0);
      Serial.print(temp/10);
      Serial.print(temp%10);   
    }
    else if(temp < 1000) {
      Serial.print(temp/100);
      Serial.print(temp%100/10);
      Serial.print(temp%10);                                                                                                                      
    } 
    
    if(brightness < 10) {
      Serial.print(0);
      Serial.print(0);
      Serial.print(brightness);
    }
    else if(brightness < 100) {
      Serial.print(0);
      Serial.print(brightness/10);
      Serial.print(brightness%10);   
    }
    else if(brightness < 1000) {
      Serial.print(brightness/100);
      Serial.print(brightness%100/10);
      Serial.print(brightness%10);    
    }
  }
  delay(500);
}