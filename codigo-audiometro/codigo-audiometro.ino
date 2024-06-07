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
TaskHandle_t handlePlayFrequencies;

bool perdaAuditiva;

void taskPlayFrequencies(void* pvParameters) {
  uint8_t currentTrack = 1;

  if(df.begin(softwareSerial)) {
    Serial.println("Connected!");
    df.volume(0);
    df.play(1);
    Serial.print("Playing Track ");
    Serial.println(currentTrack);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    for(;;) {
      if(digitalRead(BUSY_PIN) == HIGH) {
        if(df.readVolume() <= 4) perdaAuditiva = 0;
        else perdaAuditiva = 1;
        df.volume(0);
        currentTrack += 1;
        if (currentTrack > 6) {
          Serial.println("Finished Frequencies.");
          vTaskDelete(handlePlayFrequencies);
        }
        df.next();
        Serial.print("Playing Track ");
        Serial.println(currentTrack);
      }
      df.volumeUp();
      Serial.println(df.readVolume());
      vTaskDelay(1500 / portTICK_PERIOD_MS);
    }
  }
  else {
    Serial.println("Connection to DFPlayer Mini failed!");
  }
}

void setup() {
  Serial.begin(9600);
  softwareSerial.begin(9600);
  
  pinMode(BUSY_PIN, INPUT);
  xTaskCreatePinnedToCore(taskPlayFrequencies, "Play Frequencies", 2048, NULL, 1, &handlePlayFrequencies, 0);

}

void loop() {


}
