//
// dht_artik_cloud.c
//
#include <artik_mqtt.h>
#include "SimpleDHT.h"
#include "jsmn.h"

#define LOOP_PERIOD 1000
#define LOOP_NUM 300

#define MAX_MSG_LEN	512
#define MAX_UUID_LEN 128

#define LRED 28
#define LBLUE 38

static artik_gpio_handle lred_handle, lblue_handle;
static artik_gpio_config lred_config, lblue_config;

static int count = 0;
static byte temperature = 0;
static byte humidity = 0;

static SimpleDHT11 dht11(ARTIK_A710_GPIO4); // fixed at this pin for now
static artik_mqtt_module *mqtt = NULL;
static artik_loop_module *loop = NULL;
static artik_gpio_module *gpio = NULL;

static int broker_port = 8883;
static char sub_topic[MAX_UUID_LEN + 128];
static artik_mqtt_config config;
static artik_mqtt_msg subscribe_msg;
static artik_mqtt_handle client;
static artik_ssl_config ssl;

static char device_id[MAX_UUID_LEN] = "e9d7f22c849f4f68826926ebec1c8edd";
static char token[MAX_UUID_LEN] = "4787256cf1c34250af49fbb769e4b894";
static char pub_msg[MAX_MSG_LEN] = "\{\"temp\":20.0,  \"humidity\":50.0 \}";
static char pub_topic[MAX_UUID_LEN + 128];

static const char *akc_root_ca =
	"-----BEGIN CERTIFICATE-----\n"
	"MIIE0zCCA7ugAwIBAgIQGNrRniZ96LtKIVjNzGs7SjANBgkqhkiG9w0BAQUFADCB\r\n"
	"yjELMAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQL\r\n"
	"ExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJp\r\n"
	"U2lnbiwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxW\r\n"
	"ZXJpU2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0\r\n"
	"aG9yaXR5IC0gRzUwHhcNMDYxMTA4MDAwMDAwWhcNMzYwNzE2MjM1OTU5WjCByjEL\r\n"
	"MAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQLExZW\r\n"
	"ZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJpU2ln\r\n"
	"biwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxWZXJp\r\n"
	"U2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0aG9y\r\n"
	"aXR5IC0gRzUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCvJAgIKXo1\r\n"
	"nmAMqudLO07cfLw8RRy7K+D+KQL5VwijZIUVJ/XxrcgxiV0i6CqqpkKzj/i5Vbex\r\n"
	"t0uz/o9+B1fs70PbZmIVYc9gDaTY3vjgw2IIPVQT60nKWVSFJuUrjxuf6/WhkcIz\r\n"
	"SdhDY2pSS9KP6HBRTdGJaXvHcPaz3BJ023tdS1bTlr8Vd6Gw9KIl8q8ckmcY5fQG\r\n"
	"BO+QueQA5N06tRn/Arr0PO7gi+s3i+z016zy9vA9r911kTMZHRxAy3QkGSGT2RT+\r\n"
	"rCpSx4/VBEnkjWNHiDxpg8v+R70rfk/Fla4OndTRQ8Bnc+MUCH7lP59zuDMKz10/\r\n"
	"NIeWiu5T6CUVAgMBAAGjgbIwga8wDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8E\r\n"
	"BAMCAQYwbQYIKwYBBQUHAQwEYTBfoV2gWzBZMFcwVRYJaW1hZ2UvZ2lmMCEwHzAH\r\n"
	"BgUrDgMCGgQUj+XTGoasjY5rw8+AatRIGCx7GS4wJRYjaHR0cDovL2xvZ28udmVy\r\n"
	"aXNpZ24uY29tL3ZzbG9nby5naWYwHQYDVR0OBBYEFH/TZafC3ey78DAJ80M5+gKv\r\n"
	"MzEzMA0GCSqGSIb3DQEBBQUAA4IBAQCTJEowX2LP2BqYLz3q3JktvXf2pXkiOOzE\r\n"
	"p6B4Eq1iDkVwZMXnl2YtmAl+X6/WzChl8gGqCBpH3vn5fJJaCGkgDdk+bW48DW7Y\r\n"
	"5gaRQBi5+MHt39tBquCWIMnNZBU4gcmU7qKEKQsTb47bDN0lAtukixlE0kF6BWlK\r\n"
	"WE9gyn6CagsCqiUXObXbf+eEZSqVir2G3l6BFoMtEMze/aiCKm0oHw0LxOXnGiYZ\r\n"
	"4fQRbxC1lfznQgUy286dUV4otp6F01vvpX1FQHKOtw5rDgb7MzVIcbidJ4vEZV8N\r\n"
	"hnacRHr2lVz2XTIIM6RUthg/aFzyQkqFOFSDX9HoLPKsEdao7WNq\r\n"
	"-----END CERTIFICATE-----\n";

