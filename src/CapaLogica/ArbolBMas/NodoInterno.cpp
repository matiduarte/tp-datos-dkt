/*
 * NodoInterno.cpp
 *
 *  Created on: 13/10/2013
 *      Author: juanma
 */

#include "NodoInterno.h"

NodoInterno::NodoInterno() {
	// TODO Auto-generated constructor stub
	this->claves = new list<Clave>();
	this->hijos = new list<unsigned int>();
}

NodoInterno::NodoInterno(ArchivoBloque* archivo){

	char bloque[1024];
	this->claves = new list<Clave>();
	this->hijos = new list<unsigned int>();
	unsigned int numeroDeBloque = archivo->escribir(bloque);
	this->setNumeroDeBloque(numeroDeBloque);


}

NodoInterno::~NodoInterno() {
	// TODO Auto-generated destructor stub
}

bool NodoInterno::estaVacio(){
	return this->claves->empty();
}
