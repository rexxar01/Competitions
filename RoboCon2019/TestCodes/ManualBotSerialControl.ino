    //Test code for serial control of manual bot. As of now it DOES NOT include the D45 Library codes. Will be done as soon as that is obtained.
    #define pwm1 1 //The pin nos go here
    #define dxn1 2 //The pin nos go here
    #define pwm2 3 //The pin nos go here
    #define dxn2 4 //The pin nos go here
    #define pwm3 5 //The pin nos go here
    #define dxn3 6 //The pin nos go here
    #define pwm4 7 //The pin nos go here
    #define dxn4 8 //The pin nos go here
    int state = 0;
    void setup() {
      pinMode(pwm1, OUTPUT);
      pinMode(pwm2,OUTPUT);
      pinMode(pwm3, OUTPUT);
      pinMode(pwm4,OUTPUT);
      pinMode(dxn1, OUTPUT);
      pinMode(dxn2,OUTPUT);
      pinMode(dxn3, OUTPUT);
      pinMode(dxn4,OUTPUT);
      Serial.begin(9600); // Default communication rate of the Bluetooth module
      Serial1.begin(9600);
    }
    void loop() {
      test:
      if(Serial1.available() > 0){ // Checks whether data is comming from the serial port
        state = Serial1.read();// Reads the data from the serial port
     }
     if(state == 'f'){ // f for forward b for backward r for reverse + for anti clockwise rot - for clockwise rotation s for stop
       while(state == 'f'){
          digitalWrite(dxn1,HIGH);
          digitalWrite(dxn2,LOW);
          digitalWrite(dxn3,LOW);
          digitalWrite(dxn4,HIGH);
          analogWrite(pwm1,255);
          analogWrite(pwm2,255);
          analogWrite(pwm3,255);
          analogWrite(pwm4,255);
          goto test;
       }
     }
     if(state == 'b'){
       while(state == 'b'){
          digitalWrite(dxn1,LOW);
          digitalWrite(dxn2,HIGH);
          digitalWrite(dxn3,HIGH);
          digitalWrite(dxn4,LOW);
          analogWrite(pwm1,255);
          analogWrite(pwm2,255);
          analogWrite(pwm3,255);
          analogWrite(pwm4,255);
          goto test;
       }
     }
     if(state == 'l'){
       while(state == 'l'){
          digitalWrite(dxn1,HIGH);
          digitalWrite(dxn2,HIGH);
          digitalWrite(dxn3,LOW);
          digitalWrite(dxn4,LOW);
          analogWrite(pwm1,255);
          analogWrite(pwm2,255);
          analogWrite(pwm3,255);
          analogWrite(pwm4,255);
          goto test;
       }
     }
     if(state == 'r'){
       while(state == 'r'){
          digitalWrite(dxn1,LOW);
          digitalWrite(dxn2,LOW);
          digitalWrite(dxn3,HIGH);
          digitalWrite(dxn4,HIGH);
          analogWrite(pwm1,255);
          analogWrite(pwm2,255);
          analogWrite(pwm3,255);
          analogWrite(pwm4,255);
          goto test;
       }
     }
     if(state == '+'){
        while(state == '+'){
          digitalWrite(dxn1,HIGH);
          digitalWrite(dxn2,HIGH);
          digitalWrite(dxn3,HIGH);
          digitalWrite(dxn4,HIGH);
          analogWrite(pwm1,255);
          analogWrite(pwm2,255);
          analogWrite(pwm3,255);
          analogWrite(pwm4,255);
          goto test;
       }
     }
     if(state == '-'){
        while(state == '-'){
          digitalWrite(dxn1,LOW);
          digitalWrite(dxn2,LOW);
          digitalWrite(dxn3,LOW);
          digitalWrite(dxn4,LOW);
          analogWrite(pwm1,255);
          analogWrite(pwm2,255);
          analogWrite(pwm3,255);
          analogWrite(pwm4,255);
          goto test;
       }
     }
     if(state == 's'){ // breaking can be better if MAXMODE is used. As of now braking will not be very good.
      while(state == 's'){
          //digitalWrite(dxn1,LOW);
          //digitalWrite(dxn2,LOW);
          //digitalWrite(dxn3,HIGH);
          //digitalWrite(dxn4,HIGH);
          analogWrite(pwm1,0);
          analogWrite(pwm2,0);
          analogWrite(pwm3,0);
          analogWrite(pwm4,0);
          goto test;
       }
     }
    }