// compare json token with a string
int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

void on_connect_subscribe(artik_mqtt_config *client_config, void *user_data, artik_error result) {
	artik_mqtt_handle *client_data = (artik_mqtt_handle *) client_config->handle;
	artik_error ret;

	if (result == S_OK && client_data) {
		// Subscribe to receive actions
		ret = mqtt->subscribe(client_data, subscribe_msg.qos, subscribe_msg.topic);
		if (ret == S_OK)
			fprintf(stdout, "subscribe success\n");
		else
			fprintf(stderr, "subscribe err: %s\n", error_msg(ret));
	}
}

void on_message(artik_mqtt_config *client_config, void *user_data, artik_mqtt_msg *msg)
{
	char *msg_payload;
	jsmntok_t jsmntoks[100];  // We expect no more than 10 tokens 
	static jsmn_parser p;

	if (msg && client_config) {
		jsmn_init(&p);
		msg_payload = (char *)msg->payload;
		fprintf(stdout, "topic %s, content %s\n", msg->topic, msg_payload);
		int num_toks = jsmn_parse(&p, msg_payload, strlen(msg_payload), jsmntoks, sizeof(jsmntoks)/sizeof(jsmntoks[0]));
		if (num_toks < 0) {
			printf("Failed to parse JSON: %d\n", num_toks);
			return;
		}

		 // Assume the top-level element is an object 
		if (num_toks < 1 || jsmntoks[0].type != JSMN_OBJECT) {
			printf("Object expected\n");
			return;
		}

		 // Loop over all keys of the root object 
		int i;
		for (i = 1; i < num_toks; i++) {
			if (jsoneq(msg_payload, &jsmntoks[i], "actions") == 0) {
				if (jsmntoks[i+1].type == JSMN_ARRAY) {
					int j;
					for (j = 0; j < jsmntoks[i+1].size; j++) {
						if (jsmntoks[i+j+2].type == JSMN_OBJECT) {
							int k;
							for (k = 0; k < jsmntoks[i+j+2].size; k++) {
								if (jsoneq(msg_payload, &jsmntoks[i+j+k+3], "name") == 0) {
									if (jsoneq(msg_payload, &jsmntoks[i+j+k+4], "redOn") == 0) {
										fprintf(stdout, "Recv action: RedOn\n");
										gpio->write(lred_handle, 1);
									} else if (jsoneq(msg_payload, &jsmntoks[i+j+k+4], "redOff") == 0) {
										fprintf(stdout, "Recv action: RedOff\n");
										gpio->write(lred_handle, 0);
									} else if (jsoneq(msg_payload, &jsmntoks[i+j+k+4], "blueOn") == 0) {
										fprintf(stdout, "Recv action: blueOn\n");
										gpio->write(lblue_handle, 1);
									} else if (jsoneq(msg_payload, &jsmntoks[i+j+k+4], "blueOff") == 0) {
										fprintf(stdout, "Recv action: blueOff\n");
										gpio->write(lblue_handle, 0);
									}
								}
							}
							j += jsmntoks[i+j+2].size + 1;
						}
					}
					i += jsmntoks[i+1].size + 1;
				}
			}
		}
	}
}

void on_disconnect(artik_mqtt_config *client_config, void *user_data,
							artik_error result)
{
	artik_mqtt_handle *client_data = (artik_mqtt_handle *)
							client_config->handle;

	if (client_data) {
		mqtt->destroy_client(client_data);
		client_data = NULL;
		fprintf(stdout, "Disconnect client \n");
		loop->quit();
	}
}

