################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Util/FileWriterUtil.cpp \
../Util/LogUtil.cpp \
../Util/PathUtil.cpp 

OBJS += \
./Util/FileWriterUtil.o \
./Util/LogUtil.o \
./Util/PathUtil.o 

CPP_DEPS += \
./Util/FileWriterUtil.d \
./Util/LogUtil.d \
./Util/PathUtil.d 


# Each subdirectory must supply rules for building sources it contributes
Util/%.o: ../Util/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/oli/Pobrane/Leap_Developer_Kit_0.8.0_5300_Linux/LeapSDK/include -I/home/oli/Pobrane/libsvm-master -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


