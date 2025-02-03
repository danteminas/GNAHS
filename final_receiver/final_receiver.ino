/*
final_receiver.ino
Code for the final receiver module of the GNAHS prototype.
This implementation uses Electronic Speed Controllers (ESCs)
to control the motors, and therefore we no longer need to
split the onboard control into an Uno and a Nano.
Author: Evan Dworkin
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

// See GNAHS documentation for the Arduino pinin and RF24L01 pinout
// Setup for radio
int CE = 7;
int CNS = 8;
RF24 radio(CE, CNS);
const byte addresses[][6] = {"node1"};

// Constants
const int ANGLE__OF_ROTATION = 25;
const int SERVO_MIN_ANGLE = 90 - ANGLE__OF_ROTATION;
const int SERVO_MAX_ANGLE = 90 + ANGLE__OF_ROTATION;
const int ESC_MIN_MICROSECONDS = 1000;
const int ESC_MAX_MICROSECONDS = 2000;

// Treat the ESCs as Servo motors for handy controls
Servo servo;
Servo esc1;
Servo esc2;

int servo_pin = 3;
int esc1_pin = 6;
int esc2_pin = 10;

// Read data from receiver module
struct Data {
  int x, y; // x and y inputs from joystcik
};
Data data; // Raw data from transmitter (joystick values)
int angle, duty; // Final angle and duty values

void setup() {
  radio.begin();
  radio.openReadingPipe(1, addresses[0]); // node1
  radio.setPALevel(RF24_PA_LOW); // Power amplification, low for short range
  radio.startListening(); // Recevier needs to listen
  radio.flush_rx(); // Make sure no residual in buffer

  Serial.begin(9600);

  servo.attach(servo_pin);
  esc1.attach(esc1_pin);
  esc2.attach(esc2_pin);

  if (!radio.isChipConnected()) {
    Serial.println("RF24 module is not connected properly!");
  } else {
    Serial.println("RF24 module connected successfully.");
  }
}

void loop() {
  // Serial.println("No data available");
  if (radio.available()) {
    radio.read(&data, sizeof(data)); // read data

    // Map values to a PWM-accessible value
    angle = map(data.x, 0, 1023, SERVO_MAX_ANGLE, SERVO_MIN_ANGLE);
    duty = map(data.y, 0, 1023, ESC_MAX_MICROSECONDS, 0);

    servo.write(angle);
    esc1.writeMicroseconds(duty);
    esc2.writeMicroseconds(duty);

    // Print values to transfer
    // Serial.println("Data received");
    Serial.print(duty);
    Serial.print(", ");
    Serial.println(angle);
  }
}
