/*
 * TFG de Pilar Navarro Garcia
 * 
 * ESP8266 AWS IoT - DEVICES
 *
 * Suscrito al topic desiredTopic.
 */
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <string.h>
#include <Ticker.h>
extern "C" {
#include "libb64/cdecode.h"
}
#include "wifi_credentials.h"
#include "myvariables.h"
#include "DHT.h"


WiFiClientSecure wiFiClient;
void callback(char* topic, byte* payload, unsigned int len);
PubSubClient client(awsEndpoint, 8883, callback, wiFiClient);
void program_interruption();

#define LED_BUILTIN 2

// Luces
#define luz_pasillo D5
#define luz_cocina D6
#define luz_habitacion D7
// Ambiente
#define LDR A0
#define DHTTYPE DHT11

DHT dht(dht_pin, DHTTYPE);
Ticker timer;

StaticJsonDocument<500> doc;
char buffer[1000];

bool fromAlexa = false;


// ********************************************************************************

void setup() {
  
  Serial.begin(115200); Serial.println();
  Serial.println("NodeMCU with devices, connected to AWS IoT");

  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();
  Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());

  // get current time, otherwise certificates are flagged as expired
  setCurrentTime();

  uint8_t binaryCert[certificatePemCrt.length()*3/4];
  int len = b64decode(certificatePemCrt, binaryCert);
  wiFiClient.setCertificate(binaryCert, len);
  
  uint8_t binaryPrivate[privatePemKey.length()*3/4];
  len = b64decode(privatePemKey, binaryPrivate);
  wiFiClient.setPrivateKey(binaryPrivate, len);

  uint8_t binaryCA[caPemCrt.length()*3/4];
  len = b64decode(caPemCrt, binaryCA);
  wiFiClient.setCACert(binaryCA, len);

  pinMode(LDR, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(luz_pasillo, OUTPUT);
  digitalWrite(luz_pasillo, LOW);
  pinMode(luz_cocina, OUTPUT);
  digitalWrite(luz_cocina, LOW);
  pinMode(luz_habitacion, OUTPUT);
  digitalWrite(luz_habitacion, LOW);

  dht.begin();
}

// ********************************************************************************

void loop() {

  pubSubCheckConnect();
}

// ********************************************************************************

