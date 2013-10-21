/*
 * Clave.cpp
 *
 *  Created on: 18/10/2013
 *      Author: matias
 */

#include "Clave.h"


Clave::Clave(){

}

Clave::Clave(string clave) {
	// TODO Auto-generated constructor stub
	this->id = clave;
}

Clave::~Clave() {
	// TODO Auto-generated destructor stub
}

string Clave::getClave(){ return this->id; }

void Clave::setClave(string clave){

	this->id = clave;

}


int Clave::persistir(char* bloque){

	//Devuelve la cantidad de bytes almacenados
	unsigned int bytesAlmacenados = 0;
	unsigned int tamanioClave = this->id.size();
	unsigned int tamanioInt = sizeof(unsigned int);

	//Agrego el tamanio de la clave
	memcpy(bloque, (char*)&tamanioClave, tamanioInt);
	bytesAlmacenados += tamanioInt;
	//Agrego la clave
	memcpy(bloque + bytesAlmacenados, this->id.c_str(), tamanioClave);
	bytesAlmacenados += tamanioClave;

	return bytesAlmacenados;

}

int Clave::hidratar(char* bloque){

	//Devuelve la cantidad de bytes hidratados
	int bytesLeidos = 0;
	unsigned int tamanioInt = sizeof(unsigned int);
	unsigned int tamanioClave;
	string clave;
	//Aca usar constante de tamanioBloque
	char bloqueAux[1024];

	memcpy(&tamanioClave, bloque, tamanioInt);
	bytesLeidos += tamanioInt;
	memcpy(bloqueAux, bloque + tamanioInt, tamanioClave);
	bytesLeidos += tamanioClave;
	clave = bloqueAux;
	//Seteo la clave nueva
	this->setClave(clave);

	return bytesLeidos;

}

