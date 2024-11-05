/*
prototype_motor_controller.ino
Code for the Arduino Uno that controls the motors of
  the GNAHS prototype. This Uno is not receiving
  any data wirelessly from the nRF24L01 modules, but
  is instead receiving data through a wired connection
  to a Nano that is receiving through the nRF24L01.
  The reason for the split between two Arduinos is
  interference that with Rev3 motor shield and the
  SPI pinout.
Author: Evan Dworkin
*/

/*
To-do:
  Test code with Nano receiver
*/

#include <Servo.h>

// Motor control pins
int servo_control_pin = 5;
int motor_pin_a = 3;
int motor_pin_b = 11;
int direction_pin_a = 12;
int direction_pin_b = 13;

// Data reception pins
int angle_receptor_pin = A5;
int duty_receptor_pin = A4;

// Read data from Nano receiver
int angle; // An angle value between 0-180
int duty; // A duty cycle value between -255-255

// Setup servo
Servo servo;

void setup() {
  pinMode(servo_control_pin, OUTPUT);
  pinMode(motor_pin_a, OUTPUT);
  pinMode(motor_pin_b, OUTPUT);
  pinMode(direction_pin_a, OUTPUT);
  pinMode(direction_pin_b, OUTPUT);

  pinMode(angle_receptor_pin, INPUT);
  pinMode(duty_receptor_pin, INPUT);

  servo.attach(servo_control_pin);
}

void loop() {
  // Map received values to appropriate output values
  angle = map(analogRead(angle_receptor_pin), 0, 255, 0, 180);
  duty = map(analogRead(duty_receptor_pin), 0, 255, -255, 255);

  // Write Servo motor
  servo.write(angle);

  // Write DC motor directions and duty cycles
  digitalWrite(direction_pin_a, duty > 0 ? HIGH : LOW); // set direction of DCs
  digitalWrite(direction_pin_b, duty > 0 ? HIGH : LOW); // set direction of DCs
  analogWrite(motor_pin_a, abs(duty)); // set duty cycle
  analogWrite(motor_pin_b, abs(duty)); // set duty cycle
}
