/*
 * RegistroArbol.cpp
 *
 *  Created on: 17/10/2013
 *      Author: matias
 */

#include "RegistroArbol.h"


RegistroArbol::RegistroArbol(){


}

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

int RegistroArbol::getTamanioValor(){

	return this->valor.size();

}

int RegistroArbol::getTamanioClave(){

	return this->clave.getTamanioClave();

}

void RegistroArbol::setValor(string valor){

	this->valor = valor;

}

bool RegistroArbol::existe(string clave){

	return (this->clave.getClave() == clave);

}

int RegistroArbol::agregar(Clave clave, string valor){

	string id = clave.getClave();
	int resultado;

	if (!(this->existe(id))){
			this->setClave(id);
			this->setValor(valor);
			resultado = 1;
		} else {
			//Ya existe la clave
			resultado = 3;
		}

	return resultado;

}


// Este metodo no borra nada dado que el registro se borra completo del bloque
// lo unico que haces es verificar que coincida la clave y el valor
int RegistroArbol::borrar(Clave clave, string valor){


	/*
	 *  resultado = 0 --> error
	 *  resultado = 3 --> coinciden ambos
	 *
	 */

	string id = clave.getClave();
	int resultado;

	// Si la clave existe y coincide el valor esta todo ok
	if (this->existe(id) && this->valor == valor){
		resultado = 3;
	} else {
		//Si no coincide alguno de los dos hay un error
		resultado = 0;
	}


	return resultado;


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


//Devuelvo un registro con los campos hidratados
RegistroArbol* RegistroArbol::hidratar(char* bloque){

	unsigned int bytesLeidosClave = this->clave.hidratar(bloque);
	unsigned int tamanioInt = sizeof(unsigned int);
	string clave = this->clave.getClave();
	unsigned int tamanioValor;

	//Me fijo cuantos bytes tengo que leer para el dato
	memcpy(&(tamanioValor),bloque + bytesLeidosClave, tamanioInt);
	char bloqueAux[1024]; // ESTO MOFIDICAR POR CONSTANTE TAM BLOQUE
	memcpy(&(bloqueAux),bloque + bytesLeidosClave + tamanioInt, tamanioValor);
	string valor = bloqueAux;

	RegistroArbol* registro = new RegistroArbol(clave, valor);

	return registro;


}

//Devuelvo la cantidad de bytes ocupados en el registro
int RegistroArbol::cantidadDeBytesOcupados(){


	int valor = 0;
	unsigned int tamanioInt = sizeof(unsigned int);

	//Sumo todos los campos del registro
	valor = ((this->getTamanioClave()) + (this->getTamanioValor()) + tamanioInt*2);

	return valor;

}

Clave RegistroArbol::getClaveEntera(){
	return this->clave;
}
