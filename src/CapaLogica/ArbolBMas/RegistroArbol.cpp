/*
 * RegistroArbol.cpp
 *
 *  Created on: 17/10/2013
 *      Author: matias
 */

#include "RegistroArbol.h"

RegistroArbol::RegistroArbol(string clave) {
	// TODO Auto-generated constructor stub

	this->clave = clave;
	this->id = new list<unsigned int>();
	this->tamanioClave = clave.size();
}

RegistroArbol::~RegistroArbol() {
	// TODO Auto-generated destructor stub
	delete this->id;
}

string RegistroArbol::getClave(){

	return this->clave;

}

void RegistroArbol::setClave(string clave){

	this->clave = clave;

}

list<unsigned int>* RegistroArbol::getID(){

	return this->id;

}

bool RegistroArbol::existe(unsigned int id){

	bool existe = false;
	list<unsigned int>::iterator it;
	// Recorro la lista de IDs, en caso de que
	// alguno coincida devuelvo verdadero
	for (it = (this->id)->begin(); it != (this->id)->end(); it++){
		if (*it == id){
			existe = true;
		}
	}
	return existe;
}

int RegistroArbol::agregar(string clave, unsigned int id){


	/* resultado = 0 -> no se pudo agregar
	 * resultado = 1 -> se agrego correctamente
	 * resultado = 2 -> hubo desborde
	 * resultado = 3 -> el identificador ya existe
	 */

	int resultado;


	if (this->existe(id)) {
		resultado = 3;
	} else {
		if ( this->getClave() == clave ) {
			this->id->push_back(id);
			resultado = 1;
		} else {
			resultado = 0;
		}
	}

	return resultado;

}

int RegistroArbol::borrar(string clave, unsigned int id){

	/* resultado = 0 -> no se pudo eliminar
	 * resultado = 1 -> se elimino correctamente
	 * resultado = 2 -> hubo desborde
	 * resultado = 3 -> el identificador no existe
	 */
;
	int resultado;

	if (this->existe(id)){
		// itero sobre la lista hasta encontrar el id
		list<unsigned int>::iterator it;
		for (it = (this->id)->begin(); it != (this->id)->end(); it++){
			if (*it == id)
				this->id->erase(it);
		}
		resultado = 1;
	} else {
		if (this->getClave() == clave){
			resultado = 3;
		} else {
			resultado = 0;
		}
	}

	return resultado;
}


