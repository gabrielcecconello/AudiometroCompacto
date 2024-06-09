#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

int display_opcao_selecionada;
int display_limite_opcao;
int display_interface;

Adafruit_SSD1306 display(-1);

void exibir_ajuda() {
  display_limite_opcao = 3;

  // Exibe o titulo
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.setTextColor(BLACK, WHITE);
  display.println(" *  Ajuda  * ");
  display.setTextColor(WHITE, BLACK);

  // Exibe as orientacoes
  if(display_opcao_selecionada == 0) {
    display.println("Pressione A para\nselecionar ou\nprosseguir");
  } else if(display_opcao_selecionada == 1) {
    display.println();
    display.println("Pressione B para\nnavegar ou voltar");
  } else if(display_opcao_selecionada == 2) {
    display.println("Para iniciar o teste,selecione qual orelhadeseja testar");
  } else if(display_opcao_selecionada >= 3) {
    display.println();
    display.println("E quando escutar um\nsom, pressione A");
  }
}

void exibir_menu_principal() {
  display_limite_opcao = 2;

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.setTextColor(BLACK, WHITE);
  display.println(" *  Audiometro  * ");
  display.println();

  if(display_opcao_selecionada == 1) {
    display.println("- Iniciar Teste");
    display.setTextColor(WHITE, BLACK);
  } else if(display_opcao_selecionada == 2) {
    display.setTextColor(WHITE, BLACK);
    display.println("- Iniciar Teste");
    display.setTextColor(BLACK, WHITE);
  } else {
    display.setTextColor(WHITE, BLACK);
    display.println("- Iniciar Teste");
  }

  display.println("- Ajuda");
}

void exibir_frequencia() {
  display_limite_opcao = 5;

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.setTextColor(BLACK, WHITE);
  display.println(" *  Teste  * ");

  display.setTextColor(WHITE, BLACK);
  display.println();

  if(display_opcao_selecionada == 0) {
    display.println("250 Hz");
  
  } else if(display_opcao_selecionada == 1) {
    display.println("500 Hz");
  
  } else if(display_opcao_selecionada == 2) {
    display.println("1000 Hz");
  
  } else if(display_opcao_selecionada == 3) {
    display.println("2000 Hz");
  
  } else if(display_opcao_selecionada == 4) {
    display.println("4000 Hz");
  
  } else if(display_opcao_selecionada == 5) {
    display.println("8000 Hz");
  }
}

void exibir_resultado_teste() {
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.setTextColor(BLACK, WHITE);
  display.println(" *  Resultado  * ");
  display.println();
}

void task_display(void *pvParameters) {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display_opcao_selecionada = 0;
  display_interface = 0;

  for(;;) {
    display.clearDisplay();

    if(display_interface == 0) {
      exibir_menu_principal();
    } else if(display_interface == 1) {
      exibir_ajuda();
    } else if(display_interface == 2) {
      exibir_frequencia();
    } else if(display_interface == 3) {
      exibir_resultado_teste();
    }

    display.display();
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}