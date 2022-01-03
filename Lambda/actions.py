import response_builders
import shadow_connection
import time


# --------------- Funciones de evento  ---------------

def on_launch(launch_request, session):
    return response_builders.get_welcome_response()


def on_session_ended(session_ended_request, session):
    return response_builders.get_goodbye_response()


def on_intent(intent_request, session):

    intent = intent_request['intent']
    intent_name = intent['name']

    if intent_name == "DescribeScreenIntent":
        currentScreen_slot = ""
        if 'value' in intent['slots']['CurrentScreen']:
            currentScreen_slot = intent['slots']['CurrentScreen']['value']
        return describe_screen(intent_request, currentScreen_slot)
        
    elif intent_name == "GetLightsIntent":
        return get_lights()
    
    elif intent_name == "GetSpecificLightIntent":
        slot = intent['slots']['SpecificLight']['value']
        return get_specific_light(slot)
    
    elif intent_name == "GetAllSensorsIntent":
        return get_all_sensors()
        
    elif intent_name == "GetSpecificSensorIntent":
        slot = intent['slots']['SpecificSensor']['value']
        return get_specific_sensor(slot)
        
    elif intent_name == "UpdateDeviceIntent":
        if 'value' not in intent['slots']['LightAction'] and 'value' in intent['slots']['LightDevice']: # to prevent utterance conflict
            if 'enciende' in intent['slots']['LightDevice']['value'] or 'encienda' in intent['slots']['LightDevice']['value'] or 'apaga' in intent['slots']['LightDevice']['value'] or 'apague' in intent['slots']['LightDevice']['value']:
                action = intent['slots']['LightDevice']['value']
                
                if 'enciende' in action or 'encienda' in action:
                    action = 'enciende'
                else: action = 'apaga'
                
                if 'cocina' in intent['slots']['LightDevice']['value']:
                    return update_device('cocina', action)
                elif 'pasillo' in intent['slots']['LightDevice']['value']:
                    return update_device('pasillo', action)
                elif 'habitacion' in intent['slots']['LightDevice']['value'] or 'habitación' in intent['slots']['LightDevice']['value']:
                    return update_device('habitacion', action)
                elif 'luces' in intent['slots']['LightDevice']['value'] or 'bombillas' in intent['slots']['LightDevice']['value']:
                    return update_device('todas', action)
                else: return response_builders.error_lights_response()
                return update_device(device, action)
            else:
                return get_specific_light(intent['slots']['LightDevice']['value'])
        
        action = intent['slots']['LightAction']['value']
        if 'cocina' in intent['slots']['LightDevice']['value']:
            return update_device('cocina', action)
        elif 'pasillo' in intent['slots']['LightDevice']['value']:
            return update_device('pasillo', action)
        elif 'habitacion' in intent['slots']['LightDevice']['value'] or 'habitación' in intent['slots']['LightDevice']['value']:
            return update_device('habitacion', action)
        elif 'luces' in intent['slots']['LightDevice']['value'] or 'bombillas' in intent['slots']['LightDevice']['value']:
            return update_device('todas', action)
        else: return response_builders.error_lights_response()
            
    
    elif intent_name == "ProgramLightsIntent":
        return program_lights(intent['slots']['LightDevice']['value'], intent['slots']['Percentage']['value'])
        
    elif intent_name == "DeprogramLightsIntent":
        return deprogram_lights()


# --------------- Acciones a realizar dependiendo del intent invocado ---------------

def describe_screen(intent, currentScreen_slot):
    
    if(currentScreen_slot):
        return response_builders.get_screen_response(currentScreen_slot, False)
    else:
        shadow = shadow_connection.read_shadow()
        return response_builders.get_screen_response(shadow['pantallaActual'], True)
    

def get_lights():
    
    shadow = shadow_connection.read_shadow()
    return response_builders.get_lights_response(shadow['luz_cocina'], shadow['luz_pasillo'], shadow['luz_habitacion'])
    

