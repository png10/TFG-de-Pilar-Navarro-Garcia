/*
 * TFG de Pilar Navarro Garcia
 * 
 * Declaracion de las variables de los botones.
 * Cabeceras de las funciones relativas a los botones.
 */

// Botones "Menu principal"
BDButton menu_luces_button, menu_programa_button, menu_ambiente_button, menu_config_button;

// Botones menu "Luces"
BDButton luces_luz1_button, luces_luz2_button, luces_luz3_button, luces_allon_button, luces_alloff_button;

// Botones menu "Programa"
BDButton prog_luz1_button, prog_luz2_button, prog_luz3_button, prog_activar_button, prog_desactivar_button, 
prog_mas_button, prog_menos_button;

// Slider menu "Programa"
BDSlider slider;

// Botones menu "Configuracion"
BDButton tam_letra_button, iconos_button, sonidos_button, modo_daltonico_button, modo_alto_contraste_button,
letra_peq_button, letra_med_button, letra_gran_button, letra_muygran_button, aceptar_config_button, atras_config_button;

// Boton de vuelta atras
BDButton atras_button;

// Funciones "Menu principal"
void menu_luces_button_function(BDButton * aTheTochedButton, int16_t aValue);
void menu_programa_button_function(BDButton * aTheTochedButton, int16_t aValue);
void menu_ambiente_button_function(BDButton * aTheTochedButton, int16_t aValue);
void menu_config_button_function(BDButton * aTheTochedButton, int16_t aValue);

// Funciones menu "Luces"
void luces_luz1_button_function(BDButton * aTheTochedButton, int16_t aValue);
void luces_luz2_button_function(BDButton * aTheTochedButton, int16_t aValue);
void luces_luz3_button_function(BDButton * aTheTochedButton, int16_t aValue);
void luces_allon_button_function(BDButton * aTheTochedButton, int16_t aValue);
void luces_alloff_button_function(BDButton * aTheTochedButton, int16_t aValue);

// Funciones menu "programa"
void prog_luz1_button_function(BDButton * aTheTochedButton, int16_t aValue);
void prog_luz2_button_function(BDButton * aTheTochedButton, int16_t aValue);
void prog_luz3_button_function(BDButton * aTheTochedButton, int16_t aValue);
void prog_activar_button_function(BDButton * aTheTochedButton, int16_t aValue);
void prog_desactivar_button_function(BDButton * aTheTochedButton, int16_t aValue);
void prog_mas_button_function(BDButton * aTheTochedButton, int16_t aValue);
void prog_menos_button_function(BDButton * aTheTochedButton, int16_t aValue);

// Funciones menu "Configuracion"
void tam_letra_button_function(BDButton * aTheTochedButton, int16_t aValue);
void iconos_button_function(BDButton * aTheTochedButton, int16_t aValue);
void sonidos_button_function(BDButton * aTheTochedButton, int16_t aValue);
void modo_daltonico_button_function(BDButton * aTheTochedButton, int16_t aValue);
void modo_alto_contraste_button_function(BDButton * aTheTochedButton, int16_t aValue);
void letra_peq_button_function(BDButton * aTheTochedButton, int16_t aValue);
void letra_med_button_function(BDButton * aTheTochedButton, int16_t aValue);
void letra_gran_button_function(BDButton * aTheTochedButton, int16_t aValue);
void letra_muygran_button_function(BDButton * aTheTochedButton, int16_t aValue);
void aceptar_config_button_function(BDButton * aTheTochedButton, int16_t aValue);
void atras_config_button_function(BDButton * aTheTochedButton, int16_t aValue);

// Funcion de vuelta atras
void atras_button_function(BDButton * aTheTochedButton, int16_t aValue);

void initDisplay(void);
void draw_menu_principal(void);

void draw_menu_luces(void);
void draw_menu_programa(void);
void draw_menu_programa_config_1(void);
void draw_menu_programa_config_2(void);
void draw_menu_ambiente(long, long, long);
void draw_menu_config(void);
void draw_menu_config_tam(void);

void cambiar_estado_boton(BDButton * aTheTouchedButton __attribute__((unused)), int16_t aValue, color16_t color, const char *caption, bool activar);
void cambiar_estado_boton_emoji_last(BDButton * aTheTouchedButton __attribute__((unused)), int16_t aValue, color16_t color, char const* caption, char emoji);
void cambiar_estado_boton_emoji_first(BDButton * aTheTouchedButton __attribute__((unused)), int16_t aValue, color16_t color, char const* caption, char emoji);
void cambiar_estado_boton_emoji_first_v2(BDButton * aTheTouchedButton __attribute__((unused)), int16_t aValue, color16_t color, char const* caption, char emoji);
