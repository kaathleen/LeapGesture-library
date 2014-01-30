################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Model/RecognitionModule/TestingClassResult.cpp \
../Model/RecognitionModule/TestingFrame.cpp \
../Model/RecognitionModule/TestingResult.cpp \
../Model/RecognitionModule/TrainingClassDataset.cpp \
../Model/RecognitionModule/TrainingClassResult.cpp \
../Model/RecognitionModule/TrainingResult.cpp 

OBJS += \
./Model/RecognitionModule/TestingClassResult.o \
./Model/RecognitionModule/TestingFrame.o \
./Model/RecognitionModule/TestingResult.o \
./Model/RecognitionModule/TrainingClassDataset.o \
./Model/RecognitionModule/TrainingClassResult.o \
./Model/RecognitionModule/TrainingResult.o 

CPP_DEPS += \
./Model/RecognitionModule/TestingClassResult.d \
./Model/RecognitionModule/TestingFrame.d \
./Model/RecognitionModule/TestingResult.d \
./Model/RecognitionModule/TrainingClassDataset.d \
./Model/RecognitionModule/TrainingClassResult.d \
./Model/RecognitionModule/TrainingResult.d 


# Each subdirectory must supply rules for building sources it contributes
Model/RecognitionModule/%.o: ../Model/RecognitionModule/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/oli/Pobrane/Leap_Developer_Kit_0.8.0_5300_Linux/LeapSDK/include -I/home/oli/Pobrane/libsvm-master -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


