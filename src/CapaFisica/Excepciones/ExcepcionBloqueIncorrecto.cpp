/*
 * ExcepcionBloqueIncorrecto.cpp
 *
 *  Created on: 17/10/2013
 *      Author: marian
 */

#include "ExcepcionBloqueIncorrecto.h"

ExcepcionBloqueIncorrecto::ExcepcionBloqueIncorrecto() {

	this->mensajeError = "El tamaño del bloque debe ser una potencia de 2 y mayor a 512";
}

