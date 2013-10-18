/*
 * ExceptionOverflowNodo.h
 *
 *  Created on: 14/04/2010
 *      Author: gonzalo
 */

#ifndef EXCEPTIONOVERFLOWNODO_H_
#define EXCEPTIONOVERFLOWNODO_H_

#include <exception>

using namespace std;

class ExceptionOverflowNodo : public Excepcion{

private:
	int posBloque;
	int tamDispersion;
	list<HashElement> elementos;

public:
	ExceptionOverflowNodo(const int pBloque,const int dispersion,const list<HashElement> elem){
		posBloque=pBloque;
		tamDispersion = dispersion;
		elementos= elem;
	}

	const list<HashElement>& getElementos()const{
		return elementos;
	}

	int getTamDispersion()const{
		return this->tamDispersion;
	}

	const int getPosBloque() const{
		return posBloque;
	}

	virtual const char* what() const throw(){
		return "Overflow en nodo. Se debe intentar balancear con un nuevo Nodo";
	}

	virtual ~ExceptionOverflowNodo() throw (){}
};

#endif /* EXCEPTIONOVERFLOWNODO_H_ */
