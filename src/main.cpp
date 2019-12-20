#include <Arduino.h>
#include <IRremote.h>
#include <EEPROM.h>

const int ledPin = 13;
const int recvPin = 11;
const int XBianBootPin = 12;
const int bootPulseDuration = 10;
const int eepromAddress = 0;
const int waitingTime = 5000;
IRrecv irrecv(recvPin);
decode_results results;
unsigned long keyCodeFromEEPROM = 0;

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
  Serial.print("Waiting for key code change during ");
  Serial.print(waitingTime);
  Serial.println("ms...");
  while(millis() < waitingTime) {
    if(irrecv.decode(&results)) {
      if(results.value != 0xFFFFFFFF) {
        EEPROM.put(eepromAddress, results.value);
        Serial.print("Storing key code ");
        Serial.print(results.value, HEX);
        Serial.print(" in EEPROM at address ");
        Serial.println(eepromAddress);
        irrecv.resume();
        break;
      }
      irrecv.resume();
    }
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
  }
  Serial.println("Stop waiting");
  EEPROM.get(eepromAddress, keyCodeFromEEPROM);
  Serial.print("Using key code ");
  Serial.println(keyCodeFromEEPROM, HEX);
  Serial.println("Running main loop...");
  digitalWrite(ledPin, LOW);
}

void loop() {

  if(irrecv.decode(&results)) {
    if(results.value == keyCodeFromEEPROM) {
      bootXbBian();
      Serial.println("Booting XBian...");
      digitalWrite(ledPin, HIGH);
      delay(1000);
      digitalWrite(ledPin, LOW);
    }
    irrecv.resume();
  }
}