/*
 * Archivo.h
 *
 *  Created on: 12/10/2013
 *      Author: marian
 */

#ifndef ARCHIVO_H_
#define ARCHIVO_H_

#include <fstream>
#include <vector>
#include <string.h>

using namespace std;

class Archivo {
public:
	Archivo();
	virtual ~Archivo();
	virtual unsigned int escribir(char* dato);
	virtual char* leer(unsigned int num);
	virtual void borrar(unsigned int num);

protected:
	fstream archivo;
	string nombreArchivo;
	virtual void escribirEspaciosLibres();
	virtual void leerEspaciosLibres();
};

#endif /* ARCHIVO_H_ */
