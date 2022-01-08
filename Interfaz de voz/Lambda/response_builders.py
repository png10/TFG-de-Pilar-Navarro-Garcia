def build_speechlet_response(title, output, reprompt, should_end_session):
    return {
        'outputSpeech': {
            'type': 'PlainText',
            'text': output
        },
        'card': {
            'type': 'Simple',
            'title': "SessionSpeechlet - " + title,
            ' content': "SessionSpeechlet - " + output
        },
        'reprompt': {
            'outputSpeech': {
                'type': 'PlainText',
                'text': reprompt
            }
        },
        'should_end_session': should_end_session
    }
    
    
def build_response(attributes, speechlet_response):
    return {
        'version': '1.0',
        'sessionAttributes': attributes,
        'response': speechlet_response
    }
    

# --------------- Funciones que construyen la respuesta de Alexa ---------------
    
def get_welcome_response():
    attributes = {}
    card = "Bienvenido"
    output = "Hola, te doy la bienvenida a mi skill. Si necesitas algo, dímelo."
    reprompt = "No sé si me has oído, pero estoy aquí para lo que necesites."
    return build_response(attributes, build_speechlet_response(card, output, reprompt, False))


def get_goodbye_response():
    attributes = {}
    card = "Hasta pronto"
    output = "Espero haberte sido de ayuda."
    return build_response(attributes, build_speechlet_response(card, output, None, True))
    

def error_response():
    attributes = {}
    card = "Error"
    output = "Lo siento, no te he entendido, inténtalo de nuevo."
    return build_response(attributes, build_speechlet_response(card, output, None, True))
    

def error_lights_response():
    attributes = {}
    card = "Error"
    output = "Lo siento, la luz indicada no es válida."
    return build_response(attributes, build_speechlet_response(card, output, None, True))


def error_sensors_response():
    attributes = {}
    card = "Error"
    output = "Los datos obtenidos por los sensores no son válidos."
    return build_response(attributes, build_speechlet_response(card, output, None, True))
    
    
def get_screen_response(pantalla, is_current_screen):
    attributes = {}
    card = pantalla
    output = ""
    
    if(is_current_screen):
        output = "El menú actual es " + pantalla + ". En esta pantalla "
    else: output = "En el menú " + pantalla + " "
    
    if pantalla == "principal":
        output += "puedes acceder a las cuatro opciones disponibles: luces, programa, ambiente o configuración. Si quieres saber qué puedes hacer en uno de ellos puedes decir: "
        output += "qué puedo hacer en el menú luces."
        """output += "puedes acceder a las cuatro opciones disponibles: luces, para encender o apagar las luces de tu casa; "
        output += "programa, para  programar qué luces de tu casa quieres que se enciendan cuando el nivel de luminosidad sea menor de un porcentaje elegido por ti; "
        output += "ambiente, para ver la temperatura, humedad y luminosidad de tu casa; "
        output += "o configuración, para configurar varios parámetros de la aplicación como el tamaño de letra, activar o desactivar iconos y sonidos y poner la aplicación en modo daltónico o de alto contraste.";"""
    elif pantalla == "luces":
        output += "puedes encender o apagar las luces de tu casa."
    elif pantalla == "programa":
        output += "puedes programar qué luces de tu casa quieres que se enciendan cuando el nivel de luminosidad sea menor de un porcentaje elegido por ti."
    elif pantalla == "ambiente":
        output += "puedes ver la temperatura, humedad y luminosidad de tu casa."
    elif (pantalla == "configuracion" or pantalla == "configuración"):
        output += "puedes configurar varios parámetros de la aplicación como el tamaño de letra, activar o desactivar iconos y sonidos y poner la aplicación en modo daltónico o de alto contraste."
        
    #print(output)
    return build_response(attributes, build_speechlet_response(card, output, None, False))
    
    
