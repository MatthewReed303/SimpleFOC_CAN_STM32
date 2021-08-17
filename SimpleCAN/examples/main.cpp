#include <Arduino.h>
#include "board_variant.h"
#include <SimpleCAN.h>
//#include "can_helper.h"

void handleCanMessage(can_message_t *message)
{
  Serial.print("message can received!: "); Serial.println(message->data[0]);
  digitalWrite(LED_GREEN, !digitalRead(LED_GREEN));
}

SimpleCAN can;

void setup() {
  
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  
  Serial.begin(115200);
  delay(2000);
  Serial.println("Setup");
  printClockSpeed();

  pinMode(PC13,OUTPUT);
  can.init(CAN_RX, CAN_TX, BAUD_250K, CanMode::NormalCAN);

  printCanSpeed();

  can.filterAcceptAll();
  can.subscribe(&handleCanMessage);
  can.begin();
}

void poll() {
  delay(100);
  static can_message_t message = {};
  if (can.receive(&message)==CAN_OK) {
    Serial.print(message.data[0]);
  } else {
    Serial.print(".");
  }
}

void transmit() {
  delay(1000);
  digitalWrite(LED_RED, !digitalRead(LED_RED));
  
  static uint8_t data[2];
  data[0]++;
  data[0]++;
  
  can_message_t msg = createStandardMessage(0x244, data, 2);
  Serial.println("Send");
  
  can.transmit(&msg);
}

void loop() {
  
  // subscribe() in setup means that handleCanMessage will be called
  // poll();

  transmit();
}
