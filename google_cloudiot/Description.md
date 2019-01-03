# google-cloud-iot-temp-humidity

The python script logs temperature and humidity data from a DHT11 temperature and humidity sensor via a RaspberryPi. it then pushes that information up to an endpoint. 

## Hardware Needed
The following is a list of the exact hardware I've tested this on:
- RaspberryPi Zero
- DHT11 3-pin sensor

## Software Needed
This was tested using the following software:

- Raspbian 8.0 (jessie)
- The Adafruit_Python_DHT Sensor Library

This script is set to pull two important pieces of data from environment variables:

- The URL of the endpoint where the data is to be sent
- The name of the IOT device sending this data

## Story
<p align="center"><img src="https://user-images.githubusercontent.com/34092568/50640820-5d41e500-0fa9-11e9-8908-27ad1c29c966.png"></p>

A Raspberry Pi with a weather sensor will be used for the IoT device and several components of the Google Cloud Platform will form the data pipeline. Building out the Raspberry Pi, while beneficial, is an optional portion of this codelab and the streaming weather data can be replaced with a script.

## Required results

### Temperature and Humdity on the monitor
<p align="center"><img src="https://user-images.githubusercontent.com/34092568/50632863-9029b000-0f8c-11e9-82b6-d4e887c9243c.png" width="400"></p>

### Send the data to Azure
<p align="center"><img src="https://user-images.githubusercontent.com/34092568/50632864-9029b000-0f8c-11e9-9ebc-a7e962fb6ae5.png" width="400"></p>

### Confirm the change of data on Azure
<p align="center"><img src="https://user-images.githubusercontent.com/34092568/50632866-9029b000-0f8c-11e9-806e-e5c8e20f85d0.png" width="400"></p>

