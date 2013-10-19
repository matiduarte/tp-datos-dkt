/*
 * ArbolBMas.cpp
 *
 *  Created on: 18/10/2013
 *      Author: matias
 */

#include "ArbolBMas.h"

ArbolBMas::ArbolBMas(){

}

ArbolBMas::ArbolBMas(string nombre, int tamanioBloque) {
	// TODO Auto-generated constructor stub
	this->archivo = new ArchivoBloque(nombre, tamanioBloque);
	short unsigned int cantidadDeBloque = this->archivo->getCantidadBloques();
	if (cantidadDeBloque == 0){
		//es un archivo nuevo
		//this->raiz = new NodoHoja(archivo);
		//this->raiz->persistir(archivo);
	} else {
		//busco el bloque raiz
		//this->raiz = Nodo::cargar(archivo);

	}

}

ArbolBMas::~ArbolBMas() {
	// TODO Auto-generated destructor stub
	//this->raiz->persistir(this->archivo);
	delete this->archivo;
	delete this->raiz;
}

void ArbolBMas::persistir(){
	//this->raiz->persistir(this->archivo);
}

string ArbolBMas::buscarClave (Clave clave){

	string valor;
	//Si el nivel de la raiz es cero busco directamente ahi
	if (raiz->getNivel() == 0){
		valor = buscarEnLaRaizHoja(clave);
	}else{
		valor = buscarClaveRecursivo(clave, this->raiz);
	}

	return valor;
}

string ArbolBMas::buscarEnLaRaizHoja(Clave clave){

	//return this->((NodoHoja*)raiz)->buscarClave(clave);
	return "Falta programar";
}

string ArbolBMas::buscarClaveRecursivo(Clave clave, Nodo* nodoActual){


	string valor;
    //unsigned int nodoNuevo = 0;
    if (nodoActual->getNivel() == 0){
    	//estoy en una hoja
    	//valor = ((NodoHoja*)nodoActual)->buscarClave(clave);

    } else {
    	//NodoInterno* nodoInt = (NodoInterno*)nodoActual;
    	// int indice = nodoInt->buscarClave(clave);
    	//if (indice == -1){
    		//La clave es mayor a todas
    		//nodoNuevo = nodoInt->getUltimoNodo();
            //} else {
    		//nodoNuevo = nodoInt->buscarNodo(indice);
    	}

    	//Nodo* nodoNuevoHidratado = Nodo::cargar(this->archivo,nodoNuevo);
    	//return buscarClaveRecursivo(clave,nodoNuevoHidratado);
    	return "Faltan metodos nodos";
}
