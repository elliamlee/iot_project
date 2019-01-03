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
After completing the steps, you will have a streaming data pipeline feeding a dashboard that displays temperature, humidity.
## Required results

### Temperature and Humdity on the monitor
<p align="center"><img src="https://user-images.githubusercontent.com/34092568/50642246-e8bd7500-0fad-11e9-9b6d-df09eeb20b62.png" width="400"></p>

### Send the data to Azure
<p align="center"><img src="https://user-images.githubusercontent.com/34092568/50642247-e8bd7500-0fad-11e9-98d3-2eb2be8f64df.png" width="400"></p>

### Confirm the change of data on Azure
<p align="center"><img src="https://user-images.githubusercontent.com/34092568/50642248-e8bd7500-0fad-11e9-80d3-f936a586c831.png" width="400"></p>

