#ifndef EXCEPTIONELEMENTONOENCONTRADO_H_
#define EXCEPTIONELEMENTONOENCONTRADO_H_

#include <stdexcept>

using namespace std;

class ExceptionElementoNoEncontrado: public runtime_error,public Excepcion{

public:
	ExceptionElementoNoEncontrado(const string &err) : runtime_error (err ) {}

};

#endif /* EXCEPTIONELEMENTONOENCONTRADO_H_ */
