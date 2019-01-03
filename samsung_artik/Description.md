# artik-iot-temp-humidity

The C script logs temperature and humidity data from a DHT11 temperature and humidity sensor via a ARTIK. it then pushes that information up to an endpoint. I am personally using Microsoft Azure Logic App for this functionality.

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
<p align="center"><img src=https://user-images.githubusercontent.com/34092568/50639641-8a8c9400-0fa5-11e9-8878-8e36e17292f6.png width="400"></p>

Samsung ARTIK Cloud is an open data exchange platform for the Internet of Things, which allows us to collect, store, visualize and analyze data from any device.

This is a simple project that shows how to read temperature and humidity data from sensor, then send data to ARTIK cloud. It's one more step to set up your own IoT system: monitoring temperature and humidity in your home via cloud server.

## Required results

### Temperature and Humdity on the monitor
<p align="center"><img src="https://user-images.githubusercontent.com/34092568/50639909-84e37e00-0fa6-11e9-9902-5b1082c3254b.png" width="400"></p>

### Send the data to ARTIK cloud
<p align="center"><img src="https://user-images.githubusercontent.com/34092568/50639908-84e37e00-0fa6-11e9-9e2d-3314abdd8af9.png" width="400"></p>

### Confirm the change of data on ARTIK cloud
<p align="center"><img src="https://user-images.githubusercontent.com/34092568/50639898-80b76080-0fa6-11e9-8444-08d368856e4a.jpg" width="400"></p>

