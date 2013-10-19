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
	virtual int escribir(char* dato);
	virtual char* leer(int numBloque);

protected:
	fstream archivo;
};

#endif /* ARCHIVO_H_ */
