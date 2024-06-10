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

DFRobotDFPlayerMini df;
TaskHandle_t handlePlayFrequencies;
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);

bool perdaAuditiva;
bool isButtonPressed;

void taskButtonState(void* pvParameters) {
  int buttonState;
  bool is_button_press = false;
  isButtonPressed = false;

  pinMode(CONFIRM_PIN, INPUT);

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
  int time_flag1;
  int time_flag2;
  int time_now;

  pinMode(SELECT_PIN, INPUT);
  pinMode(BUSY_PIN, INPUT);
  
  if(df.begin(softwareSerial)) {  
    Serial.println("Connected!");
    df.volume(0);
    df.play(1);
    Serial.print("Playing Track: ");
    Serial.println(currentTrack);

    time_flag1 = millis();
    time_flag2 = millis();

    for(;;) {
      time_now = millis();

      //TODO: ajustar tempo de duracao de cada som
      if(time_now - time_flag1 > 9000 || isButtonPressed) {
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
        time_flag1 = millis();
        Serial.print("Playing Track: ");
        Serial.println(currentTrack);
      }

      // Aumenta o volume
      if(time_now - time_flag2 > 1000) {
        if (!isButtonPressed) df.volumeUp();
        Serial.println(df.readVolume());
        time_flag2 = millis();
      }

      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }
  else {
    Serial.println("Connection to DFPlayer Mini failed!");
  }
}

void setup() {
  Serial.begin(9600);
  softwareSerial.begin(9600);
  
  xTaskCreatePinnedToCore(taskPlayFrequencies, "Play Frequencies", 2048, NULL, 1, &handlePlayFrequencies, 0);
  xTaskCreatePinnedToCore(taskButtonState, "Check Button State", 2048, NULL, 1, NULL, 1);
}

void loop() {}
