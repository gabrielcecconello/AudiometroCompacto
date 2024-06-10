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

#ifndef CONFIRM_PIN
#define CONFIRM_PIN 18
#endif

#ifndef SELECT_PIN
#define SELECT_PIN 19
#endif

SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);
DFRobotDFPlayerMini df;
TaskHandle_t handlePlayFrequencies;

bool perdaAuditiva;
bool isButtonPressed;

void taskButtonState(void* pvParameters) {
  int buttonState;
  bool is_button_press = false;
  isButtonPressed = false;

  for(;;) {
    buttonState = digitalRead(CONFIRM_PIN);

    // Verifica se o botao foi apertado
    if(buttonState == HIGH && !is_button_press) {
      isButtonPressed = true;
    }
  }
}

void taskPlayFrequencies(void* pvParameters) {
  uint8_t currentTrack = 1;
  perdaAuditiva = false;
  int time;

  if(df.begin(softwareSerial)) {
    Serial.println("Connected!");
    df.volume(0);
    df.play(1);
    Serial.print("Playing Track: ");
    Serial.println(currentTrack);

    time = millis();

    for(;;) {
      if(millis() - time > 1000 || isButtonPressed) {
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
          vTaskDelete(handlePlayFrequencies);
        }

        // Proxima frequencia
        df.next();
        Serial.print("Playing Track: ");
        Serial.println(currentTrack);
      }

      // Aumenta o volume
      if (!isButtonPressed) df.volumeUp();
      Serial.println(df.readVolume());

      // vTaskDelay(100 / portTICK_PERIOD_MS);
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
  pinMode(CONFIRM_PIN, INPUT);
  pinMode(SELECT_PIN, INPUT);

  xTaskCreatePinnedToCore(taskPlayFrequencies, "Play Frequencies", 2048, NULL, 1, &handlePlayFrequencies, 0);
  xTaskCreatePinnedToCore(taskButtonState, "Check Button State", 2048, NULL, 1, NULL, 1);
}

void loop() {

}
