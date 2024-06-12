bool perdaAuditiva;

#include "display.h"
#include "player.h"
#include "button.h"

void setup() {
  Serial.begin(115200);
  softwareSerial.begin(9600);
  
  xTaskCreatePinnedToCore(task_button_green,"button_green", 2048, NULL, 2, NULL, 1);
  xTaskCreate(task_button_white,"button_white", 2048, NULL, 2, NULL);
  xTaskCreatePinnedToCore(task_display,"display", 2048, NULL, 2, NULL, 1);
}

void loop() {}