void on_publish(artik_mqtt_config *client_config, void *user_data, int result)
{
	fprintf(stdout, "message published (%d)\n", result);
}

static int on_periodic_callback(void *user_data) {
    int err = SimpleDHTErrSuccess;
	if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
		fprintf(stderr, "Read DHT11 failed, err= %d ", err);
	} else {
		fprintf(stderr, "Sample Result: \n");
		fprintf(stderr, " %d *C, \n", (int)temperature);
		fprintf(stderr, " %d H, \n", (int)humidity);
	}


	 // Publish message 
	int ret = S_OK;
	snprintf(pub_msg, sizeof(pub_msg), "\{\"temp\":%d,  \"humidity\":%d \}", (int)temperature, (int)humidity);
	snprintf(pub_topic, sizeof(pub_topic), "/v1.1/messages/%s", device_id);
	ret = mqtt->publish(client, 0, false, pub_topic, strlen(pub_msg), pub_msg);
	if (ret == S_OK)
		fprintf(stdout, "publish success\n");
	else
		fprintf(stderr, "publish err: %s\n", error_msg(ret));

	// loop
	count++;
    if (count == LOOP_NUM) {
		mqtt->disconnect(client);
        fprintf(stdout, "%s triggered, exiting loop\n", __func__);
        loop->quit();
        return 0;
    }
    return 1;
}

int main(void) {
	artik_error ret = S_OK;

	loop = (artik_loop_module *) artik_request_api_module("loop");
	mqtt = (artik_mqtt_module *)artik_request_api_module("mqtt");
	gpio = (artik_gpio_module *)artik_request_api_module("gpio");

	/* Setup LEDs */
	lred_config = { LRED, "red", GPIO_OUT, GPIO_EDGE_NONE, 0, NULL };
	lblue_config = { LBLUE, "blue", GPIO_OUT, GPIO_EDGE_NONE, 0, NULL };

	ret = gpio->request(&lred_handle, &lred_config);
	if (ret != S_OK) {
		fprintf(stderr, "cannot request gpio module (%d) \n", ret);
		return ret;
	}

	ret = gpio->request(&lblue_handle, &lblue_config);
	if (ret != S_OK) {
		fprintf(stderr, "cannot request gpio module (%d) \n", ret);
		return ret;
	}

	/* Connect to artik cloud */
	memset(&subscribe_msg, 0, sizeof(artik_mqtt_msg));
	snprintf(sub_topic, sizeof(sub_topic), "/v1.1/actions/%s", device_id);
	subscribe_msg.topic = sub_topic;
	subscribe_msg.qos = 0;

	memset(&config, 0, sizeof(artik_mqtt_config));
	config.client_id = "sub_client";
	config.block = true;
	config.user_name = device_id;
	config.pwd = token;

	// TLS configuration
	memset(&ssl, 0, sizeof(artik_ssl_config));
	ssl.verify_cert = ARTIK_SSL_VERIFY_REQUIRED;
	ssl.ca_cert.data = (char *)akc_root_ca;
	ssl.ca_cert.len = strlen(akc_root_ca);
	config.tls = &ssl;

	// Connect to server
	mqtt->create_client(&client, &config);
	mqtt->set_connect(client, on_connect_subscribe, &subscribe_msg);
	mqtt->set_disconnect(client, on_disconnect, NULL);
	mqtt->set_publish(client, on_publish, NULL);
	mqtt->set_message(client, on_message, NULL);

	mqtt->connect(client, "api.artik.cloud", broker_port);

	/* Read data and publish to cloud */
	int id = 0;
	fprintf(stdout, "%s starting\n", __func__);
	loop->add_periodic_callback(&id, LOOP_PERIOD, on_periodic_callback, NULL);
	loop->run();

	gpio->release(lred_handle);
	gpio->release(lblue_handle);
	artik_release_api_module(loop);
	artik_release_api_module(gpio);
	artik_release_api_module(mqtt);
	return (ret == S_OK) ? 0 : -1;
}
