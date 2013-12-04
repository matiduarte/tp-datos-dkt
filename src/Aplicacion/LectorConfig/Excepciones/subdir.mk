################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Aplicacion/LectorConfig/Excepciones/ExcepcionArchivoInexistente.cpp \
../Aplicacion/LectorConfig/Excepciones/ExcepcionVariableInexistente.cpp 

OBJS += \
./Aplicacion/LectorConfig/Excepciones/ExcepcionArchivoInexistente.o \
./Aplicacion/LectorConfig/Excepciones/ExcepcionVariableInexistente.o 

CPP_DEPS += \
./Aplicacion/LectorConfig/Excepciones/ExcepcionArchivoInexistente.d \
./Aplicacion/LectorConfig/Excepciones/ExcepcionVariableInexistente.d 


# Each subdirectory must supply rules for building sources it contributes
Aplicacion/LectorConfig/Excepciones/%.o: ../Aplicacion/LectorConfig/Excepciones/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


