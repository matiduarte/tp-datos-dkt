/*
 * Salvable.cpp
 *
 *  Created on: 08/04/2010
 *      Author: gonzalo
 */

#include "Salvable.h"

using namespace std;

Salvable::Salvable(const string& ruta, const bool& sobreEscribir,const short& tamElem){
	//archivo = new ArchivoBloque(ruta, tamElem, sobreEscribir);
	archivo = new ArchivoBloque(ruta, tamElem);
}

void Salvable::leerBloque(const int& numBloque,string* buffer)const{
	*buffer = archivo->leer(numBloque);
}

void Salvable::escribirBloque(const string& bloque, const int& numBloque){
	try {
		if(numBloque < archivo->getCantidadBloques() && archivo->getCantidadBloques() > 0){
			archivo->borrar(numBloque);
		}
		archivo->escribir((char*)bloque.c_str());
	} catch (ExceptionOverflowTamBloque e) {
		throw e;
	}

}

const unsigned int Salvable::getCantidadDeBloques ()const{
	return archivo->getCantidadBloques();
}

Salvable::~Salvable(){
	if (archivo) {
		delete archivo;
	}
}

