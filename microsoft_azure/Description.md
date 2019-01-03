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

## Required results

### Temperature and Humdity on the monitor
<p align="center"><img src="https://user-images.githubusercontent.com/34092568/50632863-9029b000-0f8c-11e9-82b6-d4e887c9243c.png" weight="500"></p>

### Send the data to Azure
![azure2](https://user-images.githubusercontent.com/34092568/50632864-9029b000-0f8c-11e9-9ebc-a7e962fb6ae5.png)

### Confirm the change of data on Azure
![azure3](https://user-images.githubusercontent.com/34092568/50632866-9029b000-0f8c-11e9-806e-e5c8e20f85d0.png)
