# Import SDK packages
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient
import logging
import time
import datetime
import argparse
import RPi.GPIO as GPIO
import json
import random
import sys

# Custom MQTT message callback
pub_topic = "sdk/test/soline/Python"
sub_topic = "iot/test/lambda/weather/"

##TouchPadGPIO##
touchPin = 21
GPIO.setmode(GPIO.BCM)
GPIO.setup(touchPin, GPIO.IN)
################

##LEDGPIO##
redPin = 17
greenPin = 27
bluePin = 22
###########

#Will be executed each time a message is received on MQTT topic
def customCallback(client, userdata, message):
    #message = json.loads(message.payload)
    print("Received a new message: {}".format(message.payload))
    message = json.loads(message.payload)
    ledaction = message['ledAction']
    vocalaction = message['vocalAction']
    
    if ledaction == 'same':
        print('Turn on led with green color')
        greenOn()
    elif ledaction == 'hotter':
        print('Turn on led with red color')
        redOn()
    elif ledaction == 'colder':
        print('Turn on led with blue color')
        blueOn()
def blink(pin):
    #GPIO.setmode(GPIO.BOARD)
    GPIO.setup(pin, GPIO.OUT)
    GPIO.output(pin, GPIO.LOW)
    
def turnOff(pin):
    #GPIO.setmode(GPIO.BOARD)
    GPIO.setup(pin, GPIO.OUT)
    GPIO.output(pin, GPIO.HIGH)
    
def redOn():
    blink(redPin)

def redOff():
    turnOff(redPin)
    
def greenOn():
    blink(greenPin)

def greenOff():
    turnOff(greenPin)
    
def blueOn():
    blink(bluePin)

def blueOff():
    turnOff(bluePin)

def whiteOn():
    blink(redPin)
    blink(greenPin)
    blink(bluePin)

def whiteOff():
    turnOff(redPin)
    turnOff(greenPin)
    turnOff(bluePin)
            
# Configure logging
logger = logging.getLogger("AWSIoTPythonSDK.core")
logger.setLevel(logging.DEBUG)
streamHandler = logging.StreamHandler()
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
streamHandler.setFormatter(formatter)
logger.addHandler(streamHandler)


# For certificate based connection
try:
    RPiClient = AWSIoTMQTTClient("RPi01")
    RPiClient.configureEndpoint("a1hzr0azdqhe8m.iot.ap-northeast-2.amazonaws.com", 8883)
    RPiClient.configureCredentials("ca-cer-root.crt", "be9677c59f-private.pem.key", "be9677c59f-certificate.pem.crt.txt")
    RPiClient.configureOfflinePublishQueueing(-1)  # Infinite offline Publish queueing
    RPiClient.configureDrainingFrequency(2)  # Draining: 2 Hz
    RPiClient.configureMQTTOperationTimeout(5)  # 5 sec
except:
    print("Error")

# Connect and subscribe to AWS IoT
RPiClient.connect()
RPiClient.subscribe(sub_topic, 1, customCallback)
time.sleep(2)

def getTouch(count):
    padPressed = GPIO.input(touchPin)
    data = {}
    #data['count'] = count
    #data['Touch'] = int(padPressed)
    data['dateTime'] = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    return data

# Publish to the same topic in a loop forever

alreadyPressed = False
loopCount = 0
cnt = 0

while True:
    touchPressed = GPIO.input(touchPin)

    if touchPressed and not alreadyPressed:
        msg = json.dumps(getTouch(cnt))
        RPiClient.publish(pub_topic, msg, 1)
        cnt += 1
        print msg
    alreadyPressed = touchPressed
    time.sleep(0.1)
    
# For Websocket connection
# myMQTTClient = AWSIoTMQTTClient("myClientID", useWebsocket=True)
# Configurations
# For TLS mutual authentication

# For Websocket
# myMQTTClient.configureEndpoint("YOUR.ENDPOINT", 443)
