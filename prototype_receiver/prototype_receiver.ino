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
To-do: Move data transmission to a single struct to avoid
any syncronization issues
*/

#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// See GNAHS documentation for the Arduino pinin and RF24L01 pinout
// Setup for radio
int CE = 7;
int CNS = 8;
RF24 radio(CE, CNS);
const byte addresses[][6] = {"node1"};

// Read data from receiver module
int x, y; // Raw data from transmitter (joystick values)
int angle_transfer, duty_transfer; // Values that can be trasmitted via PWM

void setup() {
  radio.begin();
  radio.openReadingPipe(1, addresses[0]); // node1
  radio.setPALevel(RF24_PA_LOW); // Power amplification, low for short range
  radio.startListening(); // Recevier needs to listen
  radio.flush_rx(); // Make sure no residual in buffer

  Wire.begin(); // Start I2C as master
}

void loop() {
  while (!radio.available());
  radio.read(&x, sizeof(x)); // read angle
  delay(5); // wait to let data arrive
  radio.read(&y, sizeof(y)); // read duty

  // Map values to a PWM-accessible value
  angle_transfer = map(x, 0, 1023, 0, 255);
  duty_transfer = map(y, 0, 1023, 0, 255);

  // Write the data to the I2C bus 8
  Wire.beginTransmission(8);
  Wire.write(angle_transfer);
  Wire.write(duty_transfer);
  Wire.endTransmission();
}
