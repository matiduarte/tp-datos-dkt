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

void Salvable::leerBloque(const int& numBloque,string* buffer)const{
	*buffer = archivo->LeerBloque(numBloque);
}

void Salvable::escribirBloque(const string& bloque, const int& numBloque){
	try {
		archivo->EscribirBloque(bloque, numBloque);
	} catch (ExceptionOverflowTamBloque e) {
		throw e;
	}

}

const unsigned int Salvable::getCantidadDeBloques ()const{
	return archivo->getCantidadDeBloques();
}

Salvable::~Salvable(){
	if (archivo) {
		delete archivo;
	}
}

