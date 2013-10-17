#include "Excepcion.h"

Excepcion::Excepcion() {
this->mensajeError="";
}

Excepcion::~Excepcion() throw(){};

void Excepcion::mostrarMensaje(){
	cout<<mensajeError<<endl;
}
