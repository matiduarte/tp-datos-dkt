################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../CapaFisica/Archivos/ArchivoRegVariable/ArchivoRegVariable.cpp 

OBJS += \
./CapaFisica/Archivos/ArchivoRegVariable/ArchivoRegVariable.o 

CPP_DEPS += \
./CapaFisica/Archivos/ArchivoRegVariable/ArchivoRegVariable.d 


# Each subdirectory must supply rules for building sources it contributes
CapaFisica/Archivos/ArchivoRegVariable/%.o: ../CapaFisica/Archivos/ArchivoRegVariable/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


