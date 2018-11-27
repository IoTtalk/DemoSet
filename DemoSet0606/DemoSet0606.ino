#include <Wire.h>
#include <Bridge.h>
#include "Seeed_BME280.h"

#define sensorUpdatePeriod 30000 

//for Bridge
char D13[2];
char valueStr[23];
int  valueInt=0;

const int soilMoisturePin = A0;  //土壤濕度  A0
const int UVpin1 = A1;           //UV  A1

BME280 bme280;   //BME 大氣壓   //D2(SDA), D3(SCL)
float Temperature=0.0;
uint32_t AtmosphericPressure=0, Humidity=0;

const int BugPin = 7;
int BugCounter = 0;
int PreviousBugCounter = 9999;
long suspendTimestamp = millis();
void blink(){//Bugs
    if (millis()-suspendTimestamp > 1000){
        BugCounter += 1;
        suspendTimestamp = millis();
    }
}

void setup() {
    pinMode(4,OUTPUT);
    digitalWrite(4, LOW);    
    pinMode(5,OUTPUT);    
    digitalWrite(5, LOW);
    pinMode(6,OUTPUT);    
    digitalWrite(6, LOW);

    pinMode(BugPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BugPin), blink, RISING);
    
    if(!bme280.init()) Serial.println("Sensor Error!");  //BME208 大氣壓 D2, D3

    pinMode(soilMoisturePin,INPUT);  //土壤濕度   A0 
    pinMode(UVpin1,INPUT);            //UV        A1
    
    //Serial.begin(115200);
    Bridge.begin();   // D0 and D1 are used by the Bridge library.
 
    //IoTtalk successful registration notification
    pinMode(13,OUTPUT);
}

long sensorUpdateTimestamp = millis();
void loop() {
    if (millis()-sensorUpdateTimestamp > sensorUpdatePeriod){
        Bridge.get("Reg_done",  D13, 2);
        digitalWrite(13, atoi(D13));    

        valueInt = analogRead(soilMoisturePin);     //土壤濕度  A0
        //Serial.print("Soil moisture: ");    Serial.println(valueInt);   
        itoa(valueInt, valueStr, 10); 
        Bridge.put("SoilMoisture", valueStr); 

        valueInt = analogRead(UVpin1);      //UV sensor A1
        //Serial.print("UV: ");    Serial.println(valueInt);
        itoa(valueInt, valueStr, 10);     
        Bridge.put("UV", valueStr); 

        Temperature = bme280.getTemperature();      //BME 大氣壓   D2, D3
        AtmosphericPressure = bme280.getPressure();
        Humidity = bme280.getHumidity();
        //Serial.print("Temp: ");        Serial.print(Temperature);    Serial.println(" C");// unit: C
        //Serial.print("Pressure: ");    Serial.print(AtmosphericPressure);    Serial.println(" Pa");     // unit: Pa
        //Serial.print("Altitude: ");    Serial.print(bme280.calcAltitude(AtmosphericPressure));    Serial.println(" m");  // unit: meter
        //Serial.print("Humidity: ");    Serial.print(Humidity);    Serial.println(" %"); // unit:%
        dtostrf(Temperature, 6, 2, valueStr);
        Bridge.put("Temperature", valueStr); 
        dtostrf(AtmosphericPressure, 8, 0, valueStr);
        Bridge.put("AtPressure", valueStr);     
        dtostrf(Humidity, 8, 0, valueStr);
        Bridge.put("Humidity", valueStr);        

        if (PreviousBugCounter != BugCounter){
            PreviousBugCounter = BugCounter;
            itoa(BugCounter, valueStr, 10);
            Bridge.put("Bugs", valueStr);
        }
        //Serial.print("Bugs counter: ");    Serial.println(BugCounter);

        sensorUpdateTimestamp = millis();
    }
    
    char Switch[]="Switch-O1";  
    for (int i=0; i<=2; i++){
        Switch[8] = i+49;
        Bridge.get(Switch, valueStr, 5);
        if (strcmp(valueStr,"") != 0){
            valueInt = atoi(valueStr);            
            if (valueInt >= 1) valueInt=1; else valueInt=0;        
            digitalWrite(i+4, valueInt);            
            Bridge.put(Switch, "");
            //Serial.print("Switch-O"); Serial.print(i+1); Serial.print(": ");    Serial.println(valueInt);
        }
    }
    //Serial.println("=========================================================================");
}
