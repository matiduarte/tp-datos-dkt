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
}espacioLibre;

class ArchivoRegVariable: Archivo {

public:
	ArchivoRegVariable(string nombreArchivo);
	virtual ~ArchivoRegVariable();

	//devuelve la posicion en el archivo donde se guarda el registro
	unsigned int escribir(char* registro);
	char* leer(unsigned int numRegistro);
	void borrar(unsigned int numRegistro);
	unsigned int getCantidadRegistros();

private:
	unsigned int cantidadRegistros;

	vector<espacioLibre> vectorEspaciosLibres;
	void leerEspaciosLibres();
	void escribirEspaciosLibres();
};

#endif /* ARCHIVOREGVARIABLE_H_ */