void callback(char* topic, byte* payload, unsigned int length) {
  /*
  * Cuando recibe un mensaje a traves del topic al que esta suscrito, deserializa
  * el contenido del mensaje para poder trabajar con el.
  */
  deserializeJson(doc, payload);
  serializeJsonPretty(doc, buffer); 
  Serial.println(buffer); // print the msg received to debug

  if(strcmp(topic, "alexaTopic") == 0){
    fromAlexa = true;
  }else fromAlexa = false;

  if((doc["state"]["desired"]["luz_cocina"].as<String>() != "null" or doc["state"]["desired"]["luz_pasillo"].as<String>() != "null" 
  or doc["state"]["desired"]["luz_habitacion"].as<String>() != "null") and doc["state"]["desired"]["luz_pasillo"].as<String>() != "requested"){ // MENU LUCES (cambiar estado)

    // Luz cocina
    if(doc["state"]["desired"]["luz_cocina"].as<String>() == "on"){
      digitalWrite(luz_cocina, HIGH);
      luz_cocina_v = "on";
      luz_cocina_prev = true;
    }else if(doc["state"]["desired"]["luz_cocina"].as<String>() == "off"){
      digitalWrite(luz_cocina, LOW);
      luz_cocina_v = "off";
      luz_cocina_prev = false;
    }

    // Luz pasillo
    if(doc["state"]["desired"]["luz_pasillo"].as<String>() == "on"){
      digitalWrite(luz_pasillo, HIGH);
      luz_pasillo_v = "on";
      luz_pasillo_prev = true;
    }else if(doc["state"]["desired"]["luz_pasillo"].as<String>() == "off"){
      digitalWrite(luz_pasillo, LOW);
      luz_pasillo_v = "off";
      luz_pasillo_prev = false;
    }

    // Luz habitacion
    if(doc["state"]["desired"]["luz_habitacion"].as<String>() == "on"){
      digitalWrite(luz_habitacion, HIGH);
      luz_habitacion_v = "on";
      luz_habitacion_prev = true;
    }else if(doc["state"]["desired"]["luz_habitacion"].as<String>() == "off"){
      digitalWrite(luz_habitacion, LOW);
      luz_habitacion_v = "off";
      luz_habitacion_prev = false;
    }
    publish_json_luces(luz_pasillo_v, luz_cocina_v, luz_habitacion_v, doc["state"]["desired"]["pantallaActual"].as<String>());

  }else if(doc["state"]["desired"]["luz_pasillo"].as<String>() == "requested"){ // MENU LUCES (requested)
    publish_json_luces(luz_pasillo_v, luz_cocina_v, luz_habitacion_v, doc["state"]["desired"]["pantallaActual"].as<String>());

  }else if(doc["state"]["desired"]["p_activado"].as<String>() != "null" and doc["state"]["desired"]["p_activado"].as<String>() != "requested"){  // MENU PROGRAMA (cambiar estado)

    if(doc["state"]["desired"]["p_activado"].as<String>() == "si" and (doc["state"]["desired"]["p_luz_pasillo"].as<String>() == "on" or 
    doc["state"]["desired"]["p_luz_cocina"].as<String>() == "on" or doc["state"]["desired"]["p_luz_habitacion"].as<String>() == "on") and
    (doc["state"]["desired"]["p_luminosidad"].as<int>() >= 0) and (doc["state"]["desired"]["p_luminosidad"].as<int>() <= 100)){ // activar
      p_activado = true;
      timer.attach(5, program_interruption); // problema con esta linea... (antes)
      
      p_luminosidad = doc["state"]["desired"]["p_luminosidad"].as<int>();

      if(doc["state"]["desired"]["p_luz_pasillo"].as<String>() == "on"){
        p_luz_pasillo = "on";
      }else p_luz_pasillo = "off";

      if(doc["state"]["desired"]["p_luz_cocina"].as<String>() == "on"){
        p_luz_cocina = "on";
      }else p_luz_cocina = "off";

      if(doc["state"]["desired"]["p_luz_habitacion"].as<String>() == "on"){
        p_luz_habitacion = "on";
      }else p_luz_habitacion = "off";

      publish_json_programa("si", (String)p_luminosidad, p_luz_pasillo, p_luz_cocina, p_luz_habitacion, doc["state"]["desired"]["pantallaActual"].as<String>(), true);

    }else if(doc["state"]["desired"]["p_activado"].as<String>() == "no"){ // desactivar
      p_activado = false;
      deprogram();
      publish_json_programa("no", "null", "null", "null", "null", doc["state"]["desired"]["pantallaActual"].as<String>(), true);
    }

  }else if(doc["state"]["desired"]["p_activado"].as<String>() == "requested"){ // MENU PROGRAMA (requested)
  
    if(p_activado){
      publish_json_programa("si", (String)p_luminosidad, p_luz_pasillo, p_luz_cocina, p_luz_habitacion, doc["state"]["desired"]["pantallaActual"].as<String>(), false);
    }else{
      publish_json_programa("no", "null", "null", "null", "null", doc["state"]["desired"]["pantallaActual"].as<String>(), false);
    }
    
  }else if(doc["state"]["desired"]["temperatura"].as<String>() == "requested"){ // MENU AMBIENTE (requested)
    
    temperatura_v = dht.readTemperature();
    delay(5);
    humedad_v = dht.readHumidity();
    if(isnan(temperatura_v) || isnan(humedad_v)){
      temperatura_v = -1;
      humedad_v = -1;
    }
    luminosidad_v = get_luminosity();
    
    delay(50); // debug
    publish_json_ambiente((String)temperatura_v, (String)humedad_v, (String)luminosidad_v, doc["state"]["desired"]["pantallaActual"].as<String>());
  }
}


// ********************************************************************************

void publish_json_luces(String pasillo, String cocina, String habitacion, String pantalla){
  /*
  * Publica el estado actual de las luces.
  * Lo publica en el topic requestedTopic (para que lo lea el NodeMCU) y en el topic que actualiza
  * la sombra del thing asociado.
  */
  String request = "{\"state\": {\"reported\": {\"luz_pasillo\": \"";
  request += pasillo;
  request += "\", \"luz_cocina\": \"";
  request += cocina;
  request += "\", \"luz_habitacion\": \"";
  request += habitacion;
  request += "\", \"pantallaActual\": \"";
  request += pantalla;
  request += "\" }}}";

  doc["request"] = request;
  if(!fromAlexa) client.publish("reportedTopic", doc["request"].as<char*>());
  client.publish("$aws/things/NodeMCU-with-devices/shadow/update", doc["request"].as<char*>());
}


// ********************************************************************************

