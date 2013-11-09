################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include CapaLogica/ListaInvertida/subdir.mk
-include CapaLogica/HashExtensible/subdir.mk
-include CapaLogica/ArbolBMas/subdir.mk
-include CapaFisica/Excepciones/subdir.mk
-include CapaFisica/Archivos/ArchivoRegVariable/subdir.mk
-include CapaFisica/Archivos/ArchivoBloque/subdir.mk
-include CapaFisica/Archivos/subdir.mk
-include Aplicacion/Utiles/subdir.mk
-include Aplicacion/Programa/subdir.mk
-include Aplicacion/LectorConfig/Excepciones/subdir.mk
-include Aplicacion/LectorConfig/subdir.mk
-include Aplicacion/Indice/subdir.mk
-include Aplicacion/Entidades/subdir.mk
-include Aplicacion/Diccionario/subdir.mk
-include Aplicacion/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: PaginasDoradas

# Tool invocations
PaginasDoradas: $(OBJS) $(USER_OBJS)


	g++  -o "PaginasDoradas" $(OBJS) $(USER_OBJS) $(LIBS)

	cp config Aplicacion
	mkdir Aplicacion/Indice/Archivos
	cp ArchivosBase/* Aplicacion/Indice/Archivos
	@echo ' '
	@echo 'Paginas Doradas instalado correctamente!'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) PaginasDoradas
	-@echo ' '
	rm Aplicacion/config
	rm Aplicacion/Indice/Archivos/*
	rmdir Aplicacion/Indice/Archivos
	@echo ' '
	@echo 'Paginas Doradas desinstalado correctamente!'
	@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
