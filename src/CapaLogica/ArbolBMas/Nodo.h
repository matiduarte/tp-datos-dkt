/*
 * Nodo.h
 *
 *  Created on: 11/10/2013
 *      Author: juanma
 */

#ifndef NODO_H_
#define NODO_H_

#include "../../CapaFisica/Archivos/ArchivoBloque/ArchivoBloque.h"
#include "Clave.h"
#include "string.h"
#include "RegistroArbol.h"

class Nodo {

private:
	short unsigned int nivel;
	unsigned int cantidadDeElementos;
	unsigned int NumeroDeBloque;

public:
	Nodo();
	virtual ~Nodo();

	/*getters y setters*/
	unsigned int getCantidadDeElementos();
	short unsigned int getNivel();
	unsigned int getNumeroDeBloque();
	void setNumeroDeBloque(unsigned int numeroDeBloque);
	/*----------------*/

	void incrementarNivel();
	void decrementarNivel();




};


#endif /* NODO_H_ */
