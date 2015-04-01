#include <RFduinoBLE.h>

const int analogInPin1 = 2; //pressure
const int analogInPin2 = 3; //VDD
const int base = 235; 

float frame = 25;
float pressureValue = 0;      
float powerValue = 0;
float maxPressure = 30;
float prevValue = 0.0; 

void setup() {
  RFduinoBLE.advertisementData = "Keppi";
  RFduinoBLE.txPowerLevel = -12;
  RFduinoBLE.begin();
  Serial.begin(9600); 
}

void loop() {

  if(RFduinoBLE.radioActive){
  int ulpDelay = frame * 2 - 1;
  RFduino_ULPDelay( ulpDelay );  

  float avePressure = 0; 
  float normalValue = 0;     
  
  for(int i = 0; i < int(frame); i++){
      
    pressureValue = (analogRead(analogInPin1) - base);   
  
    if (pressureValue <= 0) {
      pressureValue = 0;
    }
    if (pressureValue > maxPressure) {
      pressureValue = prevValue; 
    }
   
    prevValue = pressureValue;
    avePressure += pressureValue;  
    delay(2); 
  }
     
  float pressure = avePressure / frame; 
  float normalPressure = pressure / maxPressure;

  powerValue = float(analogRead(analogInPin2)) / 1024.0;
  normalValue = (normalPressure * 100) * powerValue;
  Serial.print("normalValue=");
  Serial.println(normalValue); 

  //int value = int(normalValue);
  //Serial.println(value); 
  RFduinoBLE.sendFloat(normalValue);  
  }
}
