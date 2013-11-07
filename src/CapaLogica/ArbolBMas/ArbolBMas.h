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
	string buscarClaveRecursivamente(Clave clave, Nodo* nodoActual);
	void mostrarArbolRecursivamente(Nodo* nodo);


	void solucionarOverflowRaiz();
	void overflowRaizHoja();
	void overflowRaizInterna();
	int overflowNodoHoja(NodoInterno* nodoPadre, NodoHoja* nodoHijo );
	int overflowNodoInterno(NodoInterno* nodo, NodoInterno* nodoHijo);



	void balancearADerecha(NodoHoja* nodoIzq, NodoHoja* nodoDer, NodoInterno * nodoPadre);
	int solucionarUnderflowHoja(NodoInterno* nodoActual, NodoHoja* nodoUnerflow);
	int balancearNodoHoja(NodoInterno* nodoPadre,NodoHoja* nodoIzq, NodoHoja* nodoDer);
	int mergeNodoHoja(NodoInterno* nodoPadre, NodoHoja* nodoUnder, NodoHoja* nodoBalanceo);


	int solucionarUnderflowInterno(NodoInterno* nodoActual,NodoInterno* nodoUnderflow);
	int mergeNodoInternoDerecho(NodoInterno* nodoPadre, NodoInterno* nodoIzquierdo, NodoInterno* nodoDerecho);
	int balancearNodoInternoDerecho(NodoInterno* nodoPadre, NodoInterno* nodoIzquierdo, NodoInterno* nodoDerecho);
	int mergeNodoInternoIzquierdo(NodoInterno* nodoPadre, NodoInterno* nodoIzquierdo, NodoInterno* nodoDerecho);
	int balancearNodoInternoIzquierdo(NodoInterno* nodoPadre, NodoInterno* nodoIzquierdo, NodoInterno* nodoDerecho);

public:

	ArbolBMas();
	ArbolBMas(string);
	virtual ~ArbolBMas();
	void persistir();

    void agregarValor(Clave clave, string valor);
    void borrarValor(Clave clave, string valor);
    void mostrarArbol();

    string buscarClave (Clave clave);
    list<string> * elementosConIgualClave(Clave clave);
};

#endif /* ARBOLBMAS_H_ */
