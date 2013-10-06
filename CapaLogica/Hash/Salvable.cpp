/*
 * Salvable.cpp
 *
 *  Created on: 08/04/2010
 *      Author: gonzalo
 */

#include "Salvable.h"

using namespace std;

Salvable::Salvable(const string& ruta, const bool& sobreEscribir,const short& tamElem){
	archivo = new ArchivoBloque(ruta, tamElem, sobreEscribir);
}

void* Salvable::leerBloque(const int& numBloque)const{
	return archivo->LeerBloque(numBloque);
}

void Salvable::escribirBloque(void* aGuardar, const int tamanio, const int& numBloque){
	if (tamanio > getTamanioBloque()){
		throw ExceptionOverflowTamBloque();
	}
	archivo->EscribirBloque((char*)aGuardar, numBloque);
}

void Salvable::escribirBloque(Serializable* aGuardar, const int& numBloque){
	if (aGuardar->getTamanio() > getTamanioBloque()){
		throw ExceptionOverflowTamBloque();
	}
	char* buffer = new char[aGuardar->getTamanio()];
	aGuardar->serializar(buffer);
	archivo->EscribirBloque((char*)buffer, numBloque);
	delete[] buffer;
	buffer = NULL;
}

const unsigned int Salvable::getCantidadDeBloques()const{
	return archivo->getCantidadDeBloques();
}

const int Salvable::getTamanioBloque()const{
	return archivo->getTamanioBloque();
}

Salvable::~Salvable(){
	if (archivo) {
		delete archivo;
	}
}

