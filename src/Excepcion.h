#ifndef EXCEPCION_H_
#define EXCEPCION_H_

#include<iostream>
#include<string>
#include <exception>

using namespace std;

class Excepcion: public exception {
protected:
	string mensajeError;
public:
	Excepcion();
	virtual ~Excepcion() throw();
	virtual void mostrarMensaje();
};

#endif
