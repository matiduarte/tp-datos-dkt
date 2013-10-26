/*
 * ArchivoBloque.h
 *
 *  Created on: 12/10/2013
 *      Author: marian
 */

#ifndef ARCHIVOBLOQUE_H_
#define ARCHIVOBLOQUE_H_

#include <iostream>
#include <math.h>
#include <string.h>
#include "../Archivo.h"
#include "../../../Aplicacion/LectorConfig/LectorConfig.h"
#include "../../../Aplicacion/constantes.h"
#include "../../Excepciones/ExcepcionBloqueInexistente.h"
#include "../../Excepciones/ExcepcionBloqueIncorrecto.h"
#include "../../Excepciones/ExcepcionOverflowTamBloque.h"
using namespace std;

class ArchivoBloque: Archivo {
public:

	//puede lanzar ExcepcionBloqueIncorrecto
	ArchivoBloque(string nombre);
	~ArchivoBloque();

	//puede lanzar ExcepcionOverflowTamBloque
	unsigned int escribir(char* bloque);

	//puede lanzar ExcepcionOverflowTamBloque y ExcepcionBloqueInexistente
	void reescribir(char* bloque, unsigned int numeroBloque);
	void leer(char* &bloque, unsigned int numBloque);

	//puede lanzar ExcepcionBloqueInexistente
	void borrar(unsigned int numBloque);
	unsigned int getCantidadBloques();
	unsigned int getTamanoBloque();

private:
	vector<char> vectorMapaBits;
	unsigned int tamanioBloque;
	unsigned int cantidadBloques;
	bool esMultiplo(int tamanioBloque);
	unsigned int siguientePosicionLibre();
	void leerEspaciosLibres();
	void escribirEspaciosLibres();
};

#endif /* ARCHIVOBLOQUE_H_ */
