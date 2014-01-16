################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../FileListener.cpp \
../LMRecorder.cpp \
../LMpre.cpp \
../LeapListener.cpp \
../LeapProcess.cpp \
../LibLeap.cpp \
../RecognizedGesture.cpp \
../main.cpp 

OBJS += \
./FileListener.o \
./LMRecorder.o \
./LMpre.o \
./LeapListener.o \
./LeapProcess.o \
./LibLeap.o \
./RecognizedGesture.o \
./main.o 

CPP_DEPS += \
./FileListener.d \
./LMRecorder.d \
./LMpre.d \
./LeapListener.d \
./LeapProcess.d \
./LibLeap.d \
./RecognizedGesture.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/oli/Pobrane/Leap_Developer_Kit_0.8.0_5300_Linux/LeapSDK/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


