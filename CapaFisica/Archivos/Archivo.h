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
	virtual Archivo();
	virtual ~Archivo();
	virtual void subir(char* dato, int tamanioDato);
	virtual void leer(char* &dato, int tamanio, int numBloque);

private:
	fstream archivo;
};

#endif /* ARCHIVO_H_ */
