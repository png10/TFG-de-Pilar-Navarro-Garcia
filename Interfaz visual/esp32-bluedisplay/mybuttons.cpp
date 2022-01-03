/*
 * TFG de Pilar Navarro Garcia
 * 
 * Funciones extras relativas a los botones.
 */

#include <Arduino.h>
#include "BlueDisplay.h"
#include "time.h"


void cambiar_estado_boton(BDButton * aTheTouchedButton __attribute__((unused)), int16_t aValue, color16_t color, char const* caption, bool activar) {
  /*
  * Cambia el estado del boton indicado, indicando el valor, color, leyenda y si se desea activarlo o no.
  */
  aTheTouchedButton->setValue(aValue);
  aTheTouchedButton->setButtonColor(color);
  aTheTouchedButton->setCaption(caption);
  if(activar){
    aTheTouchedButton->activate();
    aTheTouchedButton->drawButton();
  }else aTheTouchedButton->deactivate();
}

void cambiar_estado_boton_emoji_last(BDButton * aTheTouchedButton __attribute__((unused)), int16_t aValue, color16_t color, char const* caption, char emoji){
  /*
  * Igual que cambiar_estado_boton pero con un emoji despues de la leyenda, en la misma linea.
  */
  aTheTouchedButton->setValue(aValue);
  aTheTouchedButton->setButtonColor(color);
  aTheTouchedButton->setCaption(((String)caption + (String)emoji).c_str());
  aTheTouchedButton->activate();
  aTheTouchedButton->drawButton();
}

void cambiar_estado_boton_emoji_first(BDButton * aTheTouchedButton __attribute__((unused)), int16_t aValue, color16_t color, char const* caption, char emoji){
  /*
  * Igual que cambiar_estado_boton pero con un emoji antes de la layenda, separados por un salto de linea.
  */
  aTheTouchedButton->setValue(aValue);
  aTheTouchedButton->setButtonColor(color);
  aTheTouchedButton->setCaption(((String)emoji + "\n" + (String)caption).c_str());
  aTheTouchedButton->activate();
  aTheTouchedButton->drawButton();
}

void cambiar_estado_boton_emoji_first_v2(BDButton * aTheTouchedButton __attribute__((unused)), int16_t aValue, color16_t color, char const* caption, char emoji){
  /*
  * Igual que cambiar_estado_boton pero con un emoji al principio, en la misma linea.
  */
  aTheTouchedButton->setValue(aValue);
  aTheTouchedButton->setButtonColor(color);
  aTheTouchedButton->setCaption(((String)emoji + (String)caption).c_str());
  aTheTouchedButton->activate();
  aTheTouchedButton->drawButton();
}