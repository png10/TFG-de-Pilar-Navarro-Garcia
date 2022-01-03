/*
 * TFG de Pilar Navarro Garcia
 * 
 * Variables.
 */

// Variables utilizadas en el menu "Luces"
bool atras = false;
bool luces = false;
bool programa = false;
bool ambiente = false;
bool config = false;
bool luz1 = false;
bool luz2 = false;
bool luz3 = false;
bool allon = false;
bool alloff = false;

// Variables utilizadas en el menu "Programa"
bool activated = false;
int luminosity = 50;
bool prog_luz1 = false;
bool prog_luz2 = false;
bool prog_luz3 = false;
bool prog_activar = false;
bool prog_desactivar = false;
bool prog_mas = false;
bool prog_menos = false;
String prog_luz1_send = "off";
String prog_luz2_send = "off";
String prog_luz3_send = "off";

// Variables utilizadas en el menu "Ambiente"
long ldr = 0;
long temperature = 0;
long humidity = 0;

// Variables utilizadas en el menu "Configuracion"
bool tam_letra = false;
bool iconos = false;
bool sonidos = true;
uint8_t sonidos_idx = 0;
bool modo_daltonico = false;
bool modo_alto_contraste = false;
bool letra_peq = false;
bool letra_med = false;
bool letra_gran = false;
bool letra_muygran = false;
bool aceptar = false;
bool atras_config = false;

// Tamaño de letra
uint16_t tam = 43; // tamaño de letra inicial, mediana
uint16_t tam1 = 40; // pequeña
uint16_t tam2 = 44; // mediana
uint16_t tam3 = 48; // grande
uint16_t tam4 = 52; // muy grande
int tam_selected = 2;

// Pantalla actual
String pantallaActual = "ninguna";

// Iconos
char buffer[10] __attribute__((section(".noinit")));

// Colors
color16_t background_color = standard_background_color;
color16_t on_color = standard_on_color;
color16_t off_color = standard_off_color;
color16_t activar_color = standard_activar_color;
color16_t desactivar_color = standard_desactivar_color;
color16_t letters_color = standard_letters_color;
color16_t letters_assigned_color = standard_letters_assigned_color;
color16_t aceptar_color = standard_aceptar_color;
color16_t gap_color = standard_gap_color;
color16_t rect_color = standard_rect_color;

color16_t luces_color = standard_luces_color;
color16_t programa_color = standard_programa_color;
color16_t ambiente_color = standard_ambiente_color;
color16_t configuracion_color = standard_configuracion_color;

// Canal de transmision ESP32-ESP8266
SoftwareSerial linkSerial(16,17); // RX, TX
StaticJsonDocument<1000> doc;