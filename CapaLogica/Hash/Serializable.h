/*
 * Serializable.h
 *
 *  Created on: 08/04/2010
 *      Author: gonzalo
 */

#ifndef SERIALIZABLE_H_
#define SERIALIZABLE_H_

using namespace std;
#include<string>
#include "utilidades/Convertidor.h"

class Serializable {
public:
	Serializable(const string separador = "@");
	virtual ~Serializable();

	virtual void serializar(void* aSerializar)  = 0;
	virtual int getTamanio()const = 0;

protected:
	//Cte para separar los elementos
	string separadorElem;

	virtual void desSerializar(const void* aDesSerializar)=0;
};

#endif /* SERIALIZABLE_H_ */
