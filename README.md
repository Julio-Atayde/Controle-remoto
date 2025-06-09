# Controle remoto
Elaboração de um controle remoto usando Arduino
Elaboração de um controle remoto usando ESP8266, com Wi-Fi

O projeto consiste em recriar um controle remoto usando o ESP8266.
Primeiro, buscamos entender como funciona a comuniação via IR entre uma TV e o seu controle remoto.
Com esse entendimento, recriamos o processo usando um til32 (LED transmissor IR) controlado por um Arduino.
Funcionando com a TV, tentamos replicar esse processo com um ar-condicionado, sem sucesso inicialmente.
Diante do fracasso de replicar o processo com Arduino, tentamos comunicação em raw. Mas, não tivemos sucesso.
Ler o sinal IR em raw é dificil porque o resultado era uma sequência de 455 valores. Criamos um programa em python pra tentar enxegar os padrões. Mas não tivemos sucesso.
Então, decidimos usar o ESP8266 por ter mais memória e usar uma biblioteca diferente. Ao usar o exemplo RecvDumpV3 conseguimos, visualizar uma resposta muito útil para nós. Resposta abaixo:
Protocol  : TCL112AC
  Code      : 0x23CB26010020030B0500000080C8 (112 Bits)
  Mesg Desc.: Model: 1 (TAC09CHSD), Type: 1, Power: Off, Mode: 3 (Cool), Temp: 20C, Fan: 5 (High), Swing(V): 0 (Auto), Swing(H): Off, Econo: Off, Health: Off, Turbo: Off, Light: On, On Timer: Off, Off Timer: Off
  uint16_t rawData[227] = {3082, 1594,  492, 1092,  492, 1092,  492, 326,  492, 326,  492, 326,  494, 1092,  492, 326,  492, 326,  492, 1092,  492, 1090,  494, 326,  492, 1092,  492, 326,  492, 326,  492, 1092,  492, 1096,  490, 326,  492, 1092,  494, 1090,  492, 326,  496, 324,  492, 1092,  492, 326,  492, 326,  492, 1092,  492, 326,  494, 324,  494, 324,  492, 326,  492, 326,  492, 326,  492, 324,  492, 326,  492, 326,  492, 326,  492, 324,  492, 326,  492, 326,  492, 326,  492, 324,  492, 326,  492, 326,  492, 326,  492, 324,  492, 326,  492, 1092,  494, 326,  492, 326,  492, 1092,  492, 1092,  492, 326,  492, 326,  492, 326,  494, 326,  492, 326,  492, 326,  492, 1092,  494, 1092,  492, 326,  492, 1092,  494, 326,  492, 326,  492, 326,  492, 326,  492, 1092,  492, 326,  492, 1092,  492, 326,  492, 326,  492, 326,  492, 326,  492, 326,  492, 326,  492, 326,  492, 326,  492, 326,  492, 326,  492, 326,  494, 324,  494, 324,  492, 326,  492, 326,  492, 326,  492, 326,  492, 326,  492, 326,  492, 326,  492, 324,  494, 324,  492, 326,  492, 326,  492, 328,  492, 326,  492, 326,  492, 326,  492, 326,  492, 326,  492, 326,  492, 326,  492, 324,  492, 326,  492, 326,  492, 326,  492, 1092,  492, 326,  492, 326,  492, 326,  490, 1092,  494, 324,  492, 326,  492, 1092,  492, 1092,  494};  // TCL112AC
uint8_t state[14] = {0x23, 0xCB, 0x26, 0x01, 0x00, 0x20, 0x03, 0x0B, 0x05, 0x00, 0x00, 0x00, 0x80, 0xC8};
Para usar isso, é só enviar esse state[14]. Mas antes nós extraímos mais valore para comparar e enxergar padrões.
state[14] = {0x23, 0xCB, 0x26, 0x01, 0x00, liga/desliga, modo, temperatura, ventilador, 0x00, 0x00, 0x00, oscilar <> e parte decimal do valor de temperautura, checksum}
ligar/desligar: 0x20 para desliagdo e 0x24 para ligado
modo: 0x03 refrigerar
temperatura: 0x0X, onde X = 31 - temperaura
ventilador: 0x05 velocidade máxima do ventilador
oscilar <> e parte decimal:  Primeiro digíto 8 para oscilar desligado e A para ligado. Segundo digíto 0 para inteiro e 8 para terminado em 0,5°C
Checksum: Últimos byte da soma valores anteriores 

Adcionamos um sensor de corrente não invasivo (SCT013-000) ao circuito para monitorar o estado do ar-condicionado. Esse sensor se trata de um transformador de corrente 2000:1. No nosso caso 100A:50mA, para usar ele precisamos converter essa corrente em tensão para o pino A0 e centralizar a senoide em 1,65V (Senoide entre 0V e 3,3V). Assumindo o máximo de corrente em 100Arms temos 50mArms no secundário (70,707mA de pico). Assim, RL = 1,65V/70,707mA = 23,34Ω. 
Usamos um resistor de 23Ω e um divisor de tensão de dois resistores de 10kΩ para centralizar a senoide. Para usar o sensor é necessário usar a biblioteca emonlib, detalhe na inicialização do sensor pede o valor de calibração que é o número de espiras dividido pelo valor do resistor (No nosso caso é 2000/23)
