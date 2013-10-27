#ifndef EXCEPTIONUNDERFLOWNODO_H_
#define EXCEPTIONUNDERFLOWNODO_H_

#include <exception>

using namespace std;

class ExceptionUnderflowNodo : public Excepcion {

private:
	int tamDispersion;

public:
	ExceptionUnderflowNodo(const int dispersion){
		tamDispersion = dispersion;
	}

	int getTamDispersion(){
		return this->tamDispersion;
	}

	virtual const char* what() const throw(){
		return "Underflow en nodo. Se debe intentar liberar Nodo";
	}

	virtual ~ExceptionUnderflowNodo() throw (){}
};

#endif /* EXCEPTIONUNDERFLOWNODO_H_ */
