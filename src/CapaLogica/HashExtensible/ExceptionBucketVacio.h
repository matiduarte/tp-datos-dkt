#ifndef EXCEPTIONBUCKETVACIO_H_
#define EXCEPTIONBUCKETVACIO_H_

#include <exception>
#include "../../Excepcion.h"
using namespace std;

class ExceptionBucketVacio : public Excepcion{

public:
	virtual const char* what() const throw(){
		return "Bucket vacio.";
	}
};


#endif /* EXCEPTIONBUCKETVACIO_H_ */