def get_specific_light(specificLight):
    
    shadow = shadow_connection.read_shadow()
    
    if 'cocina' in specificLight:
        state = shadow['luz_cocina']
        specificLight = "luz de la cocina"
    elif 'pasillo' in specificLight:
        state = shadow['luz_pasillo']
        specificLight = "luz del pasillo"
    elif 'habitacion' in specificLight or 'habitación' in specificLight:
        state = shadow['luz_habitacion']
        specificLight = "luz de la habitación"
    elif 'todas' in specificLight or 'luces' in specificLight or 'bombillas' in specificLight:
        return response_builders.get_lights_response(shadow['luz_cocina'], shadow['luz_pasillo'], shadow['luz_habitacion'])
    
    if state == 'on':
        state = 'encendida'
    elif state == 'off':
        state = 'apagada'
    
    return response_builders.get_specific_light_response(specificLight, state)


def get_all_sensors():
    
    data = "{\"state\": {\"desired\": {\"temperatura\": \"requested\" }}}"
    shadow, done = shadow_connection.send_request_and_wait_for_response(data)
    
    if done:
        if float(shadow['temperatura']) != -1:
        
            temperatura = str(int(float(shadow['temperatura'])))
            humedad = str(int(float(shadow['humedad'])))
            luminosidad = str(int(float(shadow['luminosidad'])))
            
            return response_builders.get_all_sensors_response(temperatura, humedad, luminosidad)
    else: return response_builders.error_sensors_response()


def get_specific_sensor(specificSensor):
    
    data = "{\"state\": {\"desired\": {\"temperatura\": \"requested\" }}}"
    shadow, done = shadow_connection.send_request_and_wait_for_response(data)
    
    if done:
    
        if specificSensor == 'temperatura':
            state = str(int(float(shadow['temperatura'])))
            if float(shadow['temperatura']) == -1:
                return response_builders.error_sensors_response()
        elif specificSensor == 'humedad':
            state = str(int(float(shadow['humedad'])))
            if float(shadow['humedad']) == -1:
                return response_builders.error_sensors_response()
        elif specificSensor == 'luminosidad' or specificSensor == 'luz':
            specificSensor = 'luminosidad'
            state = str(int(float(shadow['luminosidad'])))
    
        return response_builders.get_specific_sensors_response(specificSensor, state)
    else: return response_builders.error_sensors_response()


def update_device(device, action):
    
    if action == 'enciende' or action == 'encienda':
        value = "on"
    elif action == 'apaga' or action == 'apague':
        value = "off"
    
    if device == 'todas': # todas
        data = "{\"state\": {\"desired\": {\"luz_pasillo\": \"" + value + "\", \"luz_cocina\": \"" + value + "\", \"luz_habitacion\": \"" + value + "\" }}}"
    else: # una luz en especifico
        if 'pasillo' in device:
            dev = 'luz_pasillo'
            device = "luz del pasillo"
        elif 'cocina' in device:
            dev = 'luz_cocina'
            device = "luz de la cocina"
        elif 'habitacion' in device or 'habitación' in device:
            dev = 'luz_habitacion'
            device = "luz de la habitacion"
        data = "{\"state\": {\"desired\": {\"" + dev + "\": \"" + value + "\" }}}"

    
    shadow_connection.update_shadow(data)
    
    return response_builders.update_device_response(device, value)
    

def program_lights(devices, lightness):
    
    if float(lightness) >= 0.0 and float(lightness) <= 100.0:
        
        if 'pasillo' in devices:
            value1 = "on"
        else: value1 = "off"
        if 'cocina' in devices:
            value2 = "on"
        else: value2 = "off"
        if 'habitacion' in devices or 'habitación' in devices:
            value3 = "on"
        else: value3 = "off"
        if 'todas' in devices:
            value1 = "on"
            value2 = "on"
            value3 = "on"
        
        data = "{\"state\": {\"desired\": {\"p_activado\": \"si\", \"p_luz_pasillo\": \"" + value1 + "\", \"p_luz_cocina\": \"" + value2 + "\", \"p_luz_habitacion\": \"" + value3 + "\", \"p_luminosidad\": \"" + lightness + "\" }}}"
        shadow_connection.update_shadow(data)
        
        return response_builders.program_lights_response(value1, value2, value3, lightness)
        
    else: return response_builders.error_response()
    
    
def deprogram_lights():
    
    shadow = shadow_connection.read_shadow()
    if str(shadow['p_activado']) == "si":
        data = "{\"state\": {\"desired\": {\"p_activado\": \"no\" }}}"
        shadow_connection.update_shadow(data)
        return response_builders.deprogram_lights_response(True)
    else: return response_builders.deprogram_lights_response(False)