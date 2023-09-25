/*********
  Wagner Becker
*********/

#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include "DHT.h"
#include <Adafruit_Sensor.h>
#include <OneWire.h>
#include <DallasTemperature.h>


// Replace the next variables with your SSID/Password combination
const char* ssid = "TPLINK_1";
const char* password = "12031965";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "7179422ab76b4c6d8ecaa4a5dcfc530b.s2.eu.hivemq.cloud";
const char* mqtt_username = "Embarcado"; // replace with your Credential
const char* mqtt_password = "Embarcado2023";
const int mqtt_port = 8883;

WiFiClientSecure espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;


static const char *root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";


#define DHTPIN 13 
#define DHTTYPE DHT22 
#define CONNECTION_TIMEOUT 10
#define NTP_SERVER     "pool.ntp.org"
#define UTC_OFFSET     -10800
#define UTC_OFFSET_DST 0

DHT dht(DHTPIN, DHTTYPE);
const int oneWireBus = 14;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
String macAddress;
bool isConnected = false;
unsigned long lastMacPublishTime = 0;
const unsigned long macPublishInterval = 5000;


void setup() {
  Serial.begin(115200);
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  
  setup_wifi();
  while (!Serial) delay(1);
  espClient.setCACert(root_ca);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  dht.begin();
  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);
  macAddress = WiFi.macAddress();
}

void setup_wifi() {
  WiFi.mode(WIFI_STA); //Optional
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");
    int timeout_counter = 0;

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(200);
        timeout_counter++;
        if(timeout_counter >= CONNECTION_TIMEOUT*5){
        ESP.restart();
        }
    }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  char connectTopic[50];
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  snprintf(connectTopic, sizeof(connectTopic), "%s/CONFIG/Connected", macAddress.c_str());
  if (String(topic) == connectTopic) {
    Serial.print("Changing output to ");
    if(messageTemp == "yes"){
      Serial.println("Connected to API");
      isConnected = true;
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  char connectTopic[50];
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = macAddress;   // Create a random client ID
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");

      snprintf(connectTopic, sizeof(connectTopic), "%s/CONFIG/Connected", macAddress.c_str());
      client.subscribe(connectTopic);
     

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");   // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

bool publishMacAddress() {
  // Envie o endereço MAC para o tópico CONFIG/Connect
  return client.publish("CONFIG/Connect", macAddress.c_str());
}

void publishLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Connection Error");
    return;
  }

  char dataHora[20];
  snprintf(dataHora, sizeof(dataHora), "%02d/%02d/%04d %02d:%02d:%02d", 
    timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900, 
    timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

  char timeTopic[50];
  snprintf(timeTopic, sizeof(timeTopic), "%s/dataHora", macAddress.c_str());
  client.publish(timeTopic, dataHora);

  Serial.printf("%d/%d/%04d %02d:%02d:%02d\n", 
    timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900, 
    timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
}

void publishSensorData(const char* sensorName, float value) {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Connection Error");
    return;
  }

  char dataHora[20];
  snprintf(dataHora, sizeof(dataHora), "%02d/%02d/%04d %02d:%02d:%02d", 
    timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900, 
    timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

  char topic[50];
  snprintf(topic, sizeof(topic), "esp32/%s/%s", macAddress.c_str(), sensorName);
  
  char data[27];
  snprintf(data, sizeof(data), "%s/%.2f", dataHora, value);
  
  client.publish(topic, data);
  
  Serial.print(topic);
  Serial.print("/");
  Serial.println(data);
}

void publishDHT22() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (!isnan(humidity) && !isnan(temperature)) {
    publishSensorData("AirHumidity", humidity);
    publishSensorData("RmTemperature", temperature);
  } else {
    Serial.println("Failed to read DHT22 data");
  }
}

void publishBS18B20() {
  sensors.requestTemperatures();
  float soilTemperature = sensors.getTempCByIndex(0);
  publishSensorData("SilTemperature", soilTemperature);
}


void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Enviar o endereço MAC apenas se a ESP32 estiver conectada
  unsigned long currentMillis = millis();
  if (!isConnected && client.connected() && currentMillis - lastMacPublishTime >= macPublishInterval) {
    lastMacPublishTime = currentMillis;  // Atualize o tempo da última publicação
    publishMacAddress();
    Serial.println("MAC Address published successfully");
  }
  if (isConnected) {
    // Executar rotinas enquanto estiver conectado
    struct tm timecalc;
    if (!getLocalTime(&timecalc)) {
      Serial.print("Not Connected to NTP");
      return;
    }
    int min = timecalc.tm_min;
    static bool executedThisMinute = false;

    if (min % 2 == 0 && !executedThisMinute) {
      executedThisMinute = true;

      publishLocalTime();
      publishDHT22();
      publishBS18B20();
      
    } else if  (min % 2 != 0) {
      executedThisMinute = false;  // Redefina a variável para permitir a execução no próximo minuto par
    }

    // Verifique se a conexão MQTT foi perdida ou o Wi-Fi desconectado
    if (!client.connected() || WiFi.status() != WL_CONNECTED) {
      isConnected = false; // Defina o estado de conexão como false
      Serial.println("Conexão MQTT ou Wi-Fi perdida. Reconectando...");
    }
  }
}
