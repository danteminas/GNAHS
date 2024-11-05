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

/*
To-do:
  Test code, interate as necessary
*/

#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// See GNAHS documentation for the Arduino pinin and RF24L01 pinout
// Setup for radio
int CE = 7;
int CNS = 8;
RF24 radio(CE, CNS);
const byte addresses[][6] = {"node1"};

// Data transfer pins to Uno motor controller
int angle_transfer_pin = A0;
int duty_transfer_pin = A1;

// Read data from receiver module
int angle; // An angle value between 0-180
int duty; // A duty cycle value between -255-255
int angle_transfer, duty_transfer; // Values that can be trasmitted via PWM

void setup() {
  pinMode(angle_transfer_pin, OUTPUT);
  pinMode(duty_transfer_pin, OUTPUT);
  
  radio.begin();
  radio.openReadingPipe(1, addresses[0]); // node1
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
  radio.flush_rx();
}

void loop() {
  while (!radio.available());
  radio.read(&angle, sizeof(angle)); // read angle
  delay(5); // wait to let data arrive
  radio.read(&duty, sizeof(duty)); // read duty

  // Map values to a PWM-accessible value
  angle_transfer = map(angle, 0, 180, 0, 255);
  duty_transfer = map(duty, -255, 255, 0, 255);

  // Write the data to the Uno motor controller
  analogWrite(angle_transfer_pin, angle_transfer);
  analogWrite(duty_transfer_pin, duty_transfer);
}
