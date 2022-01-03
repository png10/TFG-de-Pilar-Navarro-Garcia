 /*
 * TFG de Pilar Navarro Garcia
 * 
 * Version Final BlueDisplay
 */

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include "BlueDisplay.h"
#include "mybuttons.h"
#include "mycolors.h"
#include "myvariables.h"

#ifndef BLUETOOTH_BAUD_RATE
#define BLUETOOTH_BAUD_RATE BAUD_9600
#endif

#define DISPLAY_WIDTH  2027 // para mi POCOPHONE
#define DISPLAY_HEIGHT 1013 // para mi POCOPHONE

char buffer_debug[1000];


// -------------------- Setup --------------------


void setup() {
  Serial.begin(115200);
  linkSerial.begin(4800);

  #if defined(ESP32)
    Serial.begin(115299);
    Serial.println("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_BLUE_DISPLAY);
    initSerial("TFG de Pilar Navarro Garcia");
    Serial.println("Start ESP32 BT-client with name \"ESP-Project-Version-1.0\"");
  #else
      initSerial(BLUETOOTH_BAUD_RATE);
  #endif

  BlueDisplay1.initCommunication(&initDisplay, &draw_menu_principal);

  #if defined(USE_SERIAL1) // defined in BlueSerial.h
  #  if defined(SERIAL_USB)
      delay(4000); // To be able to connect Serial monitor after reset or power up and before first printout
  #  endif
  // Just to know which program is running on my Arduino
      Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_BLUE_DISPLAY));
  #else
      BlueDisplay1.debug("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_BLUE_DISPLAY);
  #endif
}

char LUCES_ICONO = 0xD0;
char PROGRAMA_ICONO = 0xD1;
char AMBIENTE_ICONO = 0xD2;
char CONFIGURACION_ICONO = 0xD3;

char ON_ICONO = 0xD4;
char OFF_ICONO = 0xD5;
char ALL_ON_ICONO = 0xD6;
char ALL_OFF_ICONO = 0xD7;

char MAS10_ICONO = 0xD8;
char MENOS10_ICONO = 0xD9;
char ACTIVAR_ICONO = 0xDA;
char DESACTIVAR_ICONO = 0xDB;

char TEMPERATURA1_ICONO = 0xDC;
char TEMPERATURA2_ICONO = 0xDd;
char HUMEDAD_ICONO = 0xDE;
char LUZ_ICONO = 0xDF;

char ATRAS_ICONO = 0xE0;
char ICONOS_ICONO = 0xE9;
char MODO_ON_ICONO = 0xE2;
char MODO_OFF_ICONO = 0xE3;
char ACEPTAR_ICONO = 0xE4;
char LETRA_ACTUAL_ICONO = 0xE5;
char TAMANYO1_ICONO = 0xE6;
char TAMANYO2_ICONO = 0xE7;
char TAMANYO3_ICONO = 0xE8;

char GRADO_ICONO = 0xBF;

// -------------------- Loop --------------------


void loop() {

  checkAndHandleEvents();
}


// -------------------- initDisplay --------------------


void initDisplay(void) { // Aqui estan todos los botones del display

    BlueDisplay1.setFlagsAndSize(BD_FLAG_FIRST_RESET_ALL | BD_FLAG_USE_MAX_SIZE | BD_FLAG_LONG_TOUCH_ENABLE 
    | BD_FLAG_ONLY_TOUCH_MOVE_DISABLE, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    
    BlueDisplay1.setButtonsGlobalFlags(FLAG_BUTTON_GLOBAL_USE_UP_EVENTS_FOR_BUTTONS);

    BlueDisplay1.setCharacterMapping(0xD0, 0x26A1); // LUCES_ICONO
    BlueDisplay1.setCharacterMapping(0xD1, 0x231B); // PROGRAMA_ICONO
    BlueDisplay1.setCharacterMapping(0xD2, 0x26C5); // AMBIENTE_ICONO
    BlueDisplay1.setCharacterMapping(0xD3, 0x2699); // CONFIGURACION_ICONO 0x26ED 2638

    BlueDisplay1.setCharacterMapping(0xD4, 0x26AA); // ON_ICONO
    BlueDisplay1.setCharacterMapping(0xD5, 0x26AB); // OFF_ICONO
    BlueDisplay1.setCharacterMapping(0xD6, 0x26AA); // ALL_ON_ICONO
    BlueDisplay1.setCharacterMapping(0xD7, 0x26AB); // ALL_OFF_ICONO

    BlueDisplay1.setCharacterMapping(0xD8, 0x2795); // MAS10_ICONO
    BlueDisplay1.setCharacterMapping(0xD9, 0x2796); // MENOS10_ICONO
    BlueDisplay1.setCharacterMapping(0xDA, 0x2714); // ACTIVAR_ICONO
    BlueDisplay1.setCharacterMapping(0xDB, 0x2716); // DESACTIVAR_ICONO

    BlueDisplay1.setCharacterMapping(0xDC, 0x2600); // TEMPERATURA1_ICONO
    BlueDisplay1.setCharacterMapping(0xDD, 0x2744); // TEMPERATURA2_ICONO
    BlueDisplay1.setCharacterMapping(0xDE, 0x2614); // HUMEDAD_ICONO
    BlueDisplay1.setCharacterMapping(0xDF, 0x26A1); // LUZ_ICONO

    BlueDisplay1.setCharacterMapping(0xE0, 0x25C0); // ATRAS_ICONO 
    BlueDisplay1.setCharacterMapping(0xE9, 0x263A); // ICONOS_ON_ICONO
    BlueDisplay1.setCharacterMapping(0xE2, 0x2705); // MODO_ON_ICONO
    BlueDisplay1.setCharacterMapping(0xE3, 0x274C); // MODO_OFF_ICONO
    BlueDisplay1.setCharacterMapping(0xE4, 0x2714); // ACEPTAR_ICONO
    BlueDisplay1.setCharacterMapping(0xE5, 0x2705); // LETRA_ACTUAL_ICONO
    BlueDisplay1.setCharacterMapping(0xE6, 0x25FC); // TAMANYO1_ICONO
    BlueDisplay1.setCharacterMapping(0xE7, 0x25FE); // TAMANYO2_ICONO
    BlueDisplay1.setCharacterMapping(0xE8, 0x25AA); // TAMANYO3_ICONO
    
    BlueDisplay1.setCharacterMapping(0xBF, 0x00B0); // icono grado

    // -------------------- Configuracion de los botones --------------------

    // Menu principal - Luces
    menu_luces_button.init(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*2/20, DISPLAY_WIDTH*7/20, DISPLAY_HEIGHT*7/20, 
    background_color, "Luces", tam, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, luces,
    &menu_luces_button_function);

    // Menu principal - Programa
    menu_programa_button.init(DISPLAY_WIDTH*11/20, DISPLAY_HEIGHT*2/20, DISPLAY_WIDTH*7/20, DISPLAY_HEIGHT*7/20, 
    background_color, "Programar luces", tam, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, programa,
    &menu_programa_button_function);

    // Menu principal - Ambiente
    menu_ambiente_button.init(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*11/20, DISPLAY_WIDTH*7/20, DISPLAY_HEIGHT*7/20, 
    background_color, "Ambiente", tam, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, ambiente,
    &menu_ambiente_button_function);

    // Menu principal - Configuracion
    menu_config_button.init(DISPLAY_WIDTH*11/20, DISPLAY_HEIGHT*11/20, DISPLAY_WIDTH*7/20, DISPLAY_HEIGHT*7/20, 
    background_color, "Configuraci\xf3n", tam, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, config,
    &menu_config_button_function);

    // Menu/Luces
    
    luces_luz1_button.init(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*4/20, DISPLAY_WIDTH*4/20, DISPLAY_HEIGHT*10/20, 
    background_color, "Pasillo\n\nOFF", tam, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, luz1,
    &luces_luz1_button_function);
    luces_luz1_button.setCaptionForValueTrue("Pasillo\n\nON");

    luces_luz2_button.init(DISPLAY_WIDTH*8/20, DISPLAY_HEIGHT*4/20, DISPLAY_WIDTH*4/20, DISPLAY_HEIGHT*10/20, 
    background_color, "Cocina\n\nOFF", tam, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, luz2,
    &luces_luz2_button_function);
    luces_luz2_button.setCaptionForValueTrue("Cocina\n\nON");

    luces_luz3_button.init(DISPLAY_WIDTH*14/20, DISPLAY_HEIGHT*4/20, DISPLAY_WIDTH*4/20, DISPLAY_HEIGHT*10/20, 
    background_color, "Habitaci\xf3n\n\nOFF", tam, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, luz3,
    &luces_luz3_button_function);
    luces_luz3_button.setCaptionForValueTrue("Habitaci\xf3n\n\nON");

    luces_allon_button.init(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*16/20, DISPLAY_WIDTH*6/20, DISPLAY_HEIGHT*3/20, 
    background_color, "Encender todas", tam, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, allon,
    &luces_allon_button_function);

    luces_alloff_button.init(DISPLAY_WIDTH*9/20, DISPLAY_HEIGHT*16/20, DISPLAY_WIDTH*6/20, DISPLAY_HEIGHT*3/20, 
    background_color, "Apagar todas", tam, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, alloff,
    &luces_alloff_button_function);

    // Boton volver atras
    
    atras_button.init(DISPLAY_WIDTH*17/20, DISPLAY_HEIGHT*16/20, DISPLAY_WIDTH*2.8/20, DISPLAY_HEIGHT*3/20, 
    background_color, "Atr\xe1s", tam, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, atras,
    &atras_button_function);

    // Menu/Programa

    prog_luz1_button.init(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*12/20, DISPLAY_WIDTH*4/20, DISPLAY_HEIGHT*3/20, 
    background_color, "Pasillo", tam, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, prog_luz1,
    &prog_luz1_button_function);

    prog_luz2_button.init(DISPLAY_WIDTH*8/20, DISPLAY_HEIGHT*12/20, DISPLAY_WIDTH*4/20, DISPLAY_HEIGHT*3/20, 
    background_color, "Cocina", tam, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, prog_luz2,
    &prog_luz2_button_function);

    prog_luz3_button.init(DISPLAY_WIDTH*14/20, DISPLAY_HEIGHT*12/20, DISPLAY_WIDTH*4/20, DISPLAY_HEIGHT*3/20, 
    background_color, "Habitaci\xf3n", tam, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, prog_luz3,
    &prog_luz3_button_function);

    prog_activar_button.init(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*16/20, DISPLAY_WIDTH*5/20, DISPLAY_HEIGHT*3/20, 
    background_color, "Activar", tam, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, prog_activar,
    &prog_activar_button_function);

    prog_desactivar_button.init(DISPLAY_WIDTH*9/20, DISPLAY_HEIGHT*16/20, DISPLAY_WIDTH*5/20, DISPLAY_HEIGHT*3/20, 
    background_color, "Desactivar", tam, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, prog_desactivar,
    &prog_desactivar_button_function);

    prog_mas_button.init(DISPLAY_WIDTH*11/20, DISPLAY_HEIGHT*7/20, DISPLAY_WIDTH*3/20, DISPLAY_HEIGHT*2/20, 
    background_color, "+1", tam, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, prog_mas,
    &prog_mas_button_function);

    prog_menos_button.init(DISPLAY_WIDTH*5/20, DISPLAY_HEIGHT*7/20, DISPLAY_WIDTH*3/20, DISPLAY_HEIGHT*2/20, 
    background_color, "null", tam, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, prog_menos,
    &prog_menos_button_function);

    
    // Menu/Configuracion
    tam_letra_button.init(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*4/20, DISPLAY_WIDTH*14/20, DISPLAY_HEIGHT*3/20, 
    background_color, "Cambiar tama\xF1o de letra", tam, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, tam_letra,
    &tam_letra_button_function);
    
    iconos_button.init(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*7/20, DISPLAY_WIDTH*14/20, DISPLAY_HEIGHT*3/20, 
    background_color, "Activar/desactivar iconos", tam, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, iconos,
    &iconos_button_function);
    
    sonidos_button.init(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*10/20, DISPLAY_WIDTH*14/20, DISPLAY_HEIGHT*3/20, 
    background_color, "Activar/desactivar sonidos", tam, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, sonidos,
    &sonidos_button_function);
    
    modo_daltonico_button.init(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*13/20, DISPLAY_WIDTH*14/20, DISPLAY_HEIGHT*3/20, 
    background_color, "Activar/desactivar modo dalt\xf3nico", tam, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, modo_daltonico,
    &modo_daltonico_button_function);
    
    modo_alto_contraste_button.init(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*16/20, DISPLAY_WIDTH*14/20, DISPLAY_HEIGHT*3/20, 
    background_color, "Activar/desactivar modo alto contraste", tam, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, modo_alto_contraste,
    &modo_alto_contraste_button_function);

    // Menu/Configuracion/Cambiar_tamaño_letra
    letra_peq_button.init(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*4/20, DISPLAY_WIDTH*14/20, DISPLAY_HEIGHT*3/20, 
    background_color, "Ejemplo de letra pequenya", 42, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, letra_peq,
    &letra_peq_button_function);
    
    letra_med_button.init(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*7/20, DISPLAY_WIDTH*14/20, DISPLAY_HEIGHT*3/20, 
    background_color, "Ejemplo de letra mediana", 44, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, letra_med,
    &letra_med_button_function);
    
    letra_gran_button.init(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*10/20, DISPLAY_WIDTH*14/20, DISPLAY_HEIGHT*3/20, 
    background_color, "Ejemplo de letra grande", 46, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, letra_gran,
    &letra_gran_button_function);
    
    letra_muygran_button.init(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*13/20, DISPLAY_WIDTH*14/20, DISPLAY_HEIGHT*3/20, 
    background_color, "Ejemplo de letra muy grande", 48, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, letra_muygran,
    &letra_muygran_button_function);
    
    aceptar_config_button.init(DISPLAY_WIDTH*6.5/20, DISPLAY_HEIGHT*16.5/20, DISPLAY_WIDTH*7/20, DISPLAY_HEIGHT*3/20, 
    background_color, "Aceptar", tam, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, aceptar,
    &aceptar_config_button_function);
    
    atras_config_button.init(DISPLAY_WIDTH*17/20, DISPLAY_HEIGHT*16/20, DISPLAY_WIDTH*2.8/20, DISPLAY_HEIGHT*3/20, 
    background_color, "Atr\xe1s", tam, FLAG_BUTTON_DO_BEEP_ON_TOUCH | FLAG_BUTTON_TYPE_TOGGLE_RED_GREEN, atras_config,
    &atras_config_button_function);
}


// -------------------- Boton para volver al menu principal --------------------

void atras_button_function(BDButton * aTheTouchedButton __attribute__((unused)), int16_t aValue) {
  /*
  * Funcion del boton de volver atras al menu principal.
  */
  atras = aValue;
  if(atras){
    send_json_menu("principal");
    draw_menu_principal();
  }
}


// -------------------- Menu principal --------------------


void draw_menu_principal(void) {
  /*
  * Dibuja el menu principal.
  */
  clearDisplayAndDisableButtonsAndSliders(background_color);

  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*2/20, DISPLAY_WIDTH*7/20, DISPLAY_HEIGHT*7/20, rect_color, DISPLAY_WIDTH/400);
  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*11/20, DISPLAY_HEIGHT*2/20, DISPLAY_WIDTH*7/20, DISPLAY_HEIGHT*7/20, rect_color, DISPLAY_WIDTH/400);
  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*11/20, DISPLAY_WIDTH*7/20, DISPLAY_HEIGHT*7/20, rect_color, DISPLAY_WIDTH/400);
  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*11/20, DISPLAY_HEIGHT*11/20, DISPLAY_WIDTH*7/20, DISPLAY_HEIGHT*7/20, rect_color, DISPLAY_WIDTH/400);

  if(iconos){
    cambiar_estado_boton_emoji_first(&menu_luces_button, 0, luces_color, "\nLuces", LUCES_ICONO);
    if(activated){
      cambiar_estado_boton_emoji_first(&menu_programa_button, 0, programa_color, "\nProgramar luces\n\n(activado)", PROGRAMA_ICONO);
    }else cambiar_estado_boton_emoji_first(&menu_programa_button, 0, programa_color, "\nProgramar luces\n\n(desactivado)", PROGRAMA_ICONO);    
    cambiar_estado_boton_emoji_first(&menu_ambiente_button, 0, ambiente_color, "\nAmbiente", AMBIENTE_ICONO);
    cambiar_estado_boton_emoji_first(&menu_config_button, 0, configuracion_color, "\nConfiguraci\xf3n", CONFIGURACION_ICONO);
  }else{
    cambiar_estado_boton(&menu_luces_button, 0, luces_color, "Luces", true);
    if(activated){
      cambiar_estado_boton(&menu_programa_button, 0, programa_color, "Programar luces\n\n(activado)", true);
    }else cambiar_estado_boton(&menu_programa_button, 0, programa_color, "Programar luces\n\n(desactivado)", true);
    cambiar_estado_boton(&menu_ambiente_button, 0, ambiente_color, "Ambiente", true);
    cambiar_estado_boton(&menu_config_button, 0, configuracion_color, "Configuraci\xf3n", true);
  }
}

void menu_luces_button_function(BDButton * aTheTouchedButton __attribute__((unused)), int16_t aValue) {
  /*
  * Funcion del boton del menu "luces".
  */
  if(aValue){
    pantallaActual = "luces";
    
    if(iconos){
      draw_loading("LUCES ", LUCES_ICONO);
    }else draw_loading("LUCES", '0');
    
    send_json(linkSerial, "requested", "requested", "requested", "null", "null", "null", "null", "null", "null", "null", "null", pantallaActual);
    while(!linkSerial.available()){
      checkAndHandleEvents();
    }

    if(linkSerial.available()){
      DeserializationError err = deserializeJson(doc, linkSerial);
      serializeJsonPretty(doc, buffer_debug); 
      Serial.println(buffer_debug); // print the msg received to debug
      Serial.println(err.c_str());
      if(err == DeserializationError::Ok){

        Serial.println(doc["state"]["reported"]["luz_pasillo"].as<String>());        
        Serial.println(doc["state"]["reported"]["luz_cocina"].as<String>());
        Serial.println(doc["state"]["reported"]["luz_habitacion"].as<String>());
        Serial.println("-------------------");

        if(doc["state"]["reported"]["luz_pasillo"].as<String>() == "on"){
          luz1 = true;
        }else luz1 = false;
        if(doc["state"]["reported"]["luz_cocina"].as<String>() == "on"){
          luz2 = true;
        }else luz2 = false;
        if(doc["state"]["reported"]["luz_habitacion"].as<String>() == "on"){
          luz3 = true;
        }else luz3 = false;
      }
    }

    draw_menu_luces();
  }
}

void menu_programa_button_function(BDButton * aTheTouchedButton __attribute__((unused)), int16_t aValue) {
  /*
  * Funcion del boton del menu "programa".
  */
  if(aValue){
    pantallaActual = "programa";
    
    if(iconos){
      draw_loading("PROGRAMAR LUCES ", PROGRAMA_ICONO);
    }else draw_loading("PROGRAMAR LUCES", '0');

    send_json(linkSerial, "null", "null", "null", "null", "null", "null", "requested", "null", "null", "null", "null", pantallaActual);
    while(!linkSerial.available()){
      checkAndHandleEvents();
    }

    if(linkSerial.available()){
      DeserializationError err = deserializeJson(doc, linkSerial);
      if(err == DeserializationError::Ok){

        if(doc["state"]["reported"]["p_activado"].as<String>() == "si"){
          activated = true;
          luminosity = doc["state"]["reported"]["p_luminosidad"].as<int>();
          if(doc["state"]["reported"]["p_luz_pasillo"].as<String>() == "si"){
            prog_luz1 = true;
          }else prog_luz1 = false;
          if(doc["state"]["reported"]["p_luz_cocina"].as<String>() == "si"){
            prog_luz2 = true;
          }else prog_luz2 = false;
          if(doc["state"]["reported"]["p_luz_habitacion"].as<String>() == "si"){
            prog_luz3 = true;
          }else prog_luz3 = false;
        }else{
          activated = false;
          prog_luz1 = 0;
          prog_luz2 = 0;
          prog_luz3 = 0;
          luminosity = 50;
        }
      }
    }

    draw_menu_programa();
  }
}

void menu_ambiente_button_function(BDButton * aTheTouchedButton __attribute__((unused)), int16_t aValue) {
  /*
  * Funcion del boton del menu "ambiente".
  */
  if(aValue){
    pantallaActual = "ambiente";

    if(iconos){
      draw_loading("AMBIENTE ", AMBIENTE_ICONO);
    }else draw_loading("AMBIENTE", '0');
    
    send_json(linkSerial, "null", "null", "null", "requested", "requested", "requested", "null", "null", "null", "null", "null", pantallaActual);
    ldr, temperature, humidity = 0;
    while(!linkSerial.available()){
      checkAndHandleEvents();
    }
    
    if(linkSerial.available()){
      DeserializationError err = deserializeJson(doc, linkSerial);
      if(err == DeserializationError::Ok){
        if(doc["state"]["reported"]["temperatura"].as<String>() != "-1.00") temperature = doc["state"]["reported"]["temperatura"].as<long>();
        if(doc["state"]["reported"]["humedad"].as<String>() != "-1.00") humidity = doc["state"]["reported"]["humedad"].as<long>();
        ldr = doc["state"]["reported"]["luminosidad"].as<long>();
        //serializeJsonPretty(doc, buffer_debug); 
        //Serial.println(buffer_debug); // print the msg received to debug
        draw_menu_ambiente(temperature, humidity, ldr);
      }
    }
  }
    
}

void menu_config_button_function(BDButton * aTheTouchedButton __attribute__((unused)), int16_t aValue) {
  /*
  * Funcion del boton del menu "configuracion".
  */
  if(aValue){
    pantallaActual = "configuracion";
    send_json_menu(pantallaActual);
    draw_menu_config();
  }
}


