# Ethernet Shield

Usando Arduino com Ethernet Shield 5100/5500 V2

# Para usar o shield de ethernet com o arduino, vamos precisar de algumas bibliotecas: 

Ethernet.h para o modelo 5100 V1
ou
Ethernet_v2.h para o modelo 5500 V2

SPI.h - Necessária como dependencia das bibliotecas acima

SD.h - Para a comunicação com o sd card do shield ethernet para a leitura do arquivo HTML

# No arquivo .ino temos algumas configurações necessárias a serem feitas:

YourMACAddress - É o endereço mac registrado ao seu shield ethernet

YourIP - É o IP que será gerado na sua rede
