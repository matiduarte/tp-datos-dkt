#ifndef EXCEPTIONELEMENTOKEYYAINGRESADO_H_
#define EXCEPTIONELEMENTOKEYYAINGRESADO_H_

#include <stdexcept>

using namespace std;

class ExceptionElementoKeyYaIngresado: public runtime_error,public Excepcion{

public:
	ExceptionElementoKeyYaIngresado(const string &err) : runtime_error (err ) {}

};

#endif /* EXCEPTIONELEMENTOKEYYAINGRESADO_H_ */
