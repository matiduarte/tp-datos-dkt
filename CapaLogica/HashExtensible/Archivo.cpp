/*
 * Archivo.cpp
 *
 *  Created on: 01/05/2010
 *      Author: gonzalo
 */

#include "Archivo.h"

Archivo::Archivo(const string& r,const bool& sobreEscribir){
	if (sobreEscribir){
		archivo.open(r.c_str(), ios::in | ios::out | ios::trunc | ios::binary);
	} else {
		archivo.open(r.c_str(), ios::in | ios::out | ios::binary);
		//si no se pudo abrir el archivo, es porque no existe entonces lo creo...
		if (archivo.fail()){
			archivo.open(r.c_str(), ios::in | ios::out | ios::trunc | ios::binary);
		}
	}
}

fstream& Archivo::getStream(){
	return archivo;
}

Archivo::~Archivo() {
	if (archivo.is_open()){
		archivo.close();
	}
}
