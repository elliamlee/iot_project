# azure-iot-temp-humidity

C script logs temperature and humidity data from a DHT11 temperature and humidity sensor via a RaspberryPi. it then pushes that information up to an endpoint. I am personally using Microsoft Azure Logic App for this functionality.

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
