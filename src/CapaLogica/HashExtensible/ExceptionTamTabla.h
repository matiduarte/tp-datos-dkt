#ifndef EXCEPTIONTAMTABLA_H_
#define EXCEPTIONTAMTABLA_H_

#include <exception>
using namespace std;

class ExceptionTamTabla : public Excepcion{

public:
	virtual const char* what() const throw(){
		return "ERROR - tamano de Tabla muy grande para bloques tan chicos";
	}
};

#endif /* EXCEPTIONTAMTABLA_H_ */
