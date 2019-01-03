// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>
#include <stdlib.h>

#include "iothub_client.h"
#include "iothub_message.h"
#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/shared_util_options.h"
#include "iothubtransportmqtt.h"
#include "iothub_client_options.h"
#include "SimpleDHT.h"  // add the header file



#ifdef MBED_BUILD_TIMESTAMP
#define SET_TRUSTED_CERT_IN_SAMPLES
#endif // MBED_BUILD_TIMESTAMP

#ifdef SET_TRUSTED_CERT_IN_SAMPLES
#include "certs.h"
#endif // SET_TRUSTED_CERT_IN_SAMPLES


static const char* connectionString = "HostName=seungeunlee4b9ec.azure-devices.net;DeviceId=SeungeunDevice;SharedAccessKey=wT37Vt8PGrDrUU0b0aMWlQK6kaYU6vcnm+2ddcZeGkY=";


/*static const char* connectionString = HostName=seungeunlee4b9ec.azure-devices.net;SharedAccessKeyName=iothubowner;SharedAccessKey=9BUWoOFNjJQy+GtPBqyLgX18c1wQ8JwsfjIM6oeBXpA=
*/

static int callbackCounter;
static char msgText[1024];
static char propText[1024];
static bool g_continueRunning;
#define MESSAGE_COUNT 100
#define DOWORK_LOOP_NUM     3


typedef struct EVENT_INSTANCE_TAG
{
    IOTHUB_MESSAGE_HANDLE messageHandle;
    size_t messageTrackingId;  // For tracking the messages within the user callback.
} EVENT_INSTANCE;

static IOTHUBMESSAGE_DISPOSITION_RESULT ReceiveMessageCallback(IOTHUB_MESSAGE_HANDLE message, void* userContextCallback)
{
    int* counter = (int*)userContextCallback;
    const char* buffer;
    size_t size;
    MAP_HANDLE mapProperties;
    const char* messageId;
    const char* correlationId;
    const char* userDefinedContentType;
    const char* userDefinedContentEncoding;

    // Message properties
    if ((messageId = IoTHubMessage_GetMessageId(message)) == NULL)
    {
        messageId = "<null>";
    }

    if ((correlationId = IoTHubMessage_GetCorrelationId(message)) == NULL)
    {
        correlationId = "<null>";
    }

    if ((userDefinedContentType = IoTHubMessage_GetContentTypeSystemProperty(message)) == NULL)
    {
        userDefinedContentType = "<null>";
    }

    if ((userDefinedContentEncoding = IoTHubMessage_GetContentEncodingSystemProperty(message)) == NULL)
    {
        userDefinedContentEncoding = "<null>";
    }

    // Message content
    if (IoTHubMessage_GetByteArray(message, (const unsigned char**)&buffer, &size) != IOTHUB_MESSAGE_OK)
    {
        (void)printf("unable to retrieve the message data\r\n");
    }
    else
    {
        (void)printf("Received Message [%d]\r\n Message ID: %s\r\n Correlation ID: %s\r\n Content-Type: %s\r\n Content-Encoding: %s\r\n Data: <<<%.*s>>> & Size=%d\r\n", 
            *counter, messageId, correlationId, userDefinedContentType, userDefinedContentEncoding, (int)size, buffer, (int)size);
        // If we receive the work 'quit' then we stop running
        if (size == (strlen("quit") * sizeof(char)) && memcmp(buffer, "quit", size) == 0)
        {
            g_continueRunning = false;
        }
    }

    // Retrieve properties from the message
    mapProperties = IoTHubMessage_Properties(message);
    if (mapProperties != NULL)
    {
        const char*const* keys;
        const char*const* values;
        size_t propertyCount = 0;
        if (Map_GetInternals(mapProperties, &keys, &values, &propertyCount) == MAP_OK)
        {
            if (propertyCount > 0)
            {
                size_t index;

                printf(" Message Properties:\r\n");
                for (index = 0; index < propertyCount; index++)
                {
                    (void)printf("\tKey: %s Value: %s\r\n", keys[index], values[index]);
                }
                (void)printf("\r\n");
            }
        }
    }

    /* Some device specific action code goes here... */
    (*counter)++;
    return IOTHUBMESSAGE_ACCEPTED;
}



