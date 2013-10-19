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
	this->clave = clave;
}

Clave::~Clave() {
	// TODO Auto-generated destructor stub
}

string Clave::getClave(){ return this->clave; }

void Clave::setClave(string clave){

	this->clave = clave;

}

//Devuelve la cantidad de bytes almacenados
int Clave::persistir(char* bloque){

	unsigned int bytesAlmacenados = 0;
	unsigned int tamanioClave = this->clave.size();
	unsigned int tamanioInt = sizeof(unsigned int);

	//Agrego el tamanio de la clave
	memcpy(bloque, (char*)&tamanioClave, tamanioInt);
	bytesAlmacenados += tamanioInt;
	//Agrego la clave
	memcpy(bloque + bytesAlmacenados, this->clave.c_str(), tamanioClave);
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

bool Clave::operator != (const Clave& otra) const{

        return (this->clave != otra.clave);
}

bool Clave::operator == (const Clave& otra) const{

        return (this->clave == otra.clave);
}

bool Clave::operator <= (const Clave& otra) const{

        return (this->clave <= otra.clave);
}

bool Clave::operator >= (const Clave& otra) const{

        return (this->clave >= otra.clave);
}

bool Clave::operator < (const Clave& otra) const{

        return (this->clave < otra.clave);
}

bool Clave::operator > (const Clave& otra) const{

        return (this->clave > otra.clave);
}
