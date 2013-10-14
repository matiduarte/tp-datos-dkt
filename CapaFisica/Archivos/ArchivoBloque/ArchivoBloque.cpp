/*
 * ArchivoBloque.cpp
 *
 *  Created on: 12/10/2013
 *      Author: marian
 */

#include "ArchivoBloque.h"

ArchivoBloque::ArchivoBloque(const char* nombre) {
	vectorMapaBits.push_back(true);
	archivo.open(nombre, ios::binary);
}

ArchivoBloque::~ArchivoBloque() {
	archivo.close();
}

void ArchivoBloque::subir(char* bloque, int tamanioBloque){

	int posicion = this->siguientePosicionLibre();
	archivo.seekp(posicion * tamanioBloque);
	archivo.write(bloque, tamanioBloque);
}

int ArchivoBloque::siguientePosicionLibre(){

	int pos;
	for(pos= 0; pos<vectorMapaBits.size() && !vectorMapaBits.at(pos); pos++){};
	if(pos== vectorMapaBits.size()) vectorMapaBits.push_back(false);
	else vectorMapaBits.at(pos) = false;

	return pos;
}

void ArchivoBloque::leer(char* &dato, int tamanio, int numBloque){
	archivo.seekp(tamanio * numBloque);
	archivo.read(dato, tamanio);
}

void ArchivoBloque::borrarBloque(int numeroBloque, int tamanioBloque){
	vectorMapaBits.at(numeroBloque * tamanioBloque) = true;
}

