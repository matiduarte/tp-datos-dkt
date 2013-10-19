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
#include <math.h>
#include <string.h>
#include "../Archivo.h"
#include "../../Excepciones/ExcepcionBloqueInexistente.h"
#include "../../Excepciones/ExcepcionBloqueIncorrecto.h"
#include "../../Excepciones/ExcepcionOverflowTamBloque.h"
using namespace std;

class ArchivoBloque: Archivo {
public:

	//puede lanzar ExcepcionBloqueIncorrecto
	ArchivoBloque(string nombre, int tamanioBloque);
	virtual ~ArchivoBloque();

	//puede lanzar ExcepcionOverflowTamBloque
	int escribir(char* bloque);
	char* leer(int numBloque);

	//puede lanzar ExcepcionBloqueInexistente
	void borrar(unsigned int numBloque);
	int getCantidadBloques();

private:
	string nombreArchivo;
	vector<char> vectorMapaBits;
	unsigned int tamanioBloque;
	bool esMultiplo(int tamanioBloque);
	int siguientePosicionLibre();
	void leerMapaDeBits();
	void escribirMapaBits();
};

#endif /* ARCHIVOBLOQUE_H_ */
