#include <Wire.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;
int forwards = 7;
int backwards = 8;
int pushButton = 2;

void setup() {

  Serial.begin(9600);

  while (!Serial) { // needed to keep leonardo/micro from starting too fast!
  delay(10);
  }
  
  Serial.println("Adafruit MPR121 Capacitive Touch sensor test"); 
  
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");


pinMode(forwards,OUTPUT); //initialises forward as an output
pinMode(backwards,OUTPUT); //initialises backwards as an output
pinMode(pushButton,INPUT); //initialises button press as an input

}

void loop() {
    int buttonState = digitalRead(pushButton);
    Serial.println(buttonState);
    currtouched = cap.touched();

    if ((currtouched & _BV(4)) && !(lasttouched & _BV(4)) ) { //if pin 4 has been touched 
      Serial.print(4); Serial.println(" touched");
      digitalWrite(forwards, LOW); 
      digitalWrite(backwards, HIGH); // extend the linear actuator
    }
     if (!(currtouched & _BV(4)) && (lasttouched & _BV(4)) ) { //if pin 4 has been released
    Serial.print(4); Serial.println(" released");
     digitalWrite(forwards, HIGH);
     digitalWrite(backwards, HIGH); //stop the linear actuator
    }
    if (buttonState == 1) { //if the button has been pressed
      Serial.print("restart");
      digitalWrite(forwards, HIGH);
      digitalWrite(backwards, LOW); //return the linear actuator to its original position
    }
    lasttouched = currtouched;

}