// -------------------- Menu/Luces --------------------


void draw_menu_luces(void) {
  /*
  * Dibuja el menu luces.
  */
  if(iconos){
    clear_and_draw_title("Luces ", LUCES_ICONO);
  }else clear_and_draw_title("Luces", '0');

  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*4/20, DISPLAY_WIDTH*4/20, DISPLAY_HEIGHT*10/20, rect_color, DISPLAY_WIDTH/400);
  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*8/20, DISPLAY_HEIGHT*4/20, DISPLAY_WIDTH*4/20, DISPLAY_HEIGHT*10/20, rect_color, DISPLAY_WIDTH/400);
  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*14/20, DISPLAY_HEIGHT*4/20, DISPLAY_WIDTH*4/20, DISPLAY_HEIGHT*10/20, rect_color, DISPLAY_WIDTH/400);


  if(luz1){
    if(iconos){
      cambiar_estado_boton_emoji_first(&luces_luz1_button, 0, on_color, "\nPasillo\n\nON", ON_ICONO);
    }else cambiar_estado_boton(&luces_luz1_button, 0, on_color, "\nPasillo\n\nON", true);
  }else{
    if(iconos){
      cambiar_estado_boton_emoji_first(&luces_luz1_button, 0, off_color, "\nPasillo\n\nOFF", OFF_ICONO);
    }else cambiar_estado_boton(&luces_luz1_button, 0, off_color, "\nPasillo\n\nOFF", true);
  }

  if(luz2){
    if(iconos){
      cambiar_estado_boton_emoji_first(&luces_luz2_button, 0, on_color, "\nCocina\n\nON", ON_ICONO);
    }else cambiar_estado_boton(&luces_luz2_button, 0, on_color, "\nCocina\n\nON", true);
  }else{
    if(iconos){
      cambiar_estado_boton_emoji_first(&luces_luz2_button, 0, off_color, "\nCocina\n\nOFF", OFF_ICONO);
    }else cambiar_estado_boton(&luces_luz2_button, 0, off_color, "\nCocina\n\nOFF", true);
  }

  if(luz3){
    if(iconos){
      cambiar_estado_boton_emoji_first(&luces_luz3_button, 0, on_color, "\nHabitaci\xf3n\n\nON", ON_ICONO);
    }else cambiar_estado_boton(&luces_luz3_button, 0, on_color, "\nHabitaci\xf3n\n\nON", true);
  }else{
    if(iconos){
      cambiar_estado_boton_emoji_first(&luces_luz3_button, 0, off_color, "\nHabitaci\xf3n\n\nOFF", OFF_ICONO);
    }else cambiar_estado_boton(&luces_luz3_button, 0, off_color, "\nHabitaci\xf3n\n\nOFF", true);
  }

  if(!luz1 || !luz2 || !luz3){
    BlueDisplay1.drawRectRel(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*16/20, DISPLAY_WIDTH*6/20, DISPLAY_HEIGHT*3/20, rect_color, DISPLAY_WIDTH/400);
    if(iconos){
      cambiar_estado_boton_emoji_last(&luces_allon_button, 0, activar_color, "Encender todas ", ALL_ON_ICONO);
    }else cambiar_estado_boton(&luces_allon_button, 0, activar_color, "Encender todas ", true);
  }else cambiar_estado_boton(&luces_allon_button, 0, background_color, "", false);
  if(luz1 || luz2 || luz3){
    BlueDisplay1.drawRectRel(DISPLAY_WIDTH*9/20, DISPLAY_HEIGHT*16/20, DISPLAY_WIDTH*6/20, DISPLAY_HEIGHT*3/20, rect_color, DISPLAY_WIDTH/400);
    if(iconos){
      cambiar_estado_boton_emoji_last(&luces_alloff_button, 0, desactivar_color, "Apagar todas ", ALL_OFF_ICONO);
    }else cambiar_estado_boton(&luces_alloff_button, 0, desactivar_color, "Apagar todas ", true);
  }else cambiar_estado_boton(&luces_alloff_button, 0, background_color, "", false);

  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*17/20, DISPLAY_HEIGHT*16/20, DISPLAY_WIDTH*2.8/20, DISPLAY_HEIGHT*3/20, rect_color, DISPLAY_WIDTH/400);
  if(iconos){
    cambiar_estado_boton_emoji_last(&atras_button, 0, gap_color, "Atr\xe1s ", ATRAS_ICONO);
  }else cambiar_estado_boton(&atras_button, 0, gap_color, "Atr\xe1s", true);
}

void luces_luz1_button_function(BDButton * aTheTouchedButton __attribute__((unused)), int16_t aValue) {
  /*
  * Funcion del boton de la luz1, dentro del menu "luces".
  */
  if(aValue){
    if(luz1){
      luz1 = 0;
      send_json(linkSerial, "off", "null", "null", "null", "null", "null", "null", "null", "null", "null", "null", pantallaActual);
    }else{
      luz1 = 1;
      send_json(linkSerial, "on", "null", "null", "null", "null", "null", "null", "null", "null", "null", "null", pantallaActual);
    }
  draw_menu_luces();
  }
}

void luces_luz2_button_function(BDButton * aTheTouchedButton __attribute__((unused)), int16_t aValue) {
  /*
  * Funcion del boton de la luz2, dentro del menu "luces".
  */
  if(aValue){
    if(luz2){
      luz2 = 0;
      send_json(linkSerial, "null", "off", "null", "null", "null", "null", "null", "null", "null", "null", "null", pantallaActual);
    }else{
      luz2 = 1;
      send_json(linkSerial, "null", "on", "null", "null", "null", "null", "null", "null", "null", "null", "null", pantallaActual);
    }
  draw_menu_luces();
  }
}

void luces_luz3_button_function(BDButton * aTheTouchedButton __attribute__((unused)), int16_t aValue) {
  /*
  * Funcion del boton de la luz3, dentro del menu "luces".
  */
  if(aValue){
    if(luz3){
      luz3 = 0;
      send_json(linkSerial, "null", "null", "off", "null", "null", "null", "null", "null", "null", "null", "null", pantallaActual);
    }else{
      luz3 = 1;
      send_json(linkSerial, "null", "null", "on", "null", "null", "null", "null", "null", "null", "null", "null", pantallaActual);
    }
  }
  draw_menu_luces();
}

void luces_allon_button_function(BDButton * aTheTouchedButton __attribute__((unused)), int16_t aValue) {
  /*
  * Funcion del boton de encender todas las luces, dentro del menu "luces".
  */
  if(aValue){
    luz1 = luz2 = luz3 = 1;
    send_json(linkSerial, "on", "on", "on", "null", "null", "null", "null", "null", "null", "null", "null", pantallaActual);
  }
  draw_menu_luces();
}

void luces_alloff_button_function(BDButton * aTheTouchedButton __attribute__((unused)), int16_t aValue) {
  /*
  * Funcion del boton de apagar todas las luces, dentro del menu "luces".
  */
  if(aValue){
    luz1 = luz2 = luz3 = 0;
    send_json(linkSerial, "off", "off", "off", "null", "null", "null", "null", "null", "null", "null", "null", pantallaActual);
  }
  draw_menu_luces();
}


// -------------------- Menu/Programa --------------------


