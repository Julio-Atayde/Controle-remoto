#include <IRremote.h>

int RECV_PIN = 2;     // Pino do receptor IR
int LED_PIN = 13;     // Pino do LED (pode ser qualquer pino digital)
unsigned long valorEsperado = 0xC13A01FE;  // Valor do código IR esperado

IRrecv irrecv(RECV_PIN);  // Objeto para recepção
decode_results results;   // Estrutura para armazenar o sinal recebido

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);  // Define o pino do LED como saída
  irrecv.enableIRIn();      // Ativa o receptor IR
  Serial.println("Pronto para receber sinais IR!");
}

void loop() {
  // Lê o sinal IR capturado
  if (irrecv.decode(&results)) {
    Serial.print("Sinal Recebido - Protocolo: ");
    
    // Exibe o protocolo
    switch (results.decode_type) {
      case NEC: Serial.println("NEC"); break;
      case SONY: Serial.println("Sony"); break;
      case SAMSUNG: Serial.println("Samsung"); break;
      case UNKNOWN: Serial.println("Desconhecido"); break;
      default: Serial.println("Outro"); break;
    }

    // Exibe o código capturado em hexadecimal
    Serial.print("Código (HEX): ");
    Serial.println(results.value, HEX);  

    // Verifica se o valor recebido é igual ao esperado
    if (results.value == valorEsperado) {
      digitalWrite(LED_PIN, HIGH);  // Acende o LED
      Serial.println("LED aceso: Código correto recebido!");
    } else {
      digitalWrite(LED_PIN, LOW);   // Desliga o LED
    }

    irrecv.resume();  // Aguarda o próximo sinal
  }
}
