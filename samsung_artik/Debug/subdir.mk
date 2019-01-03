################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../jsmn.c 

CPP_SRCS += \
../SimpleDHT.cpp \
../dht_artik_cloud.cpp 

OBJS += \
./SimpleDHT.o \
./dht_artik_cloud.o \
./jsmn.o 

C_DEPS += \
./jsmn.d 

CPP_DEPS += \
./SimpleDHT.d \
./dht_artik_cloud.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/ -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/base -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/base/platform -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/bluetooth -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/connectivity -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/lwm2m -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/media -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/mqtt -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/sensor -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/sensor/platform -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/systemio -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/wifi -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/zigbee -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/base/cpp -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/bluetooth/cpp -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/connectivity/cpp -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/media/cpp -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/mqtt/cpp -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/sensor/cpp -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/systemio/cpp -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/wifi/cpp -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/zigbee/cpp -O0 -g3 -Wall -c -fmessage-length=0 --sysroot=C:/ARTIK/SDK/A5_7_10/v1.4/sysroot -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/ -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/base -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/base/platform -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/bluetooth -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/connectivity -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/lwm2m -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/media -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/mqtt -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/sensor -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/sensor/platform -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/systemio -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/wifi -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/zigbee -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/base/cpp -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/bluetooth/cpp -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/connectivity/cpp -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/media/cpp -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/mqtt/cpp -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/sensor/cpp -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/systemio/cpp -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/wifi/cpp -IC:/ARTIK/SDK/A5_7_10/v1.4/sysroot/usr/include/artik/zigbee/cpp -O0 -g3 -Wall -c -fmessage-length=0 --sysroot=C:/ARTIK/SDK/A5_7_10/v1.4/sysroot -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


