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
To-do: Find a separate battery to power the DC motors;
  the 9V battery is insufficient
*/

#include <Servo.h>
#include <Wire.h>

// Motor control pins
int servo_control_pin = 5;
int motor_pin_a = 3;
int motor_pin_b = 11;
int direction_pin_a = 12;
int direction_pin_b = 13;

// Read data from Nano receiver
int angle_read, duty_read; // Raw data reception through I2C
int angle; // An angle value between 0-180
int duty; // A duty cycle value between -255-255

// Instantiate servo
Servo servo;

void setup() {
  pinMode(servo_control_pin, OUTPUT);
  pinMode(motor_pin_a, OUTPUT);
  pinMode(motor_pin_b, OUTPUT);
  pinMode(direction_pin_a, OUTPUT);
  pinMode(direction_pin_b, OUTPUT);

  Wire.begin(8); // Join I2C bus with address 8
  Wire.onReceive(receiveEvent); // Set instructions on how to handle events

  servo.attach(servo_control_pin); // Initialize servo to control pin

  Serial.begin(9600);
}

void loop() {
  // Map received values to appropriate output values
  angle = map(angle_read, 0, 255, 0, 180);
  duty = map(duty_read, 0, 255, -255, 255);

  // Write Servo motor
  servo.write(angle);

  // Write DC motor directions and duty cycles
  digitalWrite(direction_pin_a, duty > 0 ? HIGH : LOW); // set direction of DC A
  digitalWrite(direction_pin_b, duty > 0 ? LOW : HIGH); // set direction of DC B
  analogWrite(motor_pin_a, abs(duty)); // set duty cycle A
  analogWrite(motor_pin_b, abs(duty)); // set duty cycle B

  // Print values
  Serial.print(angle);
  Serial.print(", ");
  Serial.println(duty);
}

void receiveEvent(int bytes) {
  if (Wire.available() >= 2) {
    angle_read = Wire.read();
    duty_read = Wire.read();
  }
}