void draw_menu_programa(void) {
  /*
  * Dibuja el menu "programa".
  */
  if(iconos){
    clear_and_draw_title("Programar luces ", PROGRAMA_ICONO);
  }else clear_and_draw_title("Programar luces", '0');

  BlueDisplay1.drawText(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*5/20, "Cuando la LUMINOSIDAD sea menor de:", tam, letters_color, background_color);
  BlueDisplay1.drawText(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*11/20, "Encender las luces:", tam, letters_color, background_color);
  BlueDisplay1.drawText(DISPLAY_WIDTH*9/20, DISPLAY_HEIGHT*8.5/20, ((String)luminosity+(String)("%")).c_str(), tam+5, letters_color, background_color);

  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*11/20, DISPLAY_HEIGHT*7/20, DISPLAY_WIDTH*3/20, DISPLAY_HEIGHT*2/20, rect_color, DISPLAY_WIDTH/400);
  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*5/20, DISPLAY_HEIGHT*7/20, DISPLAY_WIDTH*3/20, DISPLAY_HEIGHT*2/20, rect_color, DISPLAY_WIDTH/400);
  if(iconos){
    cambiar_estado_boton_emoji_first_v2(&prog_mas_button, 0, gap_color, " 10", MAS10_ICONO);
    cambiar_estado_boton_emoji_first_v2(&prog_menos_button, 0, gap_color, " 10", MENOS10_ICONO);
  }else{
    cambiar_estado_boton(&prog_mas_button, 0, gap_color, "+10", true);
    cambiar_estado_boton(&prog_menos_button, 0, gap_color, "-10", true);
  }

  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*12/20, DISPLAY_WIDTH*4/20, DISPLAY_HEIGHT*3/20, rect_color, DISPLAY_WIDTH/400);
  if(prog_luz1){
    if(iconos){
      cambiar_estado_boton_emoji_first(&prog_luz1_button, 0, on_color, "Pasillo", ON_ICONO);
    }else cambiar_estado_boton(&prog_luz1_button, 0, on_color, "Pasillo", true);
  }else if(iconos){
    cambiar_estado_boton_emoji_first(&prog_luz1_button, 0, off_color, "Pasillo", OFF_ICONO);
  }else cambiar_estado_boton(&prog_luz1_button, 0, off_color, "Pasillo", true);

  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*8/20, DISPLAY_HEIGHT*12/20, DISPLAY_WIDTH*4/20, DISPLAY_HEIGHT*3/20, rect_color, DISPLAY_WIDTH/400);
  if(prog_luz2){
    if(iconos){
      cambiar_estado_boton_emoji_first(&prog_luz2_button, 0, on_color, "Cocina", ON_ICONO);
    }else cambiar_estado_boton(&prog_luz2_button, 0, on_color, "Cocina", true);
  }else if(iconos){
    cambiar_estado_boton_emoji_first(&prog_luz2_button, 0, off_color, "Cocina", OFF_ICONO);
  }else cambiar_estado_boton(&prog_luz2_button, 0, off_color, "Cocina", true);

  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*14/20, DISPLAY_HEIGHT*12/20, DISPLAY_WIDTH*4/20, DISPLAY_HEIGHT*3/20, rect_color, DISPLAY_WIDTH/400);
  if(prog_luz3){
    if(iconos){
      cambiar_estado_boton_emoji_first(&prog_luz3_button, 0, on_color, "Habitaci\xf3n", ON_ICONO);
    }else cambiar_estado_boton(&prog_luz3_button, 0, on_color, "Habitaci\xf3n", true);
  }else if(iconos){
    cambiar_estado_boton_emoji_first(&prog_luz3_button, 0, off_color, "Habitaci\xf3n", OFF_ICONO);
  }else cambiar_estado_boton(&prog_luz3_button, 0, off_color, "Habitaci\xf3n", true);

  if(prog_luz1 || prog_luz2 || prog_luz3){
    BlueDisplay1.drawRectRel(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*16/20, DISPLAY_WIDTH*5/20, DISPLAY_HEIGHT*3/20, rect_color, DISPLAY_WIDTH/400);
    if(iconos){
      cambiar_estado_boton_emoji_last(&prog_activar_button, 0, activar_color, "Activar ", ACTIVAR_ICONO);
    }else cambiar_estado_boton(&prog_activar_button, 0, activar_color, "Activar", true);
  }else cambiar_estado_boton(&prog_activar_button, 0, background_color, "", false);
  
  if(activated){
    BlueDisplay1.drawRectRel(DISPLAY_WIDTH*9/20, DISPLAY_HEIGHT*16/20, DISPLAY_WIDTH*5/20, DISPLAY_HEIGHT*3/20, rect_color, DISPLAY_WIDTH/400);
    if(iconos){
      cambiar_estado_boton_emoji_last(&prog_desactivar_button, 0, programa_color, "Desactivar ", DESACTIVAR_ICONO);
    }else cambiar_estado_boton(&prog_desactivar_button, 0, programa_color, "Desactivar", true);
  }
  
  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*17/20, DISPLAY_HEIGHT*16/20, DISPLAY_WIDTH*2.8/20, DISPLAY_HEIGHT*3/20, rect_color, DISPLAY_WIDTH/400);
  if(iconos){
    cambiar_estado_boton_emoji_last(&atras_button, 0, gap_color, "Atr\xe1s ", ATRAS_ICONO);
  }else cambiar_estado_boton(&atras_button, 0, gap_color, "Atr\xe1s", true);
}

void prog_luz1_button_function(BDButton * aTheTochedButton, int16_t aValue){
  /*
  * Funcion del boton de la luz1, dentro del menu "programa".
  */
  if(aValue){
    if(prog_luz1){
      prog_luz1 = 0;
    }else prog_luz1 = 1;
  }
  draw_menu_programa();
}

void prog_luz2_button_function(BDButton * aTheTochedButton, int16_t aValue){
  /*
  * Funcion del boton de la luz2, dentro del menu "programa".
  */
  if(aValue){
    if(prog_luz2){
      prog_luz2 = 0;
    }else prog_luz2 = 1;
  }
  draw_menu_programa();
}

void prog_luz3_button_function(BDButton * aTheTochedButton, int16_t aValue){
  /*
  * Funcion del boton de la luz3, dentro del menu "programa".
  */
  if(aValue){
    if(prog_luz3){
      prog_luz3 = 0;
    }else prog_luz3 = 1;
  }
  draw_menu_programa();
}

void prog_activar_button_function(BDButton * aTheTochedButton, int16_t aValue){
  /*
  * Funcion del boton de activacion del programa, dentro del menu "programa".
  */
  if(aValue){
    activated = 1;
    prog_activar = 1;
    if(prog_luz1){
      prog_luz1_send = "on";
    }else prog_luz1_send = "off";
    if(prog_luz2){
      prog_luz2_send = "on";
    }else prog_luz2_send = "off";
    if(prog_luz3){
      prog_luz3_send = "on";
    }else prog_luz3_send = "off";
    send_json(linkSerial, "null", "null", "null", "null", "null", "null", "si", String(luminosity), prog_luz1_send, prog_luz2_send, prog_luz3_send, "principal");
  }
  draw_menu_principal();
}

void prog_desactivar_button_function(BDButton * aTheTochedButton, int16_t aValue){
  /*
  * Funcion del boton de desactivacion del programa, dentro del menu "programa".
  */
  if(aValue){
    activated = 0;
    prog_desactivar = 1;
    prog_luz1 = 0;
    prog_luz2 = 0;
    prog_luz3 = 0;
    luminosity = 50;
    send_json(linkSerial, "null", "null", "null", "null", "null", "null", "no", "null", "null", "null", "null", "principal");
  }
  draw_menu_principal();
}

