/*
 * RegistroArbol.cpp
 *
 *  Created on: 17/10/2013
 *      Author: matias
 */

#include "RegistroArbol.h"

RegistroArbol::RegistroArbol(Clave clave, string valor) {
	// TODO Auto-generated constructor stub
	this->clave = clave;
	this->valor = valor;
}

RegistroArbol::~RegistroArbol() {
	// TODO Auto-generated destructor stub
}

string RegistroArbol::getClave(){

	return this->clave.getClave();

}

void RegistroArbol::setClave(string clave){

	this->clave.setClave(clave);

}

string RegistroArbol::getValor(){

	return this->valor;

}

void RegistroArbol::setValor(string valor){

	this->valor = valor;

}

bool RegistroArbol::existe(string clave){

	return (this->clave.getClave() == clave);

}

int RegistroArbol::agregar(string clave, string valor){


	/* resultado = 0 -> no se pudo agregar
	 * resultado = 1 -> se agrego correctamente
	 * resultado = 2 -> hubo desborde
	 * resultado = 3 -> el identificador ya existe
	 */

	return 0;

}

int RegistroArbol::borrar(string clave, unsigned int id){

	/* resultado = 0 -> no se pudo eliminar
	 * resultado = 1 -> se elimino correctamente
	 * resultado = 2 -> hubo desborde
	 * resultado = 3 -> el identificador no existe
	 */

	return 0;
}


int RegistroArbol::persistir(char* bloque){
	//Devuelve la cantidad de bytes almacenados

	unsigned int bytesAlmacenados = this->clave.persistir(bloque);
	unsigned int tamanioInt = sizeof (unsigned int);
	unsigned int tamanioValor = this->valor.size();

	//Primero agrego el tamanio del valor
	memcpy(bloque + bytesAlmacenados, (char*)&tamanioValor, tamanioInt);
	bytesAlmacenados += tamanioInt;
	//Luego el dato
	memcpy(bloque + bytesAlmacenados, this->valor.c_str(), tamanioValor);
	bytesAlmacenados += tamanioValor;

	return bytesAlmacenados;
}
