/*
  Clonagem de controle remoto TV gradiente || Projeto desenvolvido no CEFET/RJ campus Nova Iguaçu. Sob supervisão do Profº Valdinei ...

  O projeto reproduz algumas funções do controle remoto de um TV gradiente. Para isso foi preciso usar um módulo TSOP1838 para capturar os valores.

  Ciruito:
  - Arduino Uno;
  - LED IR til32;
  - OBS.: O código é um teste do LED IR, por isso não usa botões e sim o prórpio teclado. Obrigatóriamente o LED_IR tem que estar em D3, pino com PWM
  Criado em 2025
  por Julio A. da Silva, Darlan C. de Araujo e Paulo A. de Moura

*/  
#include <IRremote.h>

int LED_IR = 3;   // Pino do LED IR (Transmissor)
IRsend irsend;    // Objeto para transmissão IR

void setup() {
  Serial.begin(9600);
  Serial.println("Pronto para transmitir sinais IR!");
}

void loop() {
  // Aguarda um comando no Monitor Serial para enviar o sinal IR
  if (Serial.available()) {  
    char comando = Serial.read();
    if (comando == 'a') {  // Pressione "s" no Serial Monitor para enviar
      Serial.println("Power...");
      irsend.sendNEC(0xC13A01FE, 32);  // Envia o código NEC de 32 bits // power
      delay(500);  // Atraso para garantir que o sinal foi transmitido
    }
    if (comando == 's') {  // Pressione "s" no Serial Monitor para enviar
      Serial.println("Ir para o próximo canal...");
      irsend.sendNEC(0xC13A21DE, 32);  // Envia o código NEC de 32 bits // passar canal
      delay(500);  // Atraso para garantir que o sinal foi transmitido
    }
    if (comando == 'd') {  // Pressione "s" no Serial Monitor para enviar
      Serial.println("Voltar um canal...");
      irsend.sendNEC(0xC13AA15E, 32);  // Envia o código NEC de 32 bits // voltar canal
      delay(500);  // Atraso para garantir que o sinal foi transmitido
    }
    if (comando == 'f') {  // Pressione "s" no Serial Monitor para enviar
      Serial.println("Aumentar volume...");
      irsend.sendNEC(0xC13A7986, 32);  // Envia o código NEC de 32 bits // aumentar volume
      delay(500);  // Atraso para garantir que o sinal foi transmitido
    }
    if (comando == 'g') {  // Pressione "s" no Serial Monitor para enviar
      Serial.println("Diminuir volume...");
      irsend.sendNEC(0xC13AF906, 32);  // Envia o código NEC de 32 bits // diminuir volume
      delay(500);  // Atraso para garantir que o sinal foi transmitido
    }

  }
}
