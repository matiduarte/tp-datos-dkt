/*
 * RegistroArbol.h
 *
 *  Created on: 17/10/2013
 *      Author: matias
 */

#ifndef REGISTROARBOL_H_
#define REGISTROARBOL_H_
#include <iostream>
#include "string.h"
#include <list>
#include "Clave.h"

using namespace std;


class RegistroArbol {

private:

	Clave clave;
	string valor;

public:

	RegistroArbol();
	RegistroArbol(Clave, string);
	virtual ~RegistroArbol();

	int agregar(Clave, string);
	int borrar(Clave , string);
	bool existe(string);
	int persistir (char* bloque);
	RegistroArbol* hidratar(char* bloque);

	void setClave(string clave);
	void setValor(string valor);
	string getClave();
	string getValor();
	int getTamanioClave();
	int getTamanioValor();
	int cantidadDeBytesOcupados();
};

#endif /* REGISTROARBOL_H_ */
