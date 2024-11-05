/*
prototype_transmitter.ino
Code for the transmitter module of the GNAHS prototype. In the
  implementation, this code is uploaded to an Arduino Nano
  connected to a joystick and nRF24L01 module
Author: Evan Dworkin
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// See GNAHS documentation for the Arduino pinin and RF24L01 pinout
// Setup for radio pins
int CE = 7;
int CNS = 8;
RF24 radio(CE, CNS);
const byte addresses[][6] = {"node1"};

// Inputs from joystick
int x_in = A4;
int y_in = A5;

// Read inputs and convert to transmittable data
int x, y; // Read x and y inputs from joystick
int angle; // An angle value between 0-180
int duty; // A duty cycle value between -255-255

void setup() {
  pinMode(x_in, INPUT);
  pinMode(y_in, INPUT);

  radio.begin();
  radio.openWritingPipe(addresses[0]); // node1
  radio.setPALevel(RF24_PA_LOW);

  Serial.begin(9600);
  radio.stopListening();
  radio.flush_tx();
}

void loop() {
  x = analogRead(x_in);
  y = analogRead(y_in);

  angle = map(x, 0, 1023, 0, 180);
  duty = map(y, 0, 1023, -255, 255);

  radio.write(&angle, sizeof(angle));
  radio.write(&duty, sizeof(duty));

  delay(10);
}
