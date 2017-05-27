################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/FeaturesExtraction.cpp \
../src/Operations.cpp \
../src/test.cpp 

OBJS += \
./src/FeaturesExtraction.o \
./src/Operations.o \
./src/test.o 

CPP_DEPS += \
./src/FeaturesExtraction.d \
./src/Operations.d \
./src/test.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


