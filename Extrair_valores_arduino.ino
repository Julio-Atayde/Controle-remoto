/*
  Clonagem de controle remoto de TV gradiente || Projeto desenvolvido no CEFET/RJ campus Nova Iguaçu, sob a supervisão do profº Valdinei

  Esse código serve pra obter os valores que serão enviados pelo LED IR til32 para controlar a TV

  Circuito:
  - Arduino UNO;
  - TSOP1838;
  - OBS.: O TSOP1838 tem que estar conectado a D2.

  Criado em 2025
  por Julio A. da Silva, Darlan C. de Araujo e Paulo A. de Moura
*/

#include <IRremote.h>

int RECV_PIN = 2;  // Pino onde o sensor IR está conectado
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();  // Inicia o receptor
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);  // Mostra o código do botão em Hexadecimal
    irrecv.resume();  // Prepara para receber o próximo sinal
  }
}
