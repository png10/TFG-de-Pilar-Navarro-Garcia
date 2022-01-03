/*
 * TFG de Pilar Navarro Garcia
 * 
 * ESP8266 AWS IoT publisher
 * 
 * Suscrito al topic reportedTopic.
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <string.h>
extern "C" {
#include "libb64/cdecode.h"
}
#include "wifi_credentials.h"


WiFiClientSecure wiFiClient;
void callback(char* topic, byte* payload, unsigned int len);
PubSubClient client(awsEndpoint, 8883, callback, wiFiClient);

//#define BUTTON_BUILTIN 0
#define LED_BUILTIN 2

SoftwareSerial linkSerial(13, 15); // RX, TX
StaticJsonDocument<1000> doc;
char buffer[1000];

bool receive = false;

// ********************************************************************************

void setup() {

  linkSerial.begin(4800);
  
  Serial.begin(115200); Serial.println();
  Serial.println("TFG de Pilar Navarro Garcia");
  Serial.println("ESP8266 wifi + serial");
  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();
  Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());

  // get current time, otherwise certificates are flagged as expired
  setCurrentTime();

  uint8_t binaryCert[certificatePemCrt.length() * 3 / 4];
  int len = b64decode(certificatePemCrt, binaryCert);
  wiFiClient.setCertificate(binaryCert, len);
  
  uint8_t binaryPrivate[privatePemKey.length() * 3 / 4];
  len = b64decode(privatePemKey, binaryPrivate);
  wiFiClient.setPrivateKey(binaryPrivate, len);

  uint8_t binaryCA[caPemCrt.length() * 3 / 4];
  len = b64decode(caPemCrt, binaryCA);
  wiFiClient.setCACert(binaryCA, len);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

// ********************************************************************************

void loop() {

  pubSubCheckConnect();

  if(linkSerial.available()){
    
    DeserializationError err = deserializeJson(doc, linkSerial);

    if(err == DeserializationError::Ok){

      client.publish("desiredTopic", doc["request"].as<char*>());
      client.publish("$aws/things/NodeMCU-with-devices/shadow/update", doc["request"].as<char*>());
    }
  }

}

// ********************************************************************************

void callback(char* topic, byte* payload, unsigned int length) {
  /*
  * Cuando recibe un mensaje a traves del topic al que esta suscrito, deserializa
  * el contenido del mensaje para poder enviarlo al ESP32 via comunicacion serie.
  */
  deserializeJson(doc, payload);

  //serializeJsonPretty(doc, buffer); 
  //Serial.println(buffer); // print the msg received to debug

  serializeJson(doc, linkSerial);
}


// ********************************************************************************

void pubSubCheckConnect() {
  /*
  * Comprueba la conexion al cliente.
  */
  if (!client.connected()) {
    Serial.print("client connecting to: "); Serial.print(awsEndpoint);
    while (!client.connected()) {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.print(".");
      client.connect("NodeMCU-serial");
    }
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println(" connected");
    client.subscribe("reportedTopic");
  }
  client.loop();
}

// ********************************************************************************

int b64decode(String b64Text, uint8_t* output) {
  /*
  * Cuenta la longitud de b64Text y lo decodifica en output.
  */
  base64_decodestate s;
  base64_init_decodestate(&s);
  int cnt = base64_decode_block(b64Text.c_str(), b64Text.length(), (char*)output, &s);
  return cnt;
}

// ********************************************************************************

void setCurrentTime() {
  /*
  * Obtiene la hora actual.
  */
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: "); Serial.print(asctime(&timeinfo));
}