def get_lights_response(cocina, pasillo, habitacion):
    attributes = {}
    card = "Luces"
    
    if(cocina == "on"):
        state_cocina = "encendida"
    else: state_cocina = "apagada"
    if(pasillo == "on"):
        state_pasillo = "encendida"
    else: state_pasillo = "apagada"
    if(habitacion == "on"):
        state_habitacion = "encendida"
    else: state_habitacion = "apagada"
    
    output = "La luz del pasillo está " + state_pasillo + ", la luz de la cocina está " + state_cocina + " y la luz de la habitación está " + state_habitacion + "."
    #output = "La luz de la cocina está " + state_cocina + ", la luz del pasillo está " + state_pasillo + " y la luz de la habitación está " + state_habitacion + "."
    #print(output)
    return build_response(attributes, build_speechlet_response(card, output, None, False))


def get_specific_light_response(specificLight, state):
    attributes = {}
    card = "Luces"
    output = "La " + specificLight + " está " + state + "."
    #print(output)
    return build_response(attributes, build_speechlet_response(card, output, None, False))


def get_all_sensors_response(temperatura, humedad, luminosidad):
    attributes = {}
    card = "Sensores"
    output = "Hay una temperatura de " + temperatura + " grados, una humedad del " + humedad + " % y una luminosidad del " + luminosidad + " %."
    #print(output)
    return build_response(attributes, build_speechlet_response(card, output, None, False))


def get_specific_sensors_response(specificSensor, state):
    attributes = {}
    card = "Ambiente"
    output = "El sensor de " + specificSensor + " mide una "
    
    if specificSensor == 'temperatura':
        output += "temperatura de " + state + " grados."
    elif specificSensor == 'humedad':
        output += "humedad del " + state + " %."
    elif specificSensor == 'luminosidad' or specificSensor == 'luz':
        output += "intensidad de luz del " + state + " %."
    
    #print(output)
    return build_response(attributes, build_speechlet_response(card, output, None, False))
    

def update_device_response(device, action):
    attributes = {}
    card = "Cambiando estado"
    output = "De acuerdo, he "
    
    if action == 'on':
        output += "encendido "
    elif action == 'off':
        output += "apagado "
    
    if device == 'todas':
        output += "todas las luces para ti."
    else: output += "la " + device + " para ti."
    
    #print(output)
    return build_response(attributes, build_speechlet_response(card, output, None, False))


def program_lights_response(v1, v2, v3, percent): # v1=pasillo, v2=cocina, v3=habitacion
    
    attributes = {}
    card = "Programando luces"
    output = "De acuerdo, he programado "
    
    if v1 == v2 == v3 == "on":
        output += "todas las luces para que se enciendan cuando el nivel de luminosidad baje del " + percent + " por ciento."
    
    elif v1 == v2 == "on" or v1 == v3 == "on" or v2 == v3 == "on": # si hay al menos dos luces programadas
        output += "las luces "
        if v1 == "on":
            output += "del pasillo y de la "
            if v2 == "on":
                output += "cocina "
            elif v3 == "on":
                output += "habitación "
        else: output += "de la cocina y de la habitación "
        output += "para que se enciendan cuando el nivel de luminosidad baje del " + percent + " por ciento."
        
    else: # si solo hay una
        output += "la luz "
        if v1 == "on":
            output += "del pasillo "
        elif v2 == "on":
            output += "de la cocina "
        elif v3 == "on":
            output += "de la habitación "
        output += "para que se encienda cuando el nivel de luminosidad baje del " + percent + " por ciento."
    
    #print(output)
    return build_response(attributes, build_speechlet_response(card, output, None, False))


def deprogram_lights_response(deprogrammed):
    attributes = {}
    card = "Programando luces"
    
    if deprogrammed:
        output = "De acuerdo, he eliminado la programación de las luces que había activa."
    else:
        output = "No había ninguna programación activa."
    
    #print(output)
    return build_response(attributes, build_speechlet_response(card, output, None, False))
