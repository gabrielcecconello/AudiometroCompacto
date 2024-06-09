#define PIN_BUTTON_GREEN 19
#define PIN_BUTTON_WHITE 5

void task_button_green(void *pvParameters) {
  int button_state = 0;
  bool is_button_press = false;
  pinMode(PIN_BUTTON_GREEN, INPUT);
  
  for(;;) {
    button_state = digitalRead(PIN_BUTTON_GREEN);
    if(button_state == HIGH && !is_button_press) {
      is_button_press = true;

      if(display_interface == 0) {
        if(display_opcao_selecionada == 1) {
          display_interface = 2;
          display_opcao_selecionada = 0;
        
        } else if(display_opcao_selecionada == 2) {
          display_interface = 1;
          display_opcao_selecionada = 0;
        }
      
      } else if(display_interface == 1) {
        if(display_opcao_selecionada < display_limite_opcao) {
          display_opcao_selecionada ++;
        } else {
          display_interface = 0;
          display_opcao_selecionada = 0;
        }
      
      } else if(display_interface == 2) {
        if(display_opcao_selecionada < display_limite_opcao) {
          display_opcao_selecionada ++;
        } else {
          display_interface = 3;
        }
      
      } else if (display_interface == 3) {
        display_interface = 0;
        display_opcao_selecionada = 0;
      }

    } else if(button_state == LOW && is_button_press) {
      is_button_press = false;
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void task_button_white(void *pvParameters) {
  int button_state = 0;
  bool is_button_press = false;
  pinMode(PIN_BUTTON_WHITE, INPUT);
  
  for(;;) {
    button_state = digitalRead(PIN_BUTTON_WHITE);
    
    if(button_state == HIGH && !is_button_press) {
      is_button_press = true;

      if(display_interface == 0) {
        if(display_opcao_selecionada < display_limite_opcao) {
          display_opcao_selecionada++;
        } else {
          display_opcao_selecionada = 1;
        }
        
      } else if(display_interface == 1) {
        if(display_opcao_selecionada > 0) {
          display_opcao_selecionada--;
        } else {
          display_interface = 0;
          display_opcao_selecionada = 0;
        }
        
      } else if (display_interface == 3) {
        display_interface = 0;
        display_opcao_selecionada = 0;
      }

    } else if(button_state == LOW && is_button_press) {
      is_button_press = false;
    }

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}