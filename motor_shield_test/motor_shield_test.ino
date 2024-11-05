/*
motor_shield_test.ino
Code to test the functionality of the Arduino Rev3 Motor Shield
  in conjuction with the Arduino Uno. This program controls a
  DC motor with the Y-axis of a joystick and a servo motor with
  the X-axis of a joystick.
Author: Evan Dworkin
*/

#include <Servo.h>

int DIRECTIONPIN = 12;
int PWMPIN = 3;
int BRAKEPIN = 9;
int SERVO_CONTROL = 5;
int XIN = A5;
int YIN = A4;
// int SWTCH = A3;

int y;
int x;
int duty;

Servo servo;

//uncomment if using channel B
//int DIRECTIONPIN = 13;
//int PWMPIN = 11;
//int BRAKEPIN = 8;

void setup() {
  //define pins
  pinMode(DIRECTIONPIN, OUTPUT);
  pinMode(PWMPIN, OUTPUT);
  pinMode(BRAKEPIN, OUTPUT);
  pinMode(XIN, INPUT);
  pinMode(YIN, INPUT);
  // pinMode(SWTCH, INPUT);

  servo.attach(SERVO_CONTROL);

  Serial.begin(9600);
}

void loop() {
  // check for brake;
  // digitalWrite(BRAKEPIN, digitalRead(SWTCH));

  // set servo
  x = analogRead(XIN);
  x = map(x, 0, 1023, 180, 0);
  servo.write(x);

  // set duty cycle (0-100)
  y = analogRead(YIN);
  duty = map(y, 0, 1023, -255, 255);
  Serial.println(duty);
  digitalWrite(DIRECTIONPIN, duty > 0 ? HIGH : LOW);
  analogWrite(PWMPIN, abs(duty));
}