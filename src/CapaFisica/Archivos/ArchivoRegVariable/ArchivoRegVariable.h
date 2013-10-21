/*
 * ArchivoRegVariable.h
 *
 *  Created on: 12/10/2013
 *      Author: marian
 */

#ifndef ARCHIVOREGVARIABLE_H_
#define ARCHIVOREGVARIABLE_H_

#include "../Archivo.h"
#include "../../Excepciones/ExcepcionPosicionEnEspacioLibre.h"


typedef struct{
	unsigned int inicio;
	unsigned int tamanio;
}espacioLibre;

class ArchivoRegVariable: Archivo {

public:
	ArchivoRegVariable(string nombreArchivo);
	~ArchivoRegVariable();

	//devuelve la posicion en el archivo donde se guarda el registro
	unsigned int escribir(char* registro);
	char* leer(unsigned int posicionBytes);
	//puede lanzar ExcepcionPosicionEnEspacioLibre
	void borrar(unsigned int posicionBytes);
	unsigned int getCantidadRegistros();

private:
	unsigned int cantidadRegistros;

	vector<espacioLibre> vectorEspaciosLibres;
	void leerEspaciosLibres();
	void escribirEspaciosLibres();
};

#endif /* ARCHIVOREGVARIABLE_H_ */
