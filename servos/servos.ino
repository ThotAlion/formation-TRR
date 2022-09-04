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
#include <Servo.h>
#define PIN_STEERING 8
#define PIN_THRUST   9
Servo steering;
Servo thrust;

long t,t0;
int state = 0;
int led = 0;
int dt = 20;
int thr,steer;
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  steering.attach(PIN_STEERING);
  thrust.attach(PIN_THRUST);
  Serial.begin(115200);
  delay(1000);
  t0=millis();
}

// the loop function runs over and over again forever
void loop() {
  t = millis();
  if(state == 0){
    if(t>t0+1000){
      led = 0;
      thr = 1500;
      steer = 1500;
      t0=t;
      state=1;
    }
  }else if(state==1){
    if(t>t0+1000){
      led = 1;
      thr = 1500+200;
      steer = 1500+200;
      t0=t;
      state=0;
    }
  }
  
  digitalWrite(LED_BUILTIN, led);   // turn the LED on (HIGH is the voltage level)
  steering.writeMicroseconds(steer);
  thrust.writeMicroseconds(thr);
  Serial.println(state);
  while(millis()<t+dt){
    delay(1);
  }
}
