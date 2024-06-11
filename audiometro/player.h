#include "DFRobotDFPlayerMini.h"
#include "SoftwareSerial.h"

#define PIN_MP3_TX 16
#define PIN_MP3_RX 17
#define BUSY_PIN 34

DFRobotDFPlayerMini df;
TaskHandle_t handlePlayFrequencies;
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);

bool perdaAuditiva;
bool isButtonPressed;

void taskPlayFrequencies(void* pvParameters) {
  uint8_t currentTrack = 1;
  int time_flag1;
  int time_flag2;
  int time_now;

  perdaAuditiva = false;

  pinMode(BUSY_PIN, INPUT);

  while(!df.begin(softwareSerial)) {
    Serial.println("Connection to DFPlayer Mini failed!");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  
  Serial.println("Connected!");
  df.play(1);
  df.next();
  df.previous();
  df.volume(0);
  Serial.print("Playing Track: ");
  Serial.println(currentTrack);

  time_flag1 = millis();
  time_flag2 = millis();

  for(;;) {
    time_now = millis();

    if(time_now - time_flag1 > 30000 || isButtonPressed) {
      if(df.readVolume() > 4 && !isButtonPressed) perdaAuditiva = true;

      // Toca o proximo audio
      df.volume(0);
      currentTrack += 1;
      isButtonPressed = false;

      // Verifica se ha audio
      if (currentTrack > 6) {
        Serial.println("Finished Frequencies.");
        if (perdaAuditiva) Serial.println("Unfortunately, you have some kind of hearing loss."); 
        else Serial.println("You're hearing is all good!");
        df.pause();
        vTaskDelete(handlePlayFrequencies);
      }

      // Proxima frequencia
      df.next();
      time_flag1 = millis();
      Serial.print("Playing Track: ");
      Serial.println(currentTrack);
    }

    // Aumenta o volume
    if(time_now - time_flag2 > 1000) {
      if (!isButtonPressed) df.volumeUp();
      Serial.print("volume: ");
      Serial.println(df.readVolume());
      time_flag2 = millis();
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }

  vTaskDelete(handlePlayFrequencies);
}