void publish_json_programa(String activado, String luminosidad, String pasillo, String cocina, String habitacion, String pantalla, bool publish){
  /*
  * Publica el estado actual de la programacion de las luces.
  * Lo publica en el topic requestedTopic (para que lo lea el NodeMCU) y en el topic que actualiza
  * la sombra del thing asociado.
  */
  String request = "{\"state\": {\"reported\": {\"p_activado\": \"";
  request += activado;
  if(activado == "si"){
    request += "\", \"p_luminosidad\": \"";
    request += luminosidad;
    request += "\", \"p_luz_pasillo\": \"";
    request += pasillo;
    request += "\", \"p_luz_cocina\": \"";
    request += cocina;
    request += "\", \"p_luz_habitacion\": \"";
    request += habitacion;
    request += "\", \"pantallaActual\": \"";
    request += pantalla;
  }
  request += "\" }}}";

  doc["request"] = request;
  if(!fromAlexa) client.publish("reportedTopic", doc["request"].as<char*>());
  if(publish) client.publish("$aws/things/NodeMCU-with-devices/shadow/update", doc["request"].as<char*>());
}


// ********************************************************************************

void publish_json_ambiente(String temperatura, String humedad, String luminosidad, String pantalla){
  /*
  * Publica la lectura actual de los sensores de temperatura, humedad y luminosidad.
  * Lo publica en el topic requestedTopic (para que lo lea el NodeMCU) y en el topic que actualiza
  * la sombra del thing asociado.
  */
  String request = "{\"state\": {\"reported\": {\"temperatura\": \"";
  request += temperatura;
  request += "\", \"humedad\": \"";
  request += humedad;
  request += "\", \"luminosidad\": \"";
  request += luminosidad;
  request += "\", \"pantallaActual\": \"";
  request += pantalla;
  request += "\" }}}";

  doc["request"] = request;
  if(!fromAlexa) client.publish("reportedTopic", doc["request"].as<char*>());
  client.publish("$aws/things/NodeMCU-with-devices/shadow/update", doc["request"].as<char*>());
}


// ********************************************************************************

void program_interruption(void){
  /*
  * Activa el programa de las luces, activando una interrupcion para consultar
  * el nivel de luminosidad cada 5 segundos.
  */
  if(get_luminosity() < p_luminosidad){

    if(p_luz_pasillo == "on"){
      digitalWrite(luz_pasillo, HIGH);
      luz_pasillo_v = "on";
    }

    if(p_luz_cocina == "on"){
      digitalWrite(luz_cocina, HIGH);
      luz_cocina_v = "on";
    }

    if(p_luz_habitacion == "on"){
      digitalWrite(luz_habitacion, HIGH);
      luz_habitacion_v = "on";
    }

  }else{

    if(!luz_pasillo_prev){
      digitalWrite(luz_pasillo, LOW);
      luz_pasillo_v = "off";
    }
    if(!luz_cocina_prev){
      digitalWrite(luz_cocina, LOW);
      luz_cocina_v = "off";
    }
    if(!luz_habitacion_prev){
      digitalWrite(luz_habitacion, LOW);
      luz_habitacion_v = "off";
    }
  }
  timer.attach(5, program_interruption); // probar a quitar
}


void deprogram(void){
  /*
  * Desactiva el programa de las luces, desactivando la interrupcion.
  */
  timer.detach();

  if(!luz_pasillo_prev){
    digitalWrite(luz_pasillo, LOW);
    luz_pasillo_v = "off";
  }
  if(!luz_cocina_prev){
    digitalWrite(luz_cocina, LOW);
    luz_cocina_v = "off";
  }
  if(!luz_habitacion_prev){
    digitalWrite(luz_habitacion, LOW);
    luz_habitacion_v = "off";
  }
}


// ********************************************************************************

int get_luminosity(void){
  /*
  * Obtiene el nivel de luminosidad a traves del sensor LDR.
  */
  int v = analogRead(LDR);
  luminosidad_v = (v-10)*100/(300-10);
  if(luminosidad_v > 100){
    luminosidad_v = 100;
  }else if(luminosidad_v < 0){
    luminosidad_v = 0;
  }
  return luminosidad_v;
}

// ********************************************************************************

void pubSubCheckConnect() {
  /*
  * Comprueba la conexion al cliente.
  */
  if ( ! client.connected()) {
    Serial.print("client connecting to: "); Serial.print(awsEndpoint);
    while ( ! client.connected()) {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.print(".");
      client.connect("NodeMCU-devices");
    }
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println(" connected");
    client.subscribe("desiredTopic");
    client.subscribe("alexaTopic");
  }
  digitalWrite(LED_BUILTIN, LOW);
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
