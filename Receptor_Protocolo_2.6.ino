#include <IRremote.h>

int RECV_PIN = 2;  // Pino do receptor IR
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();  // Inicia o receptor IR
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.print("Protocolo: ");
    switch (results.decode_type) {
      case NEC: Serial.println("NEC"); break;
      case SONY: Serial.println("Sony"); break;
      case SAMSUNG: Serial.println("Samsung"); break;
      case PANASONIC: Serial.println("Panasonic"); break;
      case LG: Serial.println("LG"); break;
      case UNKNOWN: Serial.println("Desconhecido"); break;
      default: Serial.println("Outro"); break;
    }

    Serial.print("Código (HEX): ");
    Serial.println(results.value, HEX);  // Exibe o código capturado em hexadecimal

    irrecv.resume();  // Aguarda o próximo sinal
  }
}
