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

using namespace std;


class RegistroArbol {

private:

	string clave;
	list<unsigned int>* id;
	unsigned int tamanioClave;


public:
	RegistroArbol(string);
	virtual ~RegistroArbol();

	int agregar(string, unsigned int id);
	int borrar(string , unsigned int id);
	bool existe(unsigned int);

	void setClave(string clave);
	string getClave();
	list<unsigned int>* getID();
	unsigned int getTamanioClave();
};

#endif /* REGISTROARBOL_H_ */
