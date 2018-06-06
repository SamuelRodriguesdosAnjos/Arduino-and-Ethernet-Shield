#include <SPI.h>
#include <SD.h>
//#include <Ethernet.h>
#include <Ethernet_v2.h>

byte mac[] = { YourMACAddress }; 
IPAddress ip( YourIP );
EthernetServer server(80);
File webFile;

#define pinLedG 7
#define pinLedY 6
#define pinLedR 5

#define BUFFER_SIZE 100

String HTTP_req;

void setup() 
{

  Ethernet.begin(mac, ip);
  server.begin();
  Serial.begin(9600);
  
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) 
  {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  if (!SD.exists("index.htm")) 
  {
    Serial.println("ERRO - index.htm nao foi encontrado!");
    return;
  }

  pinMode(pinLedG, OUTPUT);
  pinMode(pinLedY, OUTPUT);
  pinMode(pinLedR, OUTPUT);
}


void loop() 
{  
  EthernetClient client = server.available();
  
  if (client) 
  {
    HTTP_req = "";
    boolean currentLineIsBlank = true;

    while (client.connected()) 
    {      
      if (client.available()) 
      {        
        char c = client.read(); 

        if (HTTP_req.length() < BUFFER_SIZE) 
        {
          HTTP_req += c;
        }
        
        if (c == '\n' && currentLineIsBlank) 
        {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            
            if (HTTP_req.indexOf("LED=") > -1) 
            {
              Serial.println(HTTP_req);
              UpdatePins();
            }
            
            if (HTTP_req.indexOf("ajax_LerPortas") > -1) 
            {
              ReadData(client);
            }
            else{
                            
              webFile = SD.open("index.htm");
              if (webFile) {
                while (webFile.available()) {
                  client.write(webFile.read());
                }
                webFile.close();
              }  
                
            }
             
            break;
        }
        
        if (c == '\n') 
        {
          currentLineIsBlank = true;
        } 
        else if (c != '\r') 
        {
          currentLineIsBlank = false;
        }
        
      }
      
    }
    
    delay(1);    
    // close the connection:
    client.stop();
  }
}

void UpdatePins()
{  
    int valor = 0;
    int pinLed = 0;
    int posicao = -1;

    posicao = HTTP_req.indexOf("=");
    
    if(posicao > -1)
    {
      pinLed = HTTP_req.substring(posicao + 1, posicao + 2).toInt();
      Serial.println("Porta: ");
      Serial.println(pinLed);      
    }

    posicao = HTTP_req.indexOf("_");
    
    if(posicao > -1) 
    {
       valor = HTTP_req.substring(posicao + 1, posicao + 2).toInt();
       Serial.println("Valor: ");
       Serial.println(valor);
    }
    
    digitalWrite(pinLed, valor);
        
}

void ReadData(EthernetClient client) 
{  
  client.print(digitalRead(pinLedG));
  client.print("|");

  client.print(digitalRead(pinLedY));
  client.print("|");

  client.print(digitalRead(pinLedR));
  client.print("|");  
}
