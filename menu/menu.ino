#include "display.h"
#include "button.h"

void setup() {
  Serial.begin(115200);
  xTaskCreate(task_button_green,"button_green", 2048, NULL, 2, NULL);
  xTaskCreate(task_button_white,"button_white", 2048, NULL, 2, NULL);
  xTaskCreate(task_display,"display", 2048, NULL, 2, NULL);
}

void loop() {}
