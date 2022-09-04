/*
  Xiao Pinout Diagram
Devon White
                         _______
                    ____|       |____
                   |    |       |    |
          libre|D0 |o   | -- -- |   o|  5V 
      xshutHAUT|D1 |o   |_[]_[]_|   o| GND 
    xshutDEVANT|D2 |o  ___________  o| 3V3 
      xshutCOTE|D3 |o |\()/ seeed | o| D10|libre
            SDA|D4 |o |   Model:  | o|  D9|servo direction
            SCL|D5 |o |XIAO-SAMD21| o|  D8|servo poussée
             TX|D6 |o |___________| o|  D7|RX
                   |_________________|

*/

#include <Wire.h>
#include <VL53L1X.h>
#define XSHUT1 1
#define XSHUT2 2
#define XSHUT3 3

VL53L1X sensor;

long t,t0;
int state = 0;
int led = 0;
int dt = 20;
int laser1;


void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(XSHUT1, OUTPUT);
  pinMode(XSHUT2, OUTPUT);
  pinMode(XSHUT3, OUTPUT);
  digitalWrite(XSHUT1,LOW);
  digitalWrite(XSHUT2,LOW);
  digitalWrite(XSHUT3,HIGH);
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C
  sensor.setTimeout(500);
  sensor.init();
  sensor.setDistanceMode(VL53L1X::Medium);
  sensor.setMeasurementTimingBudget(20000);
  sensor.startContinuous(20);
  delay(1000);
  t0=millis();
}

// the loop function runs over and over again forever
void loop() {
  t = millis();
  laser1 = sensor.read();
  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  
  Serial.println(laser1);
  while(millis()<t+dt){
    delay(1);
  }
}
