/*
escTest.ino
Test program to control ESC from PWM pin of Arduino Nano
The circuit consists of a potentiometer that is controlling
the PWM duty cycle being sent to the ESC
Author: Evan Dworkin
*/

#include <Servo.h>

/*
We are using the Servo library to control the esc
since Servos are designed in a similar way to take
PWM inputs. This will allow us to control the ESC
smoothly.
*/
Servo esc1;
Servo esc2;

// Min and max throttle values for the ESC
int MIN_THROTTLE = 1000;
int MAX_THROTTLE = 2000;

byte ESC1_pin = 3;  // sends PWM signal to motor 1
byte ESC2_pin = 5; // sends PWM signal to motor 2
int throttle; // a value between min and max throttle

byte potentiometer_pin = A0;  // reads potentiometer
int potentiometer_value;      // value between 0-1023


void setup() {
  pinMode(potentiometer_pin, INPUT);
  esc1.attach(ESC1_pin);
  esc2.attach(ESC2_pin);

  Serial.begin(9600);
}

void loop() {
  potentiometer_value = analogRead(potentiometer_pin);
  throttle = map(potentiometer_value, 0, 1023, MIN_THROTTLE, MAX_THROTTLE);
  esc1.writeMicroseconds(throttle);
  esc2.writeMicroseconds(throttle);
  Serial.println(throttle);
}
