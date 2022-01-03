import os
import time
import json
import boto3

thing_name = "NodeMCU-with-devices"

client = boto3.client('iot-data', region_name = 'eu-central-1')


def read_shadow():
    # Devuelve el reported state del shadow del thing "NodeMCU-with-devices" en formato JSON.
    shadow = client.get_thing_shadow(thingName = thing_name)
    payload = json.load(shadow['payload'])
    return payload['state']['reported']


def update_shadow(updated):
    
    client.publish(topic = '$aws/things/NodeMCU-with-devices/shadow/update', qos = 1, payload = updated)
    client.publish(topic = 'desiredTopic', qos = 1, payload = updated)
    
    
def send_request_and_wait_for_response(request):
    
    client.publish(topic = 'desiredTopic', qos = 1, payload = request)
    time.sleep(2.5)
    done = False
    
    shadow = client.get_thing_shadow(thingName = thing_name)
    payload = json.load(shadow['payload'])
    
    shadow_time = int(payload['metadata']['reported']['temperatura']['timestamp'])
    now = int(time.time())
    print("Diferencia: ", now-shadow_time)
    
    if((now - shadow_time) <= 5):
        return payload['state']['reported'], True
    
    return -1, done