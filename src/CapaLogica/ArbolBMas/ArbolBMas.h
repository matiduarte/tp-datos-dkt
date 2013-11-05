/*
 * ArbolBMas.h
 *
 *  Created on: 18/10/2013
 *      Author: matias
 */

#ifndef ARBOLBMAS_H_
#define ARBOLBMAS_H_

#include <iostream>
#include "string.h"
#include <stdlib.h>
#include "../../CapaFisica/Archivos/ArchivoBloque/ArchivoBloque.h"
#include "Nodo.h"
#include "NodoHoja.h"
#include "NodoInterno.h"


using namespace std;

class ArbolBMas {

private:

	//Atributos
	ArchivoBloque* archivo;
	Nodo* raiz;

	//Metodos
	int agregarRecursivamente (Nodo* nodo, Clave clave, string valor);
	int borrarRecursivamente(Nodo* nodo, Clave clave, string valor);
	void mostrarArbolRecursivamente(Nodo* nodo);

	void partirRaiz();
	void partirRaizHoja();
	void partirRaizInterna();


	int partirNodoHoja(NodoInterno* nodoPadre, NodoHoja* nodoHijo );
	int partirNodoInterno(NodoInterno* nodo, NodoInterno* nodoHijo);

	void balancearADerecha(NodoHoja* nodoIzq, NodoHoja* nodoDer, NodoInterno * nodoPadre);
	int balancearNodoHoja(NodoInterno* nodoActual, NodoHoja* nodoUnerflow);
	int equilibrarNodoHoja(NodoInterno* nodoPadre,NodoHoja* nodoIzq, NodoHoja* nodoDer);
	int mergeNodoHoja(NodoInterno* nodoPadre, NodoHoja* nodoUnder, NodoHoja* nodoBalanceo);


	int balancearNodoInterno(NodoInterno* nodoActual,NodoInterno* nodoUnderflow);
	int mergeNodoInternoDerecho(NodoInterno* nodoPadre, NodoInterno* nodoIzquierdo, NodoInterno* nodoDerecho);
	int equilibrarNodoInternoDerecho(NodoInterno* nodoPadre, NodoInterno* nodoIzquierdo, NodoInterno* nodoDerecho);
	int mergeNodoInternoIzquierdo(NodoInterno* nodoPadre, NodoInterno* nodoIzquierdo, NodoInterno* nodoDerecho);
	int equilibrarNodoInternoIzquierdo(NodoInterno* nodoPadre, NodoInterno* nodoIzquierdo, NodoInterno* nodoDerecho);

	string buscarClaveRecursivamente(Clave clave, Nodo* nodoActual);

public:

	ArbolBMas();
	ArbolBMas(string);
	virtual ~ArbolBMas();
	void persistir();

    int agregarValor(Clave clave, string valor);
    int borrarValor(Clave clave, string valor);

    void mostrarArbol();

    string buscarClave (Clave clave);
    list<string> * elementosConIgualClave(Clave clave);
};

#endif /* ARBOLBMAS_H_ */
