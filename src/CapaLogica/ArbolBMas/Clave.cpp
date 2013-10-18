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
