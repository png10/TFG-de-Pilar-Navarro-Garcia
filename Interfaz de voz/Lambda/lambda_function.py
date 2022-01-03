import os
import actions

#alexa_id = os.environ.get('AWS_ALEXA_SKILLS_KIT_ID')


def lambda_handler(event, context):
    
    #print(event)
    #print(context)

    # Comprueba que la peticion viene desde mi skill
    if(event['session']['application']['applicationId'] == 'amzn1.ask.skill.987654321'):
        print("Valid Application ID")
    else:
        print("Invalid Application ID")

    request_type = event['request']['type']
    
    if request_type == "LaunchRequest":
        return actions.on_launch(event['request'], event['session'])
    elif request_type == "IntentRequest":
        return actions.on_intent(event['request'], event['session'])
    elif request_type == "SessionEndedRequest":
        return actions.on_session_ended(event['request'], event['session'])
    """else:
        return actions.on_session_ended(event['request'], event['session'])"""