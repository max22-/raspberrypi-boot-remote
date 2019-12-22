#include <Arduino.h>
#include <IRremote.h>

const int ledPin = 13;
const int recvPin = 11;
const int XBianBootPin = 12;
const int bootPulseDuration = 10;
IRrecv irrecv(recvPin);
decode_results results;
unsigned long keyCode1 = 0x3C, keyCode2 = 0x1003C;

void bootXbBian() {
  pinMode(XBianBootPin, OUTPUT);
  digitalWrite(XBianBootPin, LOW);
  delay(bootPulseDuration);
  pinMode(XBianBootPin, INPUT);
}

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Hello, world!");
  irrecv.enableIRIn();
  Serial.print("Using key codes ");
  Serial.print(keyCode1, HEX);
  Serial.print(", ");
  Serial.println(keyCode2, HEX);
  Serial.println("Running main loop...");
  digitalWrite(ledPin, LOW);
}

void loop() {

  if(irrecv.decode(&results)) {
    if(results.value == keyCode1 || results.value == keyCode2) {
      bootXbBian();
      Serial.println("Booting XBian...");
      digitalWrite(ledPin, HIGH);
      delay(1000);
      digitalWrite(ledPin, LOW);
    }
    else {
      Serial.print("Received key code ");
      Serial.println(results.value, HEX);
    }
    irrecv.resume();
  }
}