/*
 * Archivo.h
 *
 *  Created on: 12/10/2013
 *      Author: marian
 */

#ifndef ARCHIVO_H_
#define ARCHIVO_H_

#include <fstream>
using namespace std;

class Archivo {
public:
	Archivo();
	virtual ~Archivo();
	virtual void escribir(char* dato, int tamanioDato);
	virtual void leer(char* &dato, int tamanio, int numBloque);

protected:
	fstream archivo;
};

#endif /* ARCHIVO_H_ */