void prog_mas_button_function(BDButton * aTheTochedButton, int16_t aValue){
  /*
  * Funcion del boton de aumento de la luminosidad requerida en el programa, dentro del menu "programa".
  */
  if(aValue){
    if(luminosity < 100) luminosity += 10;
  }
  draw_menu_programa();
}

void prog_menos_button_function(BDButton * aTheTochedButton, int16_t aValue){
  /*
  * Funcion del boton de decremento de la luminosidad requerida en el programa, dentro del menu "programa".
  */
  if(aValue){
    if(luminosity > 0)  luminosity -= 10;
  }
  draw_menu_programa();
}


// -------------------- Menu/Ambiente --------------------


void draw_menu_ambiente(long temperature, long humidity, long ldr) {
  /*
  * Dibuja el menu "ambiente".
  */
  if(iconos){
    clear_and_draw_title("Ambiente ", AMBIENTE_ICONO);
  }else clear_and_draw_title("Ambiente", '0');

  if(iconos){
    BlueDisplay1.drawText(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*8/20, ((String)TEMPERATURA1_ICONO+(String)TEMPERATURA2_ICONO+(String)(" Temperatura: ")+(String)temperature+(String)GRADO_ICONO+(String)("C")).c_str(), tam, letters_color, background_color);
    BlueDisplay1.drawText(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*12/20, ((String)HUMEDAD_ICONO+(String)(" Humedad: ")+(String)humidity+(String)("%")).c_str(), tam, letters_color, background_color);
    BlueDisplay1.drawText(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*16/20, ((String)LUZ_ICONO+(String)(" Intensidad de luz: ")+(String)ldr+(String)("%")).c_str(), tam, letters_color, background_color);
  }else{
    BlueDisplay1.drawText(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*8/20, ((String)("Temperatura: ")+(String)temperature+(String)GRADO_ICONO+(String)("C")).c_str(), tam, letters_color, background_color);
    BlueDisplay1.drawText(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*12/20, ((String)("Humedad: ")+(String)humidity+(String)("%")).c_str(), tam, letters_color, background_color);
    BlueDisplay1.drawText(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*16/20, ((String)("Intensidad de luz: ")+(String)ldr+(String)("%")).c_str(), tam, letters_color, background_color);
  }

  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*17/20, DISPLAY_HEIGHT*16/20, DISPLAY_WIDTH*2.8/20, DISPLAY_HEIGHT*3/20, rect_color, DISPLAY_WIDTH/400);
  if(iconos){
    cambiar_estado_boton_emoji_last(&atras_button, 0, gap_color, "Atr\xe1s ", ATRAS_ICONO);
  }else cambiar_estado_boton(&atras_button, 0, gap_color, "Atr\xe1s", true);
}


// -------------------- Menu/Configuración --------------------


void draw_menu_config(void) {
  /*
  * Dibuja el menu "configuracion".
  */
  if(iconos){
    clear_and_draw_title("Configuraci\xf3n ", CONFIGURACION_ICONO);
  }else clear_and_draw_title("Configuraci\xf3n", '0');

  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*4/20, DISPLAY_WIDTH*14/20, DISPLAY_HEIGHT*3/20, rect_color, DISPLAY_WIDTH/400);
  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*7/20, DISPLAY_WIDTH*14/20, DISPLAY_HEIGHT*3/20, rect_color, DISPLAY_WIDTH/400);
  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*10/20, DISPLAY_WIDTH*14/20, DISPLAY_HEIGHT*3/20, rect_color, DISPLAY_WIDTH/400);
  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*13/20, DISPLAY_WIDTH*14/20, DISPLAY_HEIGHT*3/20, rect_color, DISPLAY_WIDTH/400);
  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*16/20, DISPLAY_WIDTH*14/20, DISPLAY_HEIGHT*3/20, rect_color, DISPLAY_WIDTH/400);
  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*17/20, DISPLAY_HEIGHT*16/20, DISPLAY_WIDTH*2.8/20, DISPLAY_HEIGHT*3/20, rect_color, DISPLAY_WIDTH/400);

  if(iconos){
    cambiar_estado_boton(&tam_letra_button, 0, gap_color, ((String)("Cambiar tama\xF1o de letra ")+(String)TAMANYO2_ICONO+(String)TAMANYO3_ICONO+(String)TAMANYO1_ICONO).c_str(), true);
    cambiar_estado_boton_emoji_last(&iconos_button, 0, gap_color, "Desactivar iconos ", ICONOS_ICONO);
    if(sonidos){
      cambiar_estado_boton_emoji_last(&sonidos_button, 0, gap_color, "Desactivar sonidos ", MODO_ON_ICONO);
    }else cambiar_estado_boton_emoji_last(&sonidos_button, 0, gap_color, "Activar sonidos ", MODO_OFF_ICONO);
    if(modo_daltonico){
      cambiar_estado_boton_emoji_last(&modo_daltonico_button, 0, gap_color, "Desactivar modo dalt\xf3nico ", MODO_ON_ICONO);
    }else{
      cambiar_estado_boton_emoji_last(&modo_daltonico_button, 0, gap_color, "Activar modo dalt\xf3nico ", MODO_OFF_ICONO);
    }
    if(modo_alto_contraste){
      cambiar_estado_boton_emoji_last(&modo_alto_contraste_button, 0, gap_color, "Desactivar modo alto contraste ", MODO_ON_ICONO);
    }else cambiar_estado_boton_emoji_last(&modo_alto_contraste_button, 0, gap_color, "Activar modo alto contraste ", MODO_OFF_ICONO);
    cambiar_estado_boton_emoji_last(&atras_button, 0, gap_color, "Atr\xe1s ", ATRAS_ICONO);

  }else{
    cambiar_estado_boton(&tam_letra_button, 0, gap_color, "Cambiar tama\xF1o de letra", true);
    cambiar_estado_boton_emoji_last(&iconos_button, 0, gap_color, "Activar iconos ", ICONOS_ICONO);
    if(sonidos){
      cambiar_estado_boton(&sonidos_button, 0, gap_color, "Desactivar sonidos", true);
    }else cambiar_estado_boton(&sonidos_button, 0, gap_color, "Activar sonidos", true);
    if(modo_daltonico){
      cambiar_estado_boton(&modo_daltonico_button, 0, gap_color, "Desactivar modo dalt\xf3nico", true);
    }else{
      cambiar_estado_boton(&modo_daltonico_button, 0, gap_color, "Activar modo dalt\xf3nico", true);
    }
    if(modo_alto_contraste){
      cambiar_estado_boton(&modo_alto_contraste_button, 0, gap_color, "Desactivar modo alto contraste", true);
    }else cambiar_estado_boton(&modo_alto_contraste_button, 0, gap_color, "Activar modo alto contraste", true);
    cambiar_estado_boton(&atras_button, 0, gap_color, "Atr\xe1s", true);
  }


}

