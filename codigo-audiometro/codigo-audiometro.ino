#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#include "HardwareSerial.h"
HardwareSerial hardwareSerial(2);

#define PIN_MP3_TX 16
#define PIN_MP3_RX 17
#define BUSY_PIN 34

#ifndef CONFIRM_PIN
#define CONFIRM_PIN 19
#endif

SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);
DFRobotDFPlayerMini df;
TaskHandle_t handlePlayFrequencies;

int buttonState = 0;
bool perdaAuditiva = 0;
bool isButtonPressed = 0;

void taskButtonState(void* pvParameters) {
  for(;;) {
    buttonState = digitalRead(CONFIRM_PIN);
    if(buttonState == HIGH && !isButtonPressed) {
      isButtonPressed = 1;
    } 
    else if(buttonState == LOW && isButtonPressed) {
       isButtonPressed = 0;
    }
  }
}

void taskPlayFrequencies(void* pvParameters) {
  uint8_t currentTrack = 1;

  while(!df.begin(softwareSerial)) {
    Serial.print("player not connect");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }

  Serial.println("Connected!");
  df.volume(0);
  df.play(1);
  Serial.print("Playing Track ");
  Serial.println(currentTrack);

  for(;;) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    if(digitalRead(BUSY_PIN) || isButtonPressed) {
      if(df.readVolume() > 4) perdaAuditiva = 1;
      df.volume(0);
      currentTrack += 1;
      if (currentTrack > 6) {
        Serial.println("Finished Frequencies.");
        if (perdaAuditiva) Serial.println("Unfortunately, you have some kind of hearing loss."); 
        else Serial.println("You're hearing is all good!");
        vTaskDelete(handlePlayFrequencies);
      }
      df.next();
      Serial.print("Playing Track ");
      Serial.println(currentTrack);
    }
    if (!isButtonPressed) df.volumeUp();
    Serial.println(df.readVolume());
  }

  Serial.println("Connection to DFPlayer Mini failed!");
  vTaskDelete(handlePlayFrequencies);
}

void setup() {
  Serial.begin(115200);
  softwareSerial.begin(9600);
  
  pinMode(BUSY_PIN, INPUT);
  pinMode(CONFIRM_PIN, INPUT);
  pinMode(SELECT_PIN, INPUT);

  xTaskCreate(taskPlayFrequencies, "Play Frequencies", 2048, NULL, 1, &handlePlayFrequencies);
  xTaskCreate(taskButtonState, "Check Button State", 2048, NULL, 1, NULL);
}

void loop() {

}