//ADD
void SimpleDHT11(artik_gpio_id pin) {
	pin_pos = 14; // GPIOA14
	int fpoint;

	if ((fpoint = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
		fprintf(stderr, "Unable to open /dev/mem\n");
	}
	gpio_base = (volatile unsigned int *)mmap(0, getpagesize(), PROT_READ | PROT_WRITE,
		MAP_SHARED, fpoint, 0xC001A000); // GPIOA base addr
	if (gpio_base < 0) {
		fprintf(stderr, "Mmap failed.\n");
	}
}



int read_dht(byte* ptemperature, byte* phumidity, byte pdata[40]) {
	int ret = SimpleDHTErrSuccess;

	float temperature = 0;
	float humidity = 0;
	if ((ret = read2(&temperature, &humidity, pdata)) != SimpleDHTErrSuccess) {
		return ret;
	}

	if (ptemperature) {
		*ptemperature = (byte)(int)temperature;
	}

	if (phumidity) {
		*phumidity = (byte)(int)humidity;
	}

	return ret;
}


int read2(float* ptemperature, float* phumidity, byte pdata[40]) {
	int ret = SimpleDHTErrSuccess;

	byte data[40] = { 0 };
	if ((ret = sample(data)) != SimpleDHTErrSuccess) {
		return ret;
	}

	short temperature = 0;
	short humidity = 0;
	if ((ret = parse(data, &temperature, &humidity)) != SimpleDHTErrSuccess) {
		return ret;
	}

	if (pdata) {
		memcpy(pdata, data, 40);
	}
	if (ptemperature) {
		*ptemperature = (int)(temperature >> 8);
	}
	if (phumidity) {
		*phumidity = (int)(humidity >> 8);
	}

	// For example, when remove the data line, it will be success with zero data.
	if (temperature == 0 && humidity == 0) {
		return SimpleDHTErrZeroSamples;
	}

	return ret;
}

int sample(byte data[40]) {
	// empty output data.
	memset(data, 0, 40);

	// According to protocol: https://akizukidenshi.com/download/ds/aosong/DHT11.pdf
	// notify DHT11 to start:
	//    1. PULL LOW 20ms.
	//    2. PULL HIGH 20-40us.
	//    3. SET TO INPUT.
	pinMode(GPIO_OUT);
	digitalWrite(LOW);
	delay_us(20 * 1000);
	digitalWrite(HIGH);
	delay_us(30);
	pinMode(GPIO_IN);

	// DHT11 starting:
	//    1. PULL LOW 80us
	//    2. PULL HIGH 80us
	if (confirm(80, LOW)) {
		return SimpleDHTErrStartLow;
	}

	if (confirm(80, HIGH)) {
		return SimpleDHTErrStartHigh;
	}

	// DHT11 data transmite:
	//    1. 1bit start, PULL LOW 50us
	//    2. PULL HIGH 26-28us, bit(0)
	//    3. PULL HIGH 70us, bit(1)
	for (int j = 0; j < 40; j++) {
		if (confirm(50, LOW)) {
			return SimpleDHTErrDataLow;
		}

		// read a bit, should never call method,
		// for the method call use more than 20us,
		// so it maybe failed to detect the bit0.
		bool ok = false;
		int tick = 0;
		for (int i = 0; i < 8; i++, tick++) {
			if (digitalRead() != HIGH) {
				ok = true;
				break;
			}
			delay_us(10);
		}
		if (!ok) {
			return SimpleDHTErrDataRead;
		}
		data[j] = (tick > 3 ? 1 : 0);
	}

	// DHT11 EOF:
	//    1. PULL LOW 50us.
	if (confirm(50, LOW)) {
		return SimpleDHTErrDataEOF;
	}

	return SimpleDHTErrSuccess;
}


int parse(byte data[40], short* ptemperature, short* phumidity) {
	short humidity = bits2byte(data);
	short humidity2 = bits2byte(data + 8);
	short temperature = bits2byte(data + 16);
	short temperature2 = bits2byte(data + 24);
	byte check = bits2byte(data + 32);
	byte expect = (byte)humidity + (byte)humidity2 + (byte)temperature + (byte)temperature2;
	if (check != expect) {
		return SimpleDHTErrDataChecksum;
	}

	*ptemperature = temperature << 8 | temperature2;
	*phumidity = humidity << 8 | humidity2;

	return SimpleDHTErrSuccess;
}

void pinMode(int mode) {
	if (mode == GPIO_OUT)
		*(gpio_base + 1) |= (0x1 << pin_pos); // set output
	else if (mode == GPIO_IN)
		*(gpio_base + 1) &= ~(0x1 << pin_pos);
}


int digitalRead() {
	return (((*(gpio_base + 6)) & (0x1 << pin_pos)) ? 1 : 0); // read value;
}

void digitalWrite(int value) {
	if (value == 0)
		*gpio_base &= ~(0x1 << pin_pos);
	else
		*gpio_base |= (0x1 << pin_pos);
}


void delay_us(unsigned long usec)
{
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	clock_gettime(CLOCK_MONOTONIC_RAW, &end);
	unsigned long delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start
		.tv_nsec) / 1000;
	while (delta_us < usec) {
		clock_gettime(CLOCK_MONOTONIC_RAW, &end);
		delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
	}
}



