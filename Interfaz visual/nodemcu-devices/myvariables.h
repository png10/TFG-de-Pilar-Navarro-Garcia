/*
 * TFG de Pilar Navarro Garcia
 * 
 * Variables del NodeMCU-with-devices.
 */

// Variables utilizadas en el menu "Luces"
String luz_pasillo_v = "off";
String luz_cocina_v = "off";
String luz_habitacion_v = "off";

// Variables utilizadas en el menu "Programa"
bool luz_pasillo_prev = false;
bool luz_cocina_prev = false;
bool luz_habitacion_prev = false;

bool p_activado = false;
String p_luz_pasillo = "null";
String p_luz_cocina = "null";
String p_luz_habitacion = "null";
int p_luminosidad = 50;

// Variables utilizadas en el menu "Ambiente"
const int dht_pin = 5; // D1
float temperatura_v = 0;
float humedad_v = 0;
int luminosidad_v = 0;

// Pantalla actual
String pantallaActual = "-1";
