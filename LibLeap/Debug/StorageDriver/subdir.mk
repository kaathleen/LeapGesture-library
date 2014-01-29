################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../StorageDriver/BinaryFileStorageDriver.cpp 

OBJS += \
./StorageDriver/BinaryFileStorageDriver.o 

CPP_DEPS += \
./StorageDriver/BinaryFileStorageDriver.d 


# Each subdirectory must supply rules for building sources it contributes
StorageDriver/%.o: ../StorageDriver/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/kuba/Pobrane/Leap_Developer_Kit_0.8.0_5300_Linux/LeapSDK/include -I/home/kuba/Pobrane/libsvm-master -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


