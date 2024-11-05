/*
prototype_receiver.ino
Code for the receiver module of the GNAHS prototype. In the
  implementation, this code is uploaded to an Arduino Nano
  connected with a nRF24L01 module. The information from this
  Nano is then fed to an Arduino Uno through the Uno's analog
  input pins. The reason for the separation is that the Rev3
  motor shield's pinout interferes with the SPI pinout of the
  Uno.
Author: Evan Dworkin
*/

#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/*
TO DO:
  Separate the receiver code and the control code. The reciever
  code will be uploaded to an Arduino Nano, then fed to an
  Arduino Uno that will control the motors
*/

// See GNAHS documentation for the Arduino pinin and RF24L01 pinout
// Setup for radio
int CE = 7;
int CNS = 8;
RF24 radio(CE, CNS);
const byte addresses[][6] = {"node1"};

// Setup servo
Servo servo;

// Motor control pins
int servo_control = 5;
int motor_pin_a = 3;
int motor_pin_b = 11;
int direction_pin_a = 12;
int direction_pin_b = 13;

// Read data from receiver
int angle, duty;

void setup() {
  pinMode(servo_control, OUTPUT);
  pinMode(motor_pin_a, OUTPUT);
  pinMode(motor_pin_b, OUTPUT);
  pinMode(direction_pin_a, OUTPUT);
  pinMode(direction_pin_b, OUTPUT);

  servo.attach(servo_control);

  radio.begin();
  radio.openReadingPipe(1, addresses[0]); // node1
  radio.setPALevel(RF24_PA_LOW);

  Serial.begin(9600);
  radio.startListening();
  radio.flush_rx();
}

void loop() {
  while (!radio.available());
  radio.read(&angle, sizeof(angle)); // read angle
  delay(5); // wait to let data arrive
  radio.read(&duty, sizeof(duty)); // read duty

  servo.write(angle);

  digitalWrite(direction_pin_a, duty > 0 ? HIGH : LOW); // set direction of DCs
  // digitalWrite(direction_pin_b, duty > 0 ? HIGH : LOW); // set direction of DCs
  analogWrite(motor_pin_a, abs(duty)); // set duty cycle
  // analogWrite(motor_pin_b, abs(duty)); // set duty cycle
}
