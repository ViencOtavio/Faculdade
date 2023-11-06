/*
 * Pontos de acesso com ESP32
 * Miguel Sena - blog.eletrogate.com
 */
#include <DNSServer.h> //Inclui as bibliotecas
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>   
#include <WiFi.h>
 
AsyncWebServer server(80); //Cria os objetos dos servidores
DNSServer dns;
void setup() {
  AsyncWiFiManager manager(&server, &dns); //Cria o objeto do gerenciador
  Serial.begin(115200); //Inicia o monitor serial
  Serial.println();
  manager.resetSettings(); //Reseta as configurações do gerenciador
  manager.autoConnect("ESP-AP"); //Cria o ponto de acesso 
  Serial.println("Conectado"); //Imprime a mensagem de confirmação
}
 
void loop() {
 // Insira o código do seu projeto aqui
}