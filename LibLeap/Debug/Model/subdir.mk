################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Model/GestureFinger.cpp \
../Model/GestureFrame.cpp \
../Model/GestureHand.cpp \
../Model/Vertex.cpp 

OBJS += \
./Model/GestureFinger.o \
./Model/GestureFrame.o \
./Model/GestureHand.o \
./Model/Vertex.o 

CPP_DEPS += \
./Model/GestureFinger.d \
./Model/GestureFrame.d \
./Model/GestureHand.d \
./Model/Vertex.d 


# Each subdirectory must supply rules for building sources it contributes
Model/%.o: ../Model/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/kuba/Pobrane/Leap_Developer_Kit_0.8.0_5300_Linux/LeapSDK/include -I/home/kuba/Pobrane/libsvm-master -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


