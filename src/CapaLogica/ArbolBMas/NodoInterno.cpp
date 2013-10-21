/*
 * NodoInterno.cpp
 *
 *  Created on: 13/10/2013
 *      Author: juanma
 */

#include "NodoInterno.h"

NodoInterno::NodoInterno() {
	// TODO Auto-generated constructor stub

}

NodoInterno::NodoInterno(ArchivoBloque* archivo){

	char bloque[1024]; //CAMBIAR ESTOOOOOOO
	unsigned int numeroDeBloque = archivo->escribir(bloque);
	this->setNumeroDeBloque(numeroDeBloque);

}

NodoInterno::~NodoInterno() {
	// TODO Auto-generated destructor stub

}

bool NodoInterno::estaVacio(){

	return this->claves.empty();

}
void NodoInterno::agregarClave(Clave clave){

	if (this->claves.empty()){
		this->claves.push_back(clave);
		return;
	}

	list<Clave>::iterator it_claves;
	it_claves = this->claves.begin();
	while (it_claves != claves.end()){
		if ((*it_claves) > (clave)){
			this->claves.insert(it_claves,clave);
			return;
		}
		it_claves++;
	}

    this->claves.push_back(clave);
    return;
}

list<Clave> NodoInterno::getClaves(){

        return this->claves;

}

list<unsigned int> NodoInterno::getHijos(){

        return this->hijos;

}

void NodoInterno::setHijos(list<unsigned int> hijos){

        this->hijos=hijos;
}

void NodoInterno::setClaves(list<Clave> claves){

        this->claves = claves;
}

void NodoInterno::agregarClaves(list<Clave> lista_claves){

	list<Clave>::iterator it;
	for (it = lista_claves.begin(); it != lista_claves.end(); it++)
		this->agregarClave(*it);

}

void NodoInterno::agregarHijoAlFinal(unsigned int hijo){

	hijos.push_back(hijo);

}

void NodoInterno::agregarHijos(list<unsigned int> hijos){

	list<unsigned int>::iterator it;
	for (it = hijos.begin(); it != hijos.end(); it++)
		this->agregarHijoAlFinal(*it);

}

Clave NodoInterno::getClave(int indice){

	list<Clave>::iterator it;
	int contador = 0;
	Clave claveNula = Clave();
	for (it = this->claves.begin(); it != this->claves.end(); it++){
		if (contador == indice){
			return *it;
		}
		contador++;
	}

	return claveNula;
}

Clave NodoInterno::getPrimerClave(){

	return this->claves.front();

}

Clave NodoInterno::getUltimaClave(){

	return this->claves.back();

}


bool NodoInterno::hayOverflow(){

	int tamanioMaximo = getTamanioOverflow();

	return ((tamanioMaximo) >= (this->tamanioOcupado()));
}

bool NodoInterno::hayUnderflow(){

	int tamanioMinimo = getTamanioUnderflow();

	return ((tamanioMinimo) <= (this->tamanioOcupado()));
}

bool NodoInterno::capacidadMinima(){


	int tamanioMinimo = getTamanioUnderflow();
	int tamanioUltimaClave = claves.back().getTamanioClave() + sizeof(unsigned int);

	return ((tamanioMinimo) >= (this->tamanioOcupado() - tamanioUltimaClave));

}

int NodoInterno::tamanioOcupado(){

	//Tamanio que ocupa el bloque en el nodo
	int tamanioOcupado = 0;

	list<Clave>::iterator it;
	for (it = claves.begin(); it != claves.end(); it++){
		tamanioOcupado += (*it).getTamanioClave();
		tamanioOcupado+=sizeof (unsigned int); // para el tamClave
	}

	list<unsigned int>::iterator it1;
	for (it1 = hijos.begin(); it1 != hijos.end(); it1++){
		tamanioOcupado += sizeof(unsigned int);
	}

	tamanioOcupado += sizeof(unsigned int); //nro de bloque
	tamanioOcupado += sizeof(unsigned int); //nivel
	tamanioOcupado += sizeof(unsigned int); //cantidadDeElementos

	return tamanioOcupado;
}

int NodoInterno::getCantidadDeClaves(){

	return this->claves.size();

}

Clave NodoInterno::getClaveDelMedio(){

	list<Clave>::iterator it;
	int cantClaves = this->getCantidadDeClaves();
	int contador = 0;
	bool llegue = false;
	Clave clave;
	//Itero hasta la mitad
	for (it = claves.begin(); (it != claves.end()) && !(llegue); it++){
		if (contador >= cantClaves/2){
			clave = (*it);
			llegue = true;
		}
		contador++;
	}

	return clave;
}

void NodoInterno::setReferenciaAIzq(Nodo * nodo){

	unsigned int nroBloque = nodo->getNumeroDeBloque();
    this->hijos.push_front(nroBloque);
}

void NodoInterno::setReferenciaADer(Nodo * nodo){

	unsigned int nroBloque = nodo->getNumeroDeBloque();
	this->hijos.push_back(nroBloque);
}


void NodoInterno::persistir(ArchivoBloque * archivo){

	char bloque[1024]; //MODIFICAR ESTO

	unsigned int tamanioOcupado = this->tamanioOcupado();
	unsigned int nivel = getNivel();
	unsigned int tamanioInt = sizeof(unsigned int);
	unsigned int bytesOcupados = 0;

	memcpy(bloque, (char*)&tamanioOcupado, tamanioInt);
	bytesOcupados += tamanioInt;

	memcpy(bloque + bytesOcupados,(char*)&nivel ,tamanioInt);
	bytesOcupados += tamanioInt;

	bytesOcupados += escribirReferenciasANodosInternos(bloque + bytesOcupados);

	archivo->reescribir(bloque, this->getNumeroDeBloque());
}

//Devuelvo la cantidad de bytes persistidos
int NodoInterno::escribirReferenciasANodosInternos(char* bloque){

	int contador = 0;

	list<unsigned int>::iterator itNodos = hijos.begin();
	list<Clave>::iterator itClaves = claves.begin();
	Clave clave;
	unsigned int numeroDeBloque = 0;
	unsigned int tamanioInt = sizeof(unsigned int);

	while(itClaves != claves.end()){

		numeroDeBloque = (*itNodos);

		memcpy(bloque + contador,(char*)&numeroDeBloque,tamanioInt);
		contador += tamanioInt;

		clave = *itClaves;
		contador += clave.persistir(bloque + contador);

		itClaves++;
		itNodos++;
	}

	numeroDeBloque = *itNodos;
	memcpy(bloque + contador,(char*)&numeroDeBloque,tamanioInt);
	contador += tamanioInt;

	return contador;
}
