################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../CapaFisica/Archivos/ArchivoBloque/ArchivoBloque.cpp 

OBJS += \
./CapaFisica/Archivos/ArchivoBloque/ArchivoBloque.o 

CPP_DEPS += \
./CapaFisica/Archivos/ArchivoBloque/ArchivoBloque.d 


# Each subdirectory must supply rules for building sources it contributes
CapaFisica/Archivos/ArchivoBloque/%.o: ../CapaFisica/Archivos/ArchivoBloque/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


