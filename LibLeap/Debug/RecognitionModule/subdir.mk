################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../RecognitionModule/AbstractSvmModuleBase.cpp \
../RecognitionModule/FingerDiff.cpp \
../RecognitionModule/SVMclassificator.cpp \
../RecognitionModule/StaticRec.cpp 

OBJS += \
./RecognitionModule/AbstractSvmModuleBase.o \
./RecognitionModule/FingerDiff.o \
./RecognitionModule/SVMclassificator.o \
./RecognitionModule/StaticRec.o 

CPP_DEPS += \
./RecognitionModule/AbstractSvmModuleBase.d \
./RecognitionModule/FingerDiff.d \
./RecognitionModule/SVMclassificator.d \
./RecognitionModule/StaticRec.d 


# Each subdirectory must supply rules for building sources it contributes
RecognitionModule/%.o: ../RecognitionModule/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/oli/Pobrane/Leap_Developer_Kit_0.8.0_5300_Linux/LeapSDK/include -I/home/oli/Pobrane/libsvm-master -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


