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

	int escribir(char* registro);
	char* leer(int numRegistro);
	void borrar(int numRegistro);
	int getCantidadRegistros();
};

#endif /* ARCHIVOREGVARIABLE_H_ */
