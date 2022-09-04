/*
  Xiao Pinout Diagram
Devon White
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

const uint8_t sensorCount = 3;

const uint8_t xshutPins[sensorCount] = { 1, 2, 3 };

VL53L1X sensors[sensorCount];


long t,t0;
int state = 0;
int led = 0;
int dt = 20;
int laser1;
int laser2;
int laser3;


void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  for (uint8_t i = 0; i < sensorCount; i++)
  {
    pinMode(xshutPins[i], OUTPUT);
    digitalWrite(xshutPins[i], LOW);
  }
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C
  for (uint8_t i = 0; i < sensorCount; i++)
  {
    // Stop driving this sensor's XSHUT low. This should allow the carrier
    // board to pull it high. (We do NOT want to drive XSHUT high since it is
    // not level shifted.) Then wait a bit for the sensor to start up.
    pinMode(xshutPins[i], INPUT);
    delay(10);

    sensors[i].setTimeout(0);
    sensors[i].init();
    sensors[i].setAddress(0x2A + i);
    sensors[i].setDistanceMode(VL53L1X::Medium);
    sensors[i].setMeasurementTimingBudget(20000);
    sensors[i].startContinuous(20);
  }
  delay(1000);
  t0=millis();
}

// the loop function runs over and over again forever
void loop() {
  t = millis();
  laser1 = sensors[0].read();
  laser2 = sensors[1].read();
  laser3 = sensors[2].read();
  
  Serial.print(laser1);
  Serial.print('\t');
  Serial.print(laser2);
  Serial.print('\t');
  Serial.print(laser3);
  Serial.println('\t');
  while(millis()<t+dt){
    delay(1);
  }
}
