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
