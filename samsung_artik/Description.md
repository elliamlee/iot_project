# artik-iot-temp-humidity

The C script logs temperature and humidity data from a DHT11 temperature and humidity sensor via a RaspberryPi. it then pushes that information up to an endpoint. I am personally using Microsoft Azure Logic App for this functionality.

## Hardware Needed
The following is a list of the exact hardware I've tested this on:
- ARTIK 710
- DHT11 3-pin sensor

## Software Needed
This was tested using the following software:

- Samsung ARTIK Cloud for IoT
- The Adafruit_Python_DHT Sensor Library

This script is set to pull two important pieces of data from environment variables:

- The URL of the endpoint where the data is to be sent
- The name of the IOT device sending this data

## Story



## Required results

### Temperature and Humdity on the monitor
<p align="center"><img src="https://user-images.githubusercontent.com/34092568/50632863-9029b000-0f8c-11e9-82b6-d4e887c9243c.png" width="400"></p>

### Send the data to Azure
<p align="center"><img src="https://user-images.githubusercontent.com/34092568/50632864-9029b000-0f8c-11e9-9ebc-a7e962fb6ae5.png" width="400"></p>

### Confirm the change of data on Azure
<p align="center"><img src="https://user-images.githubusercontent.com/34092568/50632866-9029b000-0f8c-11e9-806e-e5c8e20f85d0.png" width="400"></p>