int confirm(int us, byte level) {
	int cnt = us / 10;
	if ((us % 10) > 0) {
		cnt++;
	}

	bool ok = false;
	for (int i = 0; i < cnt; i++) {
		delay_us(10);

		if (digitalRead() != level) {
			ok = true;
			break;
		}
	}

	if (!ok) {
		return -1;
	}
	return SimpleDHTErrSuccess;
}


byte bits2byte(byte data[8]) {
	byte v = 0;
	for (int i = 0; i < 8; i++) {
		v += data[i] << (7 - i);
	}
	return v;
}
////////////////////////////////////////////////







static void SendConfirmationCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void* userContextCallback)
{
    EVENT_INSTANCE* eventInstance = (EVENT_INSTANCE*)userContextCallback;
    (void)printf("Confirmation[%d] received for message tracking id = %zu with result = %s\r\n", callbackCounter, eventInstance->messageTrackingId, ENUM_TO_STRING(IOTHUB_CLIENT_CONFIRMATION_RESULT, result));
    /* Some device specific action code goes here... */
    callbackCounter++;
    IoTHubMessage_Destroy(eventInstance->messageHandle);
}

void iothub_client_sample_mqtt_run(void)
{
    IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle;

    EVENT_INSTANCE messages[MESSAGE_COUNT];
    
    //ADD
    SimpleDHT11(ARTIK_A710_GPIO4); 

    g_continueRunning = true;
    srand((unsigned int)time(NULL));
    double avgWindSpeed = 10.0;
    double minTemperature = 20.0;
    double minHumidity = 60.0;
    
    callbackCounter = 0;
    int receiveContext = 0;
   
    //ADD
    int err=0;
    
    if (platform_init() != 0)
    {
        (void)printf("Failed to initialize the platform.\r\n");
    }
    else
    {
        if ((iotHubClientHandle = IoTHubClient_LL_CreateFromConnectionString(connectionString, MQTT_Protocol)) == NULL)
        {
            (void)printf("ERROR: iotHubClientHandle is NULL!\r\n");
        }
        else
        {
            bool traceOn = true;
            IoTHubClient_LL_SetOption(iotHubClientHandle, OPTION_LOG_TRACE, &traceOn);

#ifdef SET_TRUSTED_CERT_IN_SAMPLES
            // For mbed add the certificate information
            if (IoTHubClient_LL_SetOption(iotHubClientHandle, OPTION_TRUSTED_CERT, certificates) != IOTHUB_CLIENT_OK)
            {
                printf("failure to set option \"TrustedCerts\"\r\n");
            }
#endif // SET_TRUSTED_CERT_IN_SAMPLES

            /* Setting Message call back, so we can receive Commands. */
            if (IoTHubClient_LL_SetMessageCallback(iotHubClientHandle, ReceiveMessageCallback, &receiveContext) != IOTHUB_CLIENT_OK)
            {
                (void)printf("ERROR: IoTHubClient_LL_SetMessageCallback..........FAILED!\r\n");
            }
            else
            {
                (void)printf("IoTHubClient_LL_SetMessageCallback...successful.\r\n");

                /* Now that we are ready to receive commands, let's send some messages */
                size_t iterator = 0;
                double temperature = 0;
                double humidity = 0;		
		//ADD
		byte temp = 0;
		byte humi = 0;
                
                do
                {
                    if (iterator < MESSAGE_COUNT)
                    {
                       	//ADD

	err = SimpleDHTErrSuccess;
	if ((err = read_dht(&temp, &humi, NULL)) != SimpleDHTErrSuccess) {
		fprintf(stderr, "Read DHT11 failed, err= %d ", err);
	} else {
		fprintf(stderr, "Sample Result: \n");
		fprintf(stderr, " %d *C, \n", (int)temp);
		fprintf(stderr, " %d H, \n", (int)humi);
		temperature = (double)temp;
		humidity = (double)humi;
		
	}

		
			
			//temperature = minTemperature + (rand() % 10);
                        //humidity = minHumidity +  (rand() % 20);
                        sprintf_s(msgText, sizeof(msgText), "{\"deviceId\":\"SeeunDevice\",\"temperature\":%.2f,\"humidity\":%.2f}", temperature, humidity);
                        if ((messages[iterator].messageHandle = IoTHubMessage_CreateFromByteArray((const unsigned char*)msgText, strlen(msgText))) == NULL)
                        {
                            (void)printf("ERROR: iotHubMessageHandle is NULL!\r\n");
                        }
                        else
                        {

                            (void)IoTHubMessage_SetMessageId(messages[iterator].messageHandle, "MSG_ID");
                            (void)IoTHubMessage_SetCorrelationId(messages[iterator].messageHandle, "CORE_ID");
                            (void)IoTHubMessage_SetContentTypeSystemProperty(messages[iterator].messageHandle, "application%2Fjson");
                            (void)IoTHubMessage_SetContentEncodingSystemProperty(messages[iterator].messageHandle, "utf-8");

                            messages[iterator].messageTrackingId = iterator;
                            MAP_HANDLE propMap = IoTHubMessage_Properties(messages[iterator].messageHandle);
                            (void)sprintf_s(propText, sizeof(propText), temperature > 28 ? "true" : "false");
                            if (Map_AddOrUpdate(propMap, "temperatureAlert", propText) != MAP_OK)
                            {
                                (void)printf("ERROR: Map_AddOrUpdate Failed!\r\n");
                            }

                            if (IoTHubClient_LL_SendEventAsync(iotHubClientHandle, messages[iterator].messageHandle, SendConfirmationCallback, &messages[iterator]) != IOTHUB_CLIENT_OK)
                            {
                                (void)printf("ERROR: IoTHubClient_LL_SendEventAsync..........FAILED!\r\n");
                            }
                            else
                            {
                                (void)printf("IoTHubClient_LL_SendEventAsync accepted message [%d] for transmission to IoT Hub.\r\n", (int)iterator);
                            }
                        }

                    }
                    IoTHubClient_LL_DoWork(iotHubClientHandle);
                    ThreadAPI_Sleep(3000);

                    iterator++;
                } while (g_continueRunning);

                (void)printf("iothub_client_sample_mqtt has gotten quit message, call DoWork %d more time to complete final sending...\r\n", DOWORK_LOOP_NUM);
                size_t index = 0;
                for (index = 0; index < DOWORK_LOOP_NUM; index++)
                {
                    IoTHubClient_LL_DoWork(iotHubClientHandle);
                    ThreadAPI_Sleep(1);
                }
            }
            IoTHubClient_LL_Destroy(iotHubClientHandle);
        }
        platform_deinit();
    }
}

int main(void)
{
    printf("This is source code for assn8 by PYD\n");
    

    iothub_client_sample_mqtt_run();
    return 0;
}