void draw_menu_config_tam(void) {
  /*
  * Dibuja el menu "cambiar tamaño de letra", dentro del menu "configuracion".
  */

  if(iconos){
    clearDisplayAndDisableButtonsAndSliders(background_color);
    BlueDisplay1.drawText(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*2/20, ((String)("Configuraci\xf3n ") + (String)CONFIGURACION_ICONO + (String)(" Cambiar tama\xF1o de letra")).c_str(), tam, letters_color, background_color);
    BlueDisplay1.drawLine(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*3/20, DISPLAY_WIDTH*15/20, DISPLAY_HEIGHT*3/20, letters_color);
  }else clear_and_draw_title("Configuraci\xf3n: Cambiar tama\xF1o de letra", '0');

  char caption[25];
  strcpy(caption, "Ejemplo de letra peque");
  strcat(caption, "\xF1");
  strcat(caption, "a");

  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*4/20, DISPLAY_WIDTH*14/20, DISPLAY_HEIGHT*3/20, rect_color, DISPLAY_WIDTH/400);
  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*7/20, DISPLAY_WIDTH*14/20, DISPLAY_HEIGHT*3/20, rect_color, DISPLAY_WIDTH/400);
  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*10/20, DISPLAY_WIDTH*14/20, DISPLAY_HEIGHT*3/20, rect_color, DISPLAY_WIDTH/400);
  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*13/20, DISPLAY_WIDTH*14/20, DISPLAY_HEIGHT*3/20, rect_color, DISPLAY_WIDTH/400);

  switch(tam_selected){
    case 1: // Pequeña
      cambiar_estado_boton(&letra_peq_button, 0, letters_assigned_color, (const char*)caption, true);
      cambiar_estado_boton(&letra_med_button, 0, gap_color, "Ejemplo de letra mediana", true);
      cambiar_estado_boton(&letra_gran_button, 0, gap_color, "Ejemplo de letra grande", true);
      cambiar_estado_boton(&letra_muygran_button, 0, gap_color, "Ejemplo de letra muy grande", true);
    break;
    case 2: // Mediana
      cambiar_estado_boton(&letra_peq_button, 0, gap_color, (const char*)caption, true);
      cambiar_estado_boton(&letra_med_button, 0, letters_assigned_color, "Ejemplo de letra mediana", true);
      cambiar_estado_boton(&letra_gran_button, 0, gap_color, "Ejemplo de letra grande", true);
      cambiar_estado_boton(&letra_muygran_button, 0, gap_color, "Ejemplo de letra muy grande", true);
    break;
    case 3: // Grande
      cambiar_estado_boton(&letra_peq_button, 0, gap_color, (const char*)caption, true);
      cambiar_estado_boton(&letra_med_button, 0, gap_color, "Ejemplo de letra mediana", true);
      cambiar_estado_boton(&letra_gran_button, 0, letters_assigned_color, "Ejemplo de letra grande", true);
      cambiar_estado_boton(&letra_muygran_button, 0, gap_color, "Ejemplo de letra muy grande", true);
    break;
    case 4: // Muy grande
      cambiar_estado_boton(&letra_peq_button, 0, gap_color, (const char*)caption, true);
      cambiar_estado_boton(&letra_med_button, 0, gap_color, "Ejemplo de letra mediana", true);
      cambiar_estado_boton(&letra_gran_button, 0, gap_color, "Ejemplo de letra grande", true);
      cambiar_estado_boton(&letra_muygran_button, 0, letters_assigned_color, "Ejemplo de letra muy grande", true);
    break;
  }

  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*6.5/20, DISPLAY_HEIGHT*16.5/20, DISPLAY_WIDTH*7/20, DISPLAY_HEIGHT*3/20, rect_color, DISPLAY_WIDTH/400);
  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*17/20, DISPLAY_HEIGHT*16/20, DISPLAY_WIDTH*2.8/20, DISPLAY_HEIGHT*3/20, rect_color, DISPLAY_WIDTH/400);
  if(iconos){
    cambiar_estado_boton_emoji_last(&aceptar_config_button, 0, aceptar_color, "Aceptar ", ACEPTAR_ICONO);
    cambiar_estado_boton_emoji_last(&atras_button, 0, gap_color, "Atr\xe1s ", ATRAS_ICONO);
  }else{
    cambiar_estado_boton(&aceptar_config_button, 0, aceptar_color, "Aceptar", true);
    cambiar_estado_boton(&atras_config_button, 0, gap_color, "Atr\xe1s", true);
  }
}

void tam_letra_button_function(BDButton * aTheTochedButton, int16_t aValue){
  /*
  * Funcion del boton de configurar el tamaño de letra, dentro del menu "configuracion".
  */
  if(aValue){
    draw_menu_config_tam();
  }
}

void iconos_button_function(BDButton * aTheTochedButton, int16_t aValue){
  /*
  * Funcion del boton de activar/desactivar iconos, dentro del menu "configuracion".
  */
  if(aValue){
    if(iconos){
      iconos = false;
    }else{
      iconos = true;
    }
  }
  draw_menu_config();
}

void sonidos_button_function(BDButton * aTheTochedButton, int16_t aValue){
  /*
  * Funcion del boton de activar/desactivar sonidos, dentro del menu "configuracion".
  */
  if(aValue)
    if(sonidos){ // Desctivar sonidos
      sonidos = false;
      BlueDisplay1.setButtonsTouchTone(FEEDBACK_TONE_NO_TONE, 0);
    }else{ // Activar sonidos
      sonidos = true;
      BlueDisplay1.setButtonsTouchTone(TONE_DEFAULT, 700);
    }
  draw_menu_config();
}

void modo_daltonico_button_function(BDButton * aTheTochedButton, int16_t aValue){
  /*
  * Funcion del boton de activar/desactivar el modo daltonico, dentro del menu "configuracion".
  */
  if(aValue)
    if(modo_daltonico){ // Desctivar modo daltonico
      modo_daltonico = 0;
      background_color = standard_background_color;
      on_color = standard_on_color;
      off_color = standard_off_color;
      activar_color = standard_activar_color;
      desactivar_color = standard_desactivar_color;
      letters_color = standard_letters_color;
      letters_assigned_color = standard_letters_assigned_color;
      aceptar_color = standard_aceptar_color;
      gap_color = standard_gap_color;
      rect_color = standard_rect_color;
      luces_color = standard_luces_color;
      programa_color = standard_programa_color;
      ambiente_color = standard_ambiente_color;
      configuracion_color = standard_configuracion_color;
    }else{ // Activar modo daltonico
      modo_daltonico = 1;
      modo_alto_contraste = 0;
      background_color = daltonic_background_color;
      on_color = daltonic_on_color;
      off_color = daltonic_off_color;
      activar_color = daltonic_activar_color;
      desactivar_color = daltonic_desactivar_color;
      letters_color = daltonic_letters_color;
      letters_assigned_color = daltonic_letters_assigned_color;
      aceptar_color = daltonic_aceptar_color;
      gap_color = daltonic_gap_color;
      rect_color = daltonic_rect_color;
      luces_color = daltonic_luces_color;
      programa_color = daltonic_programa_color;
      ambiente_color = daltonic_ambiente_color;
      configuracion_color = daltonic_configuracion_color;
    }
    draw_menu_config();
}

