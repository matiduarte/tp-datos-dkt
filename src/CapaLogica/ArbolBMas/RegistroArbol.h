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
	//list<unsigned int>* id;
	//unsigned int tamanioClave;


public:
	RegistroArbol(Clave, string);
	virtual ~RegistroArbol();

	int agregar(string, string);
	int borrar(string , unsigned int id);
	bool existe(string);
	int persistir (char* bloque);
	RegistroArbol* hidratar(char* bloque);

	void setClave(string clave);
	void setValor(string valor);
	string getClave();
	string getValor();
	list<unsigned int>* getID();
	unsigned int getTamanioClave();
};

#endif /* REGISTROARBOL_H_ */
