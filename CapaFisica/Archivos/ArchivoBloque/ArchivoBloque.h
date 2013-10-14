/*
 * ArchivoBloque.h
 *
 *  Created on: 12/10/2013
 *      Author: marian
 */

#ifndef ARCHIVOBLOQUE_H_
#define ARCHIVOBLOQUE_H_

#include <vector>
using namespace std;

class ArchivoBloque: Archivo {
public:
	ArchivoBloque(const char* nombre);
	virtual ~ArchivoBloque();

	void subir(char* bloque, int tamanioBloque);
	void leer(char* &bloque, int tamanioBloque, int numBloque);

private:
	vector<bool> vectorMapaBits;

	int siguientePosicionLibre();
	void borrarBloque(int numeroBloque, int tamanioBloque);

};


#endif /* ARCHIVOBLOQUE_H_ */
