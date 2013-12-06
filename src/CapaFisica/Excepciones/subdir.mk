################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../CapaFisica/Excepciones/ExcepcionBloqueIncorrecto.cpp \
../CapaFisica/Excepciones/ExcepcionBloqueInexistente.cpp \
../CapaFisica/Excepciones/ExcepcionOverflowTamBloque.cpp \
../CapaFisica/Excepciones/ExcepcionPosicionEnEspacioLibre.cpp 

OBJS += \
./CapaFisica/Excepciones/ExcepcionBloqueIncorrecto.o \
./CapaFisica/Excepciones/ExcepcionBloqueInexistente.o \
./CapaFisica/Excepciones/ExcepcionOverflowTamBloque.o \
./CapaFisica/Excepciones/ExcepcionPosicionEnEspacioLibre.o 

CPP_DEPS += \
./CapaFisica/Excepciones/ExcepcionBloqueIncorrecto.d \
./CapaFisica/Excepciones/ExcepcionBloqueInexistente.d \
./CapaFisica/Excepciones/ExcepcionOverflowTamBloque.d \
./CapaFisica/Excepciones/ExcepcionPosicionEnEspacioLibre.d 


# Each subdirectory must supply rules for building sources it contributes
CapaFisica/Excepciones/%.o: ../CapaFisica/Excepciones/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


