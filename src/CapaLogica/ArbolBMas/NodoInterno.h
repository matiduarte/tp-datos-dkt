/*
 * NodoInterno.h
 *
 *  Created on: 13/10/2013
 *      Author: juanma
 */

#ifndef NODOINTERNO_H_
#define NODOINTERNO_H_
#include "Nodo.h"
#include <list>
#include <iostream>
#include "Clave.h"

using namespace std;

class NodoInterno: public Nodo {

private:
	list<unsigned int> hijos;
	list<Clave> claves;

public:
	NodoInterno();
	virtual ~NodoInterno();
};

#endif /* NODOINTERNO_H_ */
