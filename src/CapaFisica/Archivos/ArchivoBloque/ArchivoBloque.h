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
#include "../../Excepciones/ExcepcionBloqueInexistente.h"
using namespace std;

class ArchivoBloque: Archivo {
public:
	ArchivoBloque(string nombre, int tamanioBloque);
	virtual ~ArchivoBloque();

	int escribir(char* bloque);
	char* leer(int numBloque);

	//puede lanzar ExcepcionBloqueInexistente
	void borrar(unsigned int numBloque);

	int getCantidadBloques();

	//pasar tamanioBloque por config

private:
	string nombreArchivo;
	vector<char> vectorMapaBits;
	int siguientePosicionLibre();
	void leerMapaDeBits();
	void escribirMapaBits();
	int tamanioBloque;

};

#endif /* ARCHIVOBLOQUE_H_ */
