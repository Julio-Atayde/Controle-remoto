Projeto desenvolvido no CEFET-RJ/Nova Iguaçu
Clonagem de controle remoto
O projeto consiste em clonar um controle remoto de ar-condicionado usando ESP para conectar o sistema com a internet.
Para começar a clonagem é necessário usar um receptor IR para capturar o que é enviado em cada comando. Uma vez que sabemos o que enviar, usamos um LED transmissor IR til32 para enviar os sinais até o dispositivo a ser controlado. É preciso da biblioteca IRremote (ou suas variações) e conhecer em qual protocolo esse sinal IR é enviado.

