/*
 * ArchivoBloque.h
 *
 *  Created on: 12/10/2013
 *      Author: marian
 */

#ifndef ARCHIVOBLOQUE_H_
#define ARCHIVOBLOQUE_H_

#include <vector>
#include <iostream>
#include "../Archivo.h"
using namespace std;

class ArchivoBloque: Archivo {
public:
	ArchivoBloque(string nombre);
	virtual ~ArchivoBloque();

	void subir(char* bloque, int tamanioBloque);
	void leer(char* &bloque, int tamanioBloque, int numBloque);
	void borrarBloque(int numeroBloque);

private:

	string nombreArchivo;
	vector<char> vectorMapaBits;
	int siguientePosicionLibre();
	void leerMapaDeBits();
	void escribirMapaBits();

};


#endif /* ARCHIVOBLOQUE_H_ */
