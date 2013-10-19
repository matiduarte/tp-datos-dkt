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

using namespace std;

class Clave {

private:

	string clave;

public:

	Clave();
	Clave(string);
	virtual ~Clave();
	string getClave();
	void setClave(string);

	int persistir (char*);
	int hidratar (char*);

	bool operator != (const Clave& otra) const;
	bool operator == (const Clave& otra) const;
	bool operator <= (const Clave& otra) const;
	bool operator >= (const Clave& otra) const;
	bool operator < (const Clave& otra) const;
	bool operator > (const Clave& otra) const;
};

#endif /* CLAVE_H_ */