void modo_alto_contraste_button_function(BDButton * aTheTochedButton, int16_t aValue){
  /*
  * Funcion del boton de activar/desactivar el modo alto contraste, dentro del menu "configuracion".
  */
  if(aValue)
    if(modo_alto_contraste){ // Desctivar modo alto contraste
      modo_alto_contraste = 0;
      background_color = standard_background_color;
      on_color = standard_on_color;
      off_color = standard_off_color;
      activar_color = standard_activar_color;
      desactivar_color = standard_desactivar_color;
      letters_color = standard_letters_color;
      letters_assigned_color = standard_letters_assigned_color;
      aceptar_color = standard_aceptar_color;
      gap_color = standard_gap_color;
      rect_color = standard_rect_color;
      luces_color = standard_luces_color;
      programa_color = standard_programa_color;
      ambiente_color = standard_ambiente_color;
      configuracion_color = standard_configuracion_color;
    }else{ // Activar modo alto contraste
      modo_alto_contraste = 1;
      modo_daltonico = 0;
      background_color = contrast_background_color;
      on_color = contrast_on_color;
      off_color = contrast_off_color;
      activar_color = contrast_activar_color;
      desactivar_color = contrast_desactivar_color;
      letters_color = contrast_letters_color;
      letters_assigned_color = contrast_letters_assigned_color;
      aceptar_color = contrast_aceptar_color;
      gap_color = contrast_gap_color;
      rect_color = contrast_rect_color;
      luces_color = contrast_luces_color;
      programa_color = contrast_programa_color;
      ambiente_color = contrast_ambiente_color;
      configuracion_color = contrast_configuracion_color;
    }
    draw_menu_config();
}

void letra_peq_button_function(BDButton * aTheTochedButton, int16_t aValue){
  /*
  * Funcion del boton de letra pequeña, dentro del menu "configuracion > cambiar tamaño de letra".
  */
  if(aValue){
    tam_selected = 1;
  }
  draw_menu_config_tam();
}

void letra_med_button_function(BDButton * aTheTochedButton, int16_t aValue){
  /*
  * Funcion del boton de letra mediana, dentro del menu "configuracion > cambiar tamaño de letra".
  */
  if(aValue){
    tam_selected = 2;
  }
  draw_menu_config_tam();
}

void letra_gran_button_function(BDButton * aTheTochedButton, int16_t aValue){
  /*
  * Funcion del boton de letra grande, dentro del menu "configuracion > cambiar tamaño de letra".
  */
  if(aValue){
    tam_selected = 3;
  }
  draw_menu_config_tam();
}

void letra_muygran_button_function(BDButton * aTheTochedButton, int16_t aValue){
  /*
  * Funcion del boton de letra muy grande, dentro del menu "configuracion > cambiar tamaño de letra".
  */
  if(aValue){
    tam_selected = 4;
  }
  draw_menu_config_tam();
}

void aceptar_config_button_function(BDButton * aTheTochedButton, int16_t aValue){
  /*
  * Funcion del boton para aceptar la configuracion del tamaño de letra, dentro del menu "configuracion > cambiar tamaño de letra".
  */
  if(aValue){
    if(tam_selected == 1){
      tam = tam1;
    }else if(tam_selected == 2){
      tam = tam2;
    }else if(tam_selected == 3){
      tam = tam3;
    }else if(tam_selected == 4){
      tam = tam4;
    }
    initDisplay();
    draw_menu_config();
  }
}

void atras_config_button_function(BDButton * aTheTochedButton, int16_t aValue){
  /*
  * Funcion del boton de volver atras al menu "configuracion", dentro del menu "configuracion > cambiar tamaño de letra".
  */
  if(aValue){
    if(tam == tam1){
      tam_selected = 1;
    }else if(tam == tam2){
      tam_selected = 2;
    }else if(tam == tam3){
      tam_selected = 3;
    }else if(tam == tam4){
      tam_selected = 4;
    }
    draw_menu_config();
  }
}

// ***********************************************************************************

void send_json(SoftwareSerial &linkSerial, String var1, String var2, String var3, String var4, 
String var5, String var6, String var7, String var8, String var9, String var10, String var11, String var12){
  /*
  * Envia al NodeMCU por serie el mensaje construido.
  * Manda solamente el estado de las variables recibidas en la llamada que no sean null.
  */
  String request = "{\"state\": {\"desired\": {";
  if(var1 != "null"){
    request += "\"luz_pasillo\": \"";
    request += var1;
    request += "\", ";
  }
  if(var2 != "null"){
    request += "\"luz_cocina\": \"";
    request += var2;
    request += "\", ";
  }
  if(var3 != "null"){
    request += "\"luz_habitacion\": \"";
    request += var3;
    request += "\", ";
  }
  if(var4 != "null"){
    request += "\"temperatura\": \"";
    request += var4;
    request += "\", \"humedad\": \"";
    request += var5;
    request += "\", \"luminosidad\": \"";
    request += var6;
    request += "\", ";
  }
  if(var7 != "null"){
    request += "\"p_activado\": \"";
    request += var7;
    request += "\", ";
  }
  if(var7 == "si"){
    request += "\"p_luminosidad\": \"";
    request += var8;
    request += "\", \"p_luz_pasillo\": \"";
    request += var9;
    request += "\", \"p_luz_cocina\": \"";
    request += var10;
    request += "\", \"p_luz_habitacion\": \"";
    request += var11;
    request += "\", ";
  }
  request += "\"pantallaActual\": \"";
  request += var12;
  request += "\" }}}";

  doc["request"] = request;

  while(!linkSerial.isListening()){
    checkAndHandleEvents();
  }
  if(linkSerial.isListening()) serializeJson(doc, linkSerial);
}

void send_json_menu(String pantallaActual){
  /*
  * Envia al NodeMCU por serie el mensaje construido.
  * Manda solamente el estado de la pantalla actual.
  */
  String request = "{\"state\": {\"desired\": { \"pantallaActual\": \"";
  request += pantallaActual;
  request += "\" }, \"reported\": { \"pantallaActual\": \"";
  request += pantallaActual;
  request += "\" }}}";

  doc["request"] = request;
  serializeJson(doc, linkSerial);
}

// ***********************************************************************************

void clear_and_draw_title(char* title, char emoji){
  /*
  * Borra la pantalla actual, desactiva todos los botones y dibuja el titulo especificado con el emoji indicado (si no es 0).
  */
  clearDisplayAndDisableButtonsAndSliders(background_color);
  if(emoji != '0'){
    BlueDisplay1.drawText(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*2/20, ((String)title + (String)emoji).c_str(), tam, letters_color, background_color);
  }else BlueDisplay1.drawText(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*2/20, title, tam, letters_color, background_color);
  
  BlueDisplay1.drawLine(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*3/20, DISPLAY_WIDTH*15/20, DISPLAY_HEIGHT*3/20, letters_color);
}

// ***********************************************************************************

void draw_loading(char* loading_menu, char emoji){
  /*
  * Dibuja la pantalla de carga indicada con el emoji indicado (si no es 0).
  */
  clearDisplayAndDisableButtonsAndSliders(background_color);
  BlueDisplay1.drawRectRel(DISPLAY_WIDTH*17/20, DISPLAY_HEIGHT*16/20, DISPLAY_WIDTH*2.8/20, DISPLAY_HEIGHT*3/20, rect_color, DISPLAY_WIDTH/400);

  if(emoji != '0'){
    BlueDisplay1.drawText(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*10/20, ((String)("Cargando men\xfa ")+(String)loading_menu+(String)emoji).c_str(), tam+5, letters_color, background_color);
    cambiar_estado_boton(&atras_button, 0, gap_color, "Atr\xe1s", true);
  }else{
    char displayed[30];
    strcpy(displayed, "Cargando men\xfa ");
    strcat(displayed, loading_menu);
    BlueDisplay1.drawText(DISPLAY_WIDTH*2/20, DISPLAY_HEIGHT*10/20, displayed, tam+5, letters_color, background_color);
    cambiar_estado_boton_emoji_last(&atras_button, 0, gap_color, "Atr\xe1s ", ATRAS_ICONO);
  }
  
}
