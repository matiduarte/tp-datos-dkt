/*
 * Nodo.cpp

 *  Created on: 11/10/2013
 *      Author: juanma
 */

#include "Nodo.h"
#include "NodoInterno.h"
#include "NodoHoja.h"


Nodo::Nodo() {
	this->cantidadDeElementos = 0;
	this->nivel = 0;
	this->NumeroDeBloque = 0;

}

Nodo::~Nodo() {
	// TODO Auto-generated destructor stub
}


unsigned int Nodo::getCantidadDeElementos(){
		return (this->cantidadDeElementos);
}


unsigned int Nodo::getNumeroDeBloque(){
	return this->NumeroDeBloque;
};


void Nodo::setNumeroDeBloque(unsigned int numeroDeBloque){
	this->NumeroDeBloque = numeroDeBloque;
}


unsigned int Nodo::getNivel(){
		return(this->nivel);
}

Nodo* Nodo::cargar(ArchivoBloque* archivo, unsigned int indice){

	char* bloque = new char[1024]; //cambiar
	archivo->leer(bloque, indice);
	unsigned int tamanioInt = sizeof(unsigned int);
	int bytesHidratados = sizeof (unsigned int);

	unsigned int nivel;
	memcpy(&nivel,bloque + bytesHidratados, tamanioInt);

	if (nivel == 0){
		//Es hoja
		return NodoHoja::cargar(archivo, indice);
	}else {
		//Es interno
		return NodoInterno::cargar(archivo, indice);
	}
}

void Nodo::setNivel(unsigned int nivel){
	this->nivel = nivel;
}


void Nodo::setCantidadDeElementos(unsigned int cantidad){
	this->cantidadDeElementos = cantidad;
}

void Nodo::incrementarNivel(){this->nivel++;}


void Nodo::decrementarNivel(){this->nivel--;}

int Nodo::getTamanioUnderflow(){

	return 1024 * 0.5; //MODIFICAR ESTO

}

int Nodo::getTamanioOverflow(){

	return 1024 * 0.8; //MODIFICAR ESTO

}

void Nodo::incrementarCantidadDeElementos(){
	this->setCantidadDeElementos((this->getCantidadDeElementos())+1);
}

void Nodo::decrementarCantidadDeElementos(){
	this->setCantidadDeElementos((this->getCantidadDeElementos())-1);;
}

//Redefinidos en los hijos:
void Nodo::persistir(ArchivoBloque* archivo){}

int Nodo::tamanioOcupado(){return 0;}

bool Nodo::hayOverflow(){return 0;}

bool Nodo::hayUnderflow(){return 0;}

bool Nodo::estaVacio(){return 0;}