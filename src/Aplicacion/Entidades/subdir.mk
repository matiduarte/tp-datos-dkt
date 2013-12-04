################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Aplicacion/Entidades/Categoria.cpp \
../Aplicacion/Entidades/Consulta.cpp \
../Aplicacion/Entidades/PedidoCotizacion.cpp \
../Aplicacion/Entidades/Servicio.cpp \
../Aplicacion/Entidades/Usuario.cpp 

OBJS += \
./Aplicacion/Entidades/Categoria.o \
./Aplicacion/Entidades/Consulta.o \
./Aplicacion/Entidades/PedidoCotizacion.o \
./Aplicacion/Entidades/Servicio.o \
./Aplicacion/Entidades/Usuario.o 

CPP_DEPS += \
./Aplicacion/Entidades/Categoria.d \
./Aplicacion/Entidades/Consulta.d \
./Aplicacion/Entidades/PedidoCotizacion.d \
./Aplicacion/Entidades/Servicio.d \
./Aplicacion/Entidades/Usuario.d 


# Each subdirectory must supply rules for building sources it contributes
Aplicacion/Entidades/%.o: ../Aplicacion/Entidades/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


