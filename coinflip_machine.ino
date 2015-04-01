/*
program for the coin flip machine
 author: kevinschiffer
 This program will control the coin flip machine. 
 It will listen for button presses and trigger the
 spring servo. The result is then read via webcam
 on the raspberry pi and received by the arduino
 which will flip the flipdot accordingly.
 */
int flip=11;                    // dot direction one Pin
int flop=10;                    // dot direction two Pin
int motor01=6;
int motor02=5;
int threshold = 0;
int flippery = false;
boolean pressed = false;
boolean flipped = false;
boolean process = false;
boolean calibrating = true;
int calibratingTime = 90000;

void setup(){
  pinMode(flip,OUTPUT);        // pin A declared as OUTPUT
  pinMode(flop,OUTPUT);        // pin B declared as OUTPUT
  pinMode(13,OUTPUT);
  Serial.begin(9600);
}
void loop(){

  if (calibrating) {
    digitalWrite(13,HIGH);
    digitalWrite(motor01,HIGH);
    digitalWrite(motor02,LOW);
    if (analogRead(3) > threshold){
      threshold = analogRead(3);
    }
    if (millis() >= 4000) {
      calibrating = false;
      digitalWrite(13,LOW);
      threshold = threshold - 8;
      //Serial.print("Calibration Complete - ");
      //Serial.println(threshold);
      process = true;
    }
    calibratingTime = calibratingTime - 1;
    //Serial.print("Calibrating... ");
    //Serial.println(threshold);
    return;
  }
  
  if (digitalRead(2) == HIGH){
    pressed = true;
  } else {
    pressed = false;
  }
  
  if (pressed && !process){
    process = true;
    digitalWrite(motor01,HIGH);
    digitalWrite(motor02,LOW);
    delay(1000);
  }
  
  if (process && analogRead(3) < threshold){
    digitalWrite(motor01,HIGH);
    digitalWrite(motor02,LOW);
    /*if (!flippery && (millis()%200) == 0){
      flipdot(true);
      flippery = true;
    } else {
      if (flippery && (millis()%200) == 0){
        flipdot(false);
        flippery = false;
      }
    }*/
    
  }
  
  if (process && analogRead(3) > threshold) {
    //Serial.println('STOP!');
    Serial.print(0);
    process = false;
    digitalWrite(motor01,LOW);
    digitalWrite(motor02,LOW);
  }
  
/*  if (!pressed && flipped){
    digitalWrite(motor01,LOW);
    digitalWrite(motor02,LOW);
    
    digitalWrite(flip,LOW);     // dot direction one pin switched to HIGH (GND)
    digitalWrite(flop,HIGH);  // dot direction two pin switched to LOW  (+5V)
    delay(10);
    digitalWrite(flop,LOW);
    flipped = false;
  }*/
}

void flipdot(boolean flippy){
if (flippy){
    digitalWrite(flip,HIGH);
    digitalWrite(flop,LOW);
    delay(10);
    digitalWrite(flip,LOW);
  } else {
    digitalWrite(flip,LOW);
    digitalWrite(flop,HIGH);
    delay(10);
    digitalWrite(flop,LOW);
  }
}

void serialEvent() {
  while (Serial.available() > 0) {
    // get the new byte:
    int inMessage = Serial.read();
    //Serial.println(inMessage);
    if (inMessage == 48) {
      digitalWrite(flip,HIGH);
      digitalWrite(flop,LOW);
      delay(10);
      digitalWrite(flip,LOW);
    }
    if (inMessage == 49){
      digitalWrite(flip,LOW);
      digitalWrite(flop,HIGH);
      delay(10);
      digitalWrite(flop,LOW);
    }
  }
}



