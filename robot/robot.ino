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
            SCL|D5 |o |XIAO-SAMD21| o|  D8|servo poussee
             TX|D6 |o |___________| o|  D7|RX
                   |_________________|
*/
#include <Servo.h>
#include <Wire.h>
#include <VL53L1X.h>
#define PIN_DIR       9
#define PIN_POUSSEE   8
Servo steering;
Servo thrust;

#define XSHUTHAUT     1
#define XSHUTDEVANT   2
#define XSHUTCOTE     3
VL53L1X laserHaut;
VL53L1X laserDevant;
VL53L1X laserCote;




long t,t0;
int dt = 20;
int thr,steer,led;
int state = 0;
int LHaut;
int LDevant;
int LCote;

float D,A,L1,L2,L3,L4,beta,Pmur;

void setup() {
  // INITIALISATION
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  steering.attach(PIN_DIR);
  thrust.attach(PIN_POUSSEE);
  
  // on eteint tout
  pinMode(XSHUTHAUT, OUTPUT);
  digitalWrite(XSHUTHAUT, LOW);
  pinMode(XSHUTDEVANT, OUTPUT);
  digitalWrite(XSHUTDEVANT, LOW);
  pinMode(XSHUTCOTE, OUTPUT);
  digitalWrite(XSHUTCOTE, LOW);

  // port I2C
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C

  pinMode(XSHUTHAUT, INPUT);
  delay(10);
  laserHaut.setTimeout(0);
  laserHaut.init();
  laserHaut.setAddress(0x2A + 0);
  laserHaut.setDistanceMode(VL53L1X::Medium);
  laserHaut.setMeasurementTimingBudget(20000);
  laserHaut.startContinuous(20);

  pinMode(XSHUTDEVANT, INPUT);
  delay(10);
  laserDevant.setTimeout(0);
  laserDevant.init();
  laserDevant.setAddress(0x2A + 1);
  laserDevant.setDistanceMode(VL53L1X::Medium);
  laserDevant.setMeasurementTimingBudget(20000);
  laserDevant.startContinuous(20);

  pinMode(XSHUTCOTE, INPUT);
  delay(10);
  laserCote.setTimeout(0);
  laserCote.init();
  laserCote.setAddress(0x2A + 2);
  laserCote.setDistanceMode(VL53L1X::Medium);
  laserCote.setMeasurementTimingBudget(20000);
  laserCote.startContinuous(20);
  
  delay(1000);
  t0=millis();

}

void loop() {
  /**********************************/
  // CAPTEURS
  /**********************************/
  t = millis();

  LHaut = laserHaut.read();
  LDevant = laserDevant.read();
  LCote = laserCote.read();

  /**********************************/
  // MACHINE A ETAT
  /**********************************/
  if(state == 0){ // etat d'attente au depart
    led = 0;
    steer=0;
    thr=0;
    if(LHaut<200){
      t0=t;
      state=1;
    }
  }else if(state==1){
    led=1;

    // loi 1
    //steer=1*(LCote-300);

    // loi 2
    D = 0.5;
    A = 0.5;
    L1 = LCote/1000.0;
    L2 = LDevant/1000.0;
    L3 = L2*cos(M_PI/4);
    L4 = L2*sin(M_PI/4);
    Pmur = L1+(L3-L1)*A/L4;
    beta = atan((Pmur-D)/A);
    
    steer=(int)(1000*beta/M_PI);
    
    if(LDevant<100){
      thr=0;
    }else if(LDevant<200){
      thr=200;
    }else if(LDevant<300){
      thr=500;
    }else{
      thr=500;
    }
    if(t>t0+10000){
      t0=t;
      state=0;
    }
  }
  /**********************************/
  // CALCULS
  /**********************************/

  /**********************************/
  // ACTIONNEURS
  /**********************************/
  // la LED
  digitalWrite(LED_BUILTIN, led);   // turn the LED on (HIGH is the voltage level)
  
  // les servos
  //250 -> 45deg vers la gauche
  if(steer>250){
    steer=250;
  }else if(steer<-250){
    steer=-250;
  }
  if(thr>1000){
    thr=1000;
  }else if(thr<-1000){
    thr=-1000;
  }
  steering.writeMicroseconds(1500+steer);
  thrust.writeMicroseconds(1500-thr);

  // le serial
  Serial.print(state);
  Serial.print('\t');
  Serial.print(LHaut);
  Serial.print('\t');
  Serial.print(LDevant);
  Serial.print('\t');
  Serial.print(LCote);
  Serial.print('\t');
  Serial.print(thr);
  Serial.print('\t');
  Serial.print(steer);
  Serial.print('\t');
  Serial.print(L1);
  Serial.print('\t');
  Serial.print(L2);
  Serial.print('\t');
  Serial.print(L3);
  Serial.print('\t');
  Serial.print(L4);
  Serial.print('\t');
  Serial.print(Pmur);
  Serial.print('\t');
  Serial.print(beta*180/M_PI);
  Serial.print('\t');
  Serial.println('\t');

  while(millis()<t+dt){
    delay(1);
  }

}
