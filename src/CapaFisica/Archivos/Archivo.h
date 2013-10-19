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
	virtual unsigned int escribir(char* dato);
	virtual char* leer(unsigned int num);
	virtual void borrar(unsigned int num);

protected:
	fstream archivo;
};

#endif /* ARCHIVO_H_ */
