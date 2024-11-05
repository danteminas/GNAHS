/*
wireless_test_receiver.ino
Sample receiver code to test nRF24L01 module. The Arduino receives
  the state of a button and turns on an LED accordingly
Author: Evan Dworkin, Will Shang
*/

#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


// See GNAHS documentation for the Arduino pinin and RF24L01 pinout
int CE = 7;
int CNS = 8;
RF24 radio(CE, CNS);
const byte addresses[][6] = {"node1"};
uint8_t buttonState = 0;

byte LED = 2;

void setup() {
  pinMode(LED, OUTPUT);

  radio.begin();
  radio.setChannel(108);
  radio.setAutoAck(true);
  radio.openReadingPipe(1, addresses[0]); // 00001
  radio.setPALevel(RF24_PA_LOW);

  Serial.begin(9600);
  radio.startListening();

}

void loop() {
  // if (!radio.isChipConnected()) {
  //   Serial.println("RF24 module is not connected properly!");
  // } else {
  //   Serial.println("RF24 module connected successfully.");
  // }
  if (radio.available()) {
    radio.read(&buttonState, sizeof(buttonState));
    // radio.flush_rx();
    digitalWrite(LED, buttonState);
    Serial.println(buttonState);
  } else {
    Serial.println("No data available");
  }
  delay(10);
}
