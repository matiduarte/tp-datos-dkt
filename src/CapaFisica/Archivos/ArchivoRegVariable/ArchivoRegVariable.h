/*
 * ArchivoRegVariable.h
 *
 *  Created on: 12/10/2013
 *      Author: marian
 */

#ifndef ARCHIVOREGVARIABLE_H_
#define ARCHIVOREGVARIABLE_H_

#include "../Archivo.h"

typedef struct{
	unsigned int inicio;
	unsigned int tamanio;
}espacioVacio;

class ArchivoRegVariable: Archivo {
public:

	ArchivoRegVariable();
	virtual ~ArchivoRegVariable();

	unsigned int escribir(char* registro);
	char* leer(unsigned int numRegistro);
	void borrar(unsigned int numRegistro);
	unsigned int getCantidadRegistros();
};

#endif /* ARCHIVOREGVARIABLE_H_ */
