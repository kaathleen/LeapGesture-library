################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../RecognitionConfiguration/TestingFingerDiffConf.cpp \
../RecognitionConfiguration/TestingStaticRecConf.cpp \
../RecognitionConfiguration/TrainingFingerDiffConf.cpp \
../RecognitionConfiguration/TrainingStaticRecConf.cpp 

OBJS += \
./RecognitionConfiguration/TestingFingerDiffConf.o \
./RecognitionConfiguration/TestingStaticRecConf.o \
./RecognitionConfiguration/TrainingFingerDiffConf.o \
./RecognitionConfiguration/TrainingStaticRecConf.o 

CPP_DEPS += \
./RecognitionConfiguration/TestingFingerDiffConf.d \
./RecognitionConfiguration/TestingStaticRecConf.d \
./RecognitionConfiguration/TrainingFingerDiffConf.d \
./RecognitionConfiguration/TrainingStaticRecConf.d 


# Each subdirectory must supply rules for building sources it contributes
RecognitionConfiguration/%.o: ../RecognitionConfiguration/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/oli/Pobrane/Leap_Developer_Kit_0.8.0_5300_Linux/LeapSDK/include -I/home/oli/Pobrane/libsvm-master -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


