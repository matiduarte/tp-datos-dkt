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
	ArchivoBloque* archivo;
	Nodo* raiz;

public:
	ArbolBMas();
	ArbolBMas(string, int);
	virtual ~ArbolBMas();
	void persistir();

	int partirNodoHoja(NodoInterno* nodoPadre, NodoHoja* nodoHijo );
	int partirNodoInterno(NodoInterno* nodoActual, NodoInterno* nodoHijo);

	void partirNodoRaiz();
	void partirNodoRaizHoja();
	void partirNodoRaizInterno();

	int balancearNodoHoja(NodoInterno* nodoPadre, NodoHoja* nodoHijo);
	int mergeNodoHojaDerecho(NodoInterno* nodoPadre, NodoHoja* nodoIzq, NodoHoja* nodoDer);
	int equilibrarNodoHojaDerecho(NodoInterno* nodoPadre,NodoHoja* nodoIzq, NodoHoja* nodoDer);
	int mergeNodoHojaIzquierdo(NodoInterno* nodoPadre,NodoHoja* nodoDer,NodoHoja* nodoIzq);
	int equilibrarNodoHojaIzquierdo(NodoInterno* nodoPadre,NodoHoja* nodoDer,NodoHoja* nodoIzq);

	int balancearNodoInterno(NodoInterno* nodoActual,NodoInterno* nuevoNodoActual);
	int mergeNodoInternoDerecho(NodoInterno* nodoPadre, NodoInterno* nodoIzquierdo, NodoInterno* nodoDerecho);
	int equilibrarNodoInternoDerecho(NodoInterno* nodoPadre, NodoInterno* nodoIzquierdo, NodoInterno* nodoDerecho);
	int mergeNodoInternoIzquierdo(NodoInterno* nodoPadre, NodoInterno* nodoIzquierdo, NodoInterno* nodoDerecho);
	int equilibrarNodoInternoIzquierdo(NodoInterno* nodoPadre, NodoInterno* nodoIzquierdo, NodoInterno* nodoDerecho);

    int Agregar(Clave clave, string valor);
    int Borrar(Clave clave, string valor);
    string buscarClave (Clave clave);

	string buscarEnLaRaizHoja(Clave clave);
	string buscarClaveRecursivo(Clave clave, Nodo* nodoActual);
	int agregarRecursivo (Nodo* nodoActual, Clave clave, string valor);
	int eliminarRecursivo (Nodo* nodoActual, Clave clave, string valor);
	void balancearADerecha(NodoHoja* nodoIzq, NodoHoja* nodoDer, NodoInterno * nodoPadre);
};

#endif /* ARBOLBMAS_H_ */
