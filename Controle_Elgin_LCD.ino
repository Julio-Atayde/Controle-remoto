/*
  Clonagem de controle remoto de ar-condicionado Elgin || Projeto desenvolvido no CEFET/RJ, sob orientação do Profº Valdinei ...

  Esse código consegue desligar/ligar, aumentar e diminuir a temperatura de um ar-condicionado.

  Circuito:
  - ESP8266 NodeMCU V3;
  - LED IR til32;
  - Display LCD 20x4;
  - Botões.
  
  Observações: 
  - Os pinos D6 e D7 não são comumente usados como I2C, mas fizemos assim para usar o LED IR em D2;
  - Os pinos D5 e D1 ficam em HIGH naturalmente e por isso acionamos os botões quando o valor estiver em LOW;
  - O pinos D8 fica em LOW naturalmente e por isso o botão é acionado quando o valor for HIGH;
  - Embora o ar-condicionado seja Elgin, usamos o protocolo TCL com base nos resultados obtidos no exemplo RecvDumpV3 de IRremoteESP8266;
  - A variavel state[14] é o resultado visto no exemplo citado acima. Após testes vimos que para o que queremos alguns bytes não mudam.
  - O 6° byte assume 0x20 para desligado e 0x24 para ligado;
  - O 7° byte determina o modo, usamos fixo em 0x03 para refrigerar;
  - O 8° byte determina a temperatura. Obedecendo o formato OxOX, onde X = 31 - temperatura;
  - O 9° byte determina a velocidade do ventilador, sendo 0x05 é mais alta;
  - O 13° byte determina o oscilar e a parte decimal da temperatura. Primeiro digito '8' oscilar desligado e 'A' para ligado. Segundo digito é '0' para inteiro e '8' para terminado em 0.5°C
  - O 14° byte é a soma dos valores anteriores, apenas dois digitos.

    Criado em 2025
    por Julio A. da Silva, Darlan C. de Araujo e Paulo A. de Moura
*/

#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Tcl.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ==== DISPLAY ====
LiquidCrystal_I2C lcd(0x27, 20, 4);

// ==== HARDWARE PINS ====
const uint8_t pinBotaoPower = D1;
const uint8_t pinBotaoAumentar = D5;
const uint8_t pinBotaoDiminuir = D8;
const uint16_t kIrLedPin = D2;

// ==== OBJETO CONTROLE IR ====
IRTcl112Ac ac(kIrLedPin);

// ==== VARIÁVEIS ====
int temperatura_atual = 20;
bool arLigado = false;
bool botaoPowerPressionado = false;
bool botaoMaisPressionado = false;
bool botaoMenosPressionado = false;

void setup() {
  Serial.begin(9600);
  ac.begin();
  Wire.begin(13, 12); // SDA = D7, SCL = D6

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Controle Elgin");

  pinMode(pinBotaoPower, INPUT_PULLUP);
  pinMode(pinBotaoAumentar, INPUT_PULLUP);
  pinMode(pinBotaoDiminuir, INPUT_PULLUP);
}

void loop() {
  lerBotaoPower();
  lerBotoesDigitais();
}

void lerBotaoPower() {
  if (digitalRead(pinBotaoPower) == LOW && !botaoPowerPressionado) {
    botaoPowerPressionado = true;
    alternarLigado();
    digitalWrite(pinBotaoDiminuir, HIGH);
  } else if (digitalRead(pinBotaoPower) == HIGH) {
    botaoPowerPressionado = false;
  }
}

void lerBotoesDigitais() {
  if (digitalRead(pinBotaoAumentar) == LOW && !botaoMaisPressionado) {
    botaoMaisPressionado = true;
    AjustarTemp(true, false);
  } else if (digitalRead(pinBotaoAumentar) == HIGH) {
    botaoMaisPressionado = false;
  }

  if (digitalRead(pinBotaoDiminuir) == HIGH && !botaoMenosPressionado) {
    botaoMenosPressionado = true;
    AjustarTemp(false, true);
  } else if (digitalRead(pinBotaoDiminuir) == LOW) {
    botaoMenosPressionado = false;
  }
}

// ==== LÓGICA ====
void alternarLigado() {
  arLigado = !arLigado;
  if (arLigado) ligar();
  else desligar();
}

void ligar() {
  uint8_t state[14] = {0x23, 0xCB, 0x26, 0x01, 0x00, 0x24, 0x03, 0x0B, 0x05, 0x00, 0x00, 0x00, 0x80, 0x00};
  state[7] = 31 - temperatura_atual;
  state[13] = calculaChecksum(state);
  ac.setRaw(state);
  ac.send();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ligado, Temp: ");
  lcd.print(temperatura_atual);
  lcd.print("C");
}

void desligar() {
  uint8_t state[14] = {0x23, 0xCB, 0x26, 0x01, 0x00, 0x20, 0x03, 0x0B, 0x05, 0x00, 0x00, 0x00, 0x80, 0x00};
  state[13] = calculaChecksum(state);
  ac.setRaw(state);
  ac.send();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Desligado");
}

void AjustarTemp(bool Tup, bool Tdown) {
  if (!arLigado) return;

  if (Tup && temperatura_atual < 31) temperatura_atual++;
  if (Tdown && temperatura_atual > 16) temperatura_atual--;

  uint8_t state[14] = {0x23, 0xCB, 0x26, 0x01, 0x00, 0x24, 0x03, (uint8_t)(31 - temperatura_atual), 0x05, 0x00, 0x00, 0x00, 0x88, 0x00};
  state[13] = calculaChecksum(state);
  ac.setRaw(state);
  ac.send();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp ajustada:");
  lcd.setCursor(0, 1);
  lcd.print(temperatura_atual);
  lcd.print(" C");
}

uint8_t calculaChecksum(uint8_t *dados) {
  uint16_t soma = 0;
  for (int i = 0; i < 13; i++) soma += dados[i];
  return (uint8_t)(soma & 0xFF);
}
 
