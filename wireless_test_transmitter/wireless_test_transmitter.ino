/*
wireless_test_transmitter.ino
Sample receiver code to test nRF24L01 module. The transmitter
  transmits the state of a button for a receiver to turn on
  an LED
Author: Evan Dworkin, Will Shang
*/

#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// See GNAHS documentation for the Arduino pinin and RF24L01 pinout
int CE = 7;
int CNS = 8;
int BUTTON = A4;
RF24 radio(CE, CNS);
const byte addresses[][6] = {"node1"};
uint8_t buttonState = 0;

void setup() {
  pinMode(BUTTON, INPUT_PULLUP);

  radio.begin();
  radio.setChannel(108);
  radio.setAutoAck(true);
  radio.openWritingPipe(addresses[0]); // 00001
  radio.setPALevel(RF24_PA_LOW);

  Serial.begin(9600);
  radio.stopListening();

}

void loop() {
  // if (!radio.isChipConnected()) {
  //   Serial.println("RF24 module is not connected properly!");
  // } else {
  //   Serial.println("RF24 module connected successfully.");
  // }
  buttonState = !digitalRead(BUTTON);

  // radio.flush_tx();
  if (radio.write(&buttonState, sizeof(buttonState))) {
    Serial.println(buttonState);
  } else {
    Serial.println("No data sent");
  }
  delay(10);
}
