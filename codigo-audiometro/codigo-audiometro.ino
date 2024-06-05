#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#ifndef PIN_MP3_TX
#define PIN_MP3_TX 17
#endif

#ifndef PIN_MP3_RX
#define PIN_MP3_RX 16
#endif

#ifndef BUSY_PIN
#define BUSY_PIN 5
#endif

SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);
DFRobotDFPlayerMini df;

void taskPlayLeftFrequencies(void* pvParameters) {
  pinMode(BUSY_PIN, INPUT);

  if(df.begin(softwareSerial)) {
    Serial.println("Connected!");
    df.playFolder(1, 1);
    df.volume(0);
    for(;;) {
      if(digitalRead(BUSY_PIN) == HIGH) {
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        df.volume(0);
        df.next();
        df.play();
      }
      df.volumeUp();
      vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
  }
  else {
    Serial.println("Connection to DFPlayer Mini failed!");
  }
}

void taskPlayRightFrequencies(void* pvParameters) {
  pinMode(BUSY_PIN, INPUT);

  if(df.begin(softwareSerial)) {
    Serial.println("Connected!");
    df.playFolder(2, 1);
    df.volume(0);
    for(;;) {
      if(digitalRead(BUSY_PIN) == HIGH) {
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        df.volume(0);
        df.next();
        df.play();
      }
      df.volumeUp();
      vTaskDelay(2000 / portTick_PERIOD_MS);
    }
  }
  else {
    Serial.println("Connection to DFPlayer Mini failed!");
  }
}

void setup() {
  Serial.begin(9600);
  softwareSerial.begin(9600);

}

void loop() {


}
