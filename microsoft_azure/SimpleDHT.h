#ifndef __SIMPLE_DHT_H
#define __SIMPLE_DHT_H


#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <artik_module.h>
#include <artik_gpio.h>
#include <artik_loop.h>
#include <artik_platform.h>
#include <time.h>
#include <sys/mman.h>
#include <fcntl.h>
// Success.
#define SimpleDHTErrSuccess 0
// Error to wait for start low signal.
#define SimpleDHTErrStartLow 100
// Error to wait for start high signal.
#define SimpleDHTErrStartHigh 101
// Error to wait for data start low signal.
#define SimpleDHTErrDataLow 102
// Error to wait for data read signal.
#define SimpleDHTErrDataRead 103
// Error to wait for data EOF signal.
#define SimpleDHTErrDataEOF 104
// Error to validate the checksum.
#define SimpleDHTErrDataChecksum 105
// Error when temperature and humidity are zero, it shouldn't happen.
#define SimpleDHTErrZeroSamples 106

// very dirty fix
#define byte char
#define LOW 0
#define HIGH 1


void test_printout_pyd();
int read_dht(byte* ptemperature, byte* phumidity, byte pdata[40]);

int read2(float* ptemperature, float* phumidity, byte pdata[40]);
int digitalRead();

 void digitalWrite(int value);
 void pinMode(int mode);

// origin usleep() allows scheduling, thus cannot satisfy
// critical timing requirement at 10s of us. Use this function instead.

 void delay_us(unsigned long usec);
// confirm the OUTPUT is level in us, 
// for example, when DHT11 start sample, it will
//    1. PULL LOW 80us, call confirm(pin, 80, LOW)
//    2. PULL HIGH 80us, call confirm(pin, 80, HIGH)
// @return 0 success; oterwise, error.
// @remark should never used to read bits, 
//    for function call use more time, maybe never got bit0.
// @remark please use simple_dht11_read().

 int confirm(int us, byte level);
// @data the bits of a byte.
// @remark please use simple_dht11_read().

 byte bits2byte(byte data[8]);

// read temperature and humidity from dht11.
// @param pin the pin for DHT11, for example, 2.
// @param data a byte[40] to read bits to 5bytes.
// @return 0 success; otherwise, error.
// @remark please use simple_dht11_read().
 int sample(byte data[40]);

// parse the 40bits data to temperature and humidity.
// @remark please use simple_dht11_read().

 int parse(byte data[40], short* ptemperature, short* phumidity);

 volatile unsigned int * gpio_base;
 unsigned pin_pos;

 void SimpleDHT11(artik_gpio_id pin);
 
int read2(float* ptemperature, float* phumidity, byte pdata[40]);

 int sample(byte data[40]);




 int read2(float* ptemperature, float* phumidity, byte pdata[40]);

 int sample(byte data[40]);





#endif

