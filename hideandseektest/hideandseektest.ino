/*
  Hide and Seek code
  To be used with the Hide and Seek interface developed by N Peterson and J Loverude
  1/29/2022
*/

#include <ArduinoBLE.h>

#define SAMPLE_SIZE 10
#define ALARM 8
#define NUM_LEDS 4
#define LED0 6
#define LED1 7
#define LED2 10
#define LED3 9

int sum;
int sampleIdx;
int avg;
int closeness;
int ledStatus;

static int leds[] = {LED0,LED1,LED2,LED3};

void setup() {
  Serial.begin(9600);

  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  sum = 0;
  sampleIdx = 0;
  avg = -127;
  closeness = -127;
  ledStatus = 0;

  pinMode(LED0,OUTPUT);
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);

  BLE.scanForName("CIRCUITPY61cf",true);
}

void loop() {
  if(avg >= -85){tone(ALARM, closeness);delay(10);noTone(ALARM);lightTo(ledStatus);} else {noTone(ALARM);}
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    sum += peripheral.rssi();
    sampleIdx++;
    if(sampleIdx == SAMPLE_SIZE){
      avg = sum / SAMPLE_SIZE;
      Serial.println(sum / SAMPLE_SIZE);
      sampleIdx = 0;
      sum = 0;

      closeness = map(avg,-90,-45,250,2000);
      ledStatus = map(closeness,250,2000,0,4);
    }
  }
}

void buzz(){
  for(int i= 0; i<100; i++){
    tone(ALARM,1000);
    delay(10);
    tone(ALARM,500);
    delay(10);
    tone(ALARM,1500);
    delay(10); 
  } 
}

void allOff(){
  for(int i = 0; i < NUM_LEDS; i++){
    pinMode(leds[i],LOW);  
  }  
}

void lightTo(int n){
  if(ledStatus == 0){allOff();return;}
  for(int i = 0; i < n; i++){
    digitalWrite(leds[i],HIGH);  
  }  
}
