################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Aplicacion/Indice/Indice.cpp 

OBJS += \
./Aplicacion/Indice/Indice.o 

CPP_DEPS += \
./Aplicacion/Indice/Indice.d 


# Each subdirectory must supply rules for building sources it contributes
Aplicacion/Indice/%.o: ../Aplicacion/Indice/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


