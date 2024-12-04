/*
final_transmitter.ino
Code for the transmitter module of the GNAHS final car. In the
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
const byte addresses[][6] = { "node1" };

// Inputs from joystick
int x_in = A4;
int y_in = A5;

struct Data {
  int x, y;  // x and y inputs from joystcik
};

Data data;

void setup() {
  pinMode(x_in, INPUT);
  pinMode(y_in, INPUT);

  radio.begin();
  radio.openWritingPipe(addresses[0]);  // node1
  radio.setPALevel(RF24_PA_LOW);        // Power amplification, low for short range
  radio.stopListening();                // Trasmitter doesn't need to listen
  radio.flush_tx();                     // Make sure no residual in buffer

  Serial.begin(9600);
  if (!radio.isChipConnected()) {
    Serial.println("RF24 module is not connected properly!");
  } else {
    Serial.println("RF24 module connected successfully.");
  }
}

void loop() {
  data.x = analogRead(x_in);
  data.y = analogRead(y_in);

  radio.write(&data, sizeof(data));

  Serial.print(data.x);
  Serial.print(", ");
  Serial.println(data.y);

  delay(10);
}
