/*
 * ExceptionElementoNoEncontrado.h
 *
 *  Created on: 28/04/2010
 *      Author: gonzalo
 */

#ifndef EXCEPTIONELEMENTONOENCONTRADO_H_
#define EXCEPTIONELEMENTONOENCONTRADO_H_

#include <stdexcept>

using namespace std;

class ExceptionElementoNoEncontrado: public runtime_error{

public:
	ExceptionElementoNoEncontrado(const string &err) : runtime_error (err ) {}

};

#endif /* EXCEPTIONELEMENTONOENCONTRADO_H_ */
