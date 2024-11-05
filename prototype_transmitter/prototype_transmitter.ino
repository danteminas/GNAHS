/*
prototype_transmitter.ino
Code for the transmitter module of the GNAHS prototype. In the
  implementation, this code is uploaded to an Arduino Nano
  connected to a joystick and nRF24L01 module
Author: Evan Dworkin
*/

/*
To-do: Move data transmission to a single struct to avoid
any syncronization issues
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
  radio.setPALevel(RF24_PA_LOW); // Power amplification, low for short range
  radio.stopListening(); // Trasmitter doesn't need to listen
  radio.flush_tx(); // Make sure no residual in buffer

  Serial.begin(9600);
  if (!radio.isChipConnected()) {
    Serial.println("RF24 module is not connected properly!");
  } else {
    Serial.println("RF24 module connected successfully.");
  }
}

void loop() {
  x = analogRead(x_in);
  y = analogRead(y_in);

  radio.write(&x, sizeof(x));
  radio.write(&y, sizeof(y));

  Serial.print(x);
  Serial.print(", ");
  Serial.println(y);

  delay(10);
}
