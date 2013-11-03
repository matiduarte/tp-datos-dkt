/*
 * Clave.h
 *
 *  Created on: 18/10/2013
 *      Author: matias
 */

#ifndef CLAVE_H_
#define CLAVE_H_

#include <iostream>
#include "string.h"
#include "../../Aplicacion/constantes.h"
#include "../../Aplicacion/LectorConfig/LectorConfig.h"

using namespace std;

class Clave {

private:

	string id;

	static unsigned int tamanioMaximoBloque;

public:

	Clave();
	Clave(string);
	virtual ~Clave();
	string getClave();
	int getTamanioClave();
	void setClave(string);

	int persistir (char*);
	int hidratar (char*);

};

#endif /* CLAVE_H_ */
