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

long t,t0;
int state = 0;
int led = 0;
int dt = 20;
char key='0';
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  delay(1000);
  t0=millis();
}

// the loop function runs over and over again forever
void loop() {
  t = millis();
  if (Serial.available() > 0) {
    key = (char)Serial.read();
  }
  
  if(state == 0){
    if(t>t0+1000 || key=='1'){
      led = 0;
      t0=t;
      state=1;
    }
  }else if(state==1 || key=='1'){
    if(t>t0+1000){
      led = 1;
      t0=t;
      state=0;
    }
  }
  
  digitalWrite(LED_BUILTIN, led);   // turn the LED on (HIGH is the voltage level)
  Serial.println(state);
  while(millis()<t+dt){
    delay(1);
  }
}
