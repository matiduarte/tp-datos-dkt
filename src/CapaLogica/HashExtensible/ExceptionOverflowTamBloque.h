/*
 * ExceptionOverflowTamBloque.h
 *
 *  Created on: 14/04/2010
 *      Author: gonzalo
 */

#ifndef EXCEPTIONOVERFLOWTAMBLOQUE_H_
#define EXCEPTIONOVERFLOWTAMBLOQUE_H_

#include <exception>

using namespace std;

class ExceptionOverflowTamBloque : public exception
{
public:


	virtual const char* what() const throw()
	{
		return "Tamaño a guardar mayor al posible";
	}


};

#endif /* EXCEPTIONOVERFLOWTAMBLOQUE_H_ */
