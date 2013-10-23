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

	bool seAgrego = false;

	if (this->claves.empty()){
		this->claves.push_back(clave);
		seAgrego = true;
	}

	list<Clave>::iterator it_claves;
	it_claves = this->claves.begin();
	while ((it_claves != claves.end()) && !(seAgrego)){
		if ((*it_claves) > (clave)){
			this->claves.insert(it_claves,clave);
		}
		it_claves++;
	}

	if(!seAgrego)
		this->claves.push_back(clave);

}

list<Clave> NodoInterno::getClaves(){

        return this->claves;

}

list<unsigned int> NodoInterno::getHijos(){

        return this->hijos;

}

void NodoInterno::setHijos(list<unsigned int> hijos){

        this->hijos = hijos;
}

void NodoInterno::setClaves(list<Clave> claves){

        this->claves = claves;
}

void NodoInterno::agregarClaves(list<Clave> lista_claves){

	list<Clave>::iterator it;
	for (it = lista_claves.begin(); it != lista_claves.end(); it++)
		this->agregarClave(*it);

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


NodoInterno* NodoInterno::cargar(ArchivoBloque* archivo, unsigned int indice){

	char* bloque = new char[1024]; // modificar esto
	archivo->leer(bloque, indice);
	return NodoInterno::hidratar(bloque,indice);
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

int NodoInterno::buscarClave(Clave clave){

	//Busca el indice del nodo donde se quiere agregar la clave
	//Devuelve -1 si la clave es mayor a todas

	int indice = 0;
	bool mayor = true;
	list<Clave>::iterator it_claves;
	it_claves = claves.begin();
	while (it_claves != claves.end()){
		if ((*it_claves)>(clave)){
			mayor = false;
		}
		indice++;
		it_claves++;
    }

	if (mayor) indice = -1;

	return indice;

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

	bytesOcupados += this->persistirReferenciasANodosInternos(bloque + bytesOcupados);

	archivo->reescribir(bloque, this->getNumeroDeBloque());
}

//Devuelvo la cantidad de bytes persistidos
int NodoInterno::persistirReferenciasANodosInternos(char* bloque){

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


NodoInterno* NodoInterno::hidratar(char* bloque, unsigned int indice){

	unsigned int bytesHidratados = 0;
	unsigned int nroDeBloque = 0;
	unsigned int tamanioInt = sizeof (unsigned int);
	unsigned int tamanioOcupado = 0;

	NodoInterno* nodoHidratado = new NodoInterno();

	memcpy(&(tamanioOcupado), bloque, tamanioInt);
  	bytesHidratados += tamanioInt;

  	unsigned int nivel = nodoHidratado->getNivel();
	memcpy(&(nivel), bloque + bytesHidratados ,tamanioInt);
  	bytesHidratados += tamanioInt;

	while ((bytesHidratados) < (tamanioOcupado - tamanioInt)) {

		char bloqueAux[128]; //VER ESTO
		string clave;
		unsigned int tamanioClave = 0;

		memcpy(&(nroDeBloque),bloque + bytesHidratados, tamanioInt);
		nodoHidratado->getHijos().push_back(nroDeBloque);
		bytesHidratados += tamanioInt;

		memcpy(&(tamanioClave), bloque + bytesHidratados, tamanioInt);
		bytesHidratados += tamanioInt;

		memcpy(bloqueAux, bloque + bytesHidratados, tamanioClave);
		clave = bloqueAux;
		bytesHidratados += tamanioClave;


		nodoHidratado->getClaves().push_back(clave);
		nroDeBloque = 0;
	}

	memcpy(&(nroDeBloque),bloque + bytesHidratados, tamanioInt);
	nodoHidratado->getHijos().push_back(nroDeBloque);
	bytesHidratados += tamanioInt;

	nodoHidratado->setNumeroDeBloque(indice);

	return nodoHidratado;

}


void NodoInterno::PartirNodoADerecha(NodoInterno* hermanoDerecho, NodoInterno* padre){


	/* Parto el nodo a la mitad, pasando la primera clave de la particion al padre
	 * y el resto al hermano derecho con sus respectivos hijos.
	 * Luego setea en el padre como su ultimo hijo al hermano derecho.
	 */
	int cantDeClaves = this->getCantidadDeClaves();
	int contador = 0;
	bool subioClave  = false;

	//Agrega claves a hermano derecho / nodo padre
	list<Clave>::iterator it_claves;
	for (it_claves = claves.begin(); it_claves != claves.end(); it_claves++){
		contador ++;
		if (contador > cantDeClaves/2){
			if (!subioClave){
				subioClave = true;
				padre->agregarClave(*it_claves);
			} else {
				hermanoDerecho->agregarClave(*it_claves);
			}

		}
	}
	//Borra claves de este nodo
	contador = 0;
	while(contador <= cantDeClaves){
		if (contador > (cantDeClaves/2)){
			claves.pop_back();
		}
		contador++;
	}
	//Agrega los hijos a los otros nodos
	contador = 0;
	list<unsigned int>::iterator it_hijos;
	for (it_hijos = hijos.begin(); it_hijos != hijos.end(); it_hijos++){
		if (contador > (cantDeClaves/2))
			hermanoDerecho->agregarHijoAlFinal(*it_hijos);
		contador++;
	}
	//Borra los hijos de este nodo
	contador = 0;
	while(contador <= cantDeClaves){
		if (contador > (cantDeClaves/2)){
			hijos.pop_back();
		}
		contador++;
	}

	padre->setReferenciaAIzq(this);
	padre->setReferenciaADer(hermanoDerecho);

}

list<Clave>* NodoInterno::getMitadDerechaClaves(){

	//Elimina la ultima mitad de las claves del nodo y los devuelve
	list<Clave>::iterator it;
	int cantDeClaves = this->getCantidadDeClaves();
	int contador = 0;
	Clave clave;
	list<Clave>* mitadDerClaves = new list<Clave>();
	//Pongo esta condicion si es par
	if (cantDeClaves%2 == 0){
		while (contador <= cantDeClaves){
			if (contador>cantDeClaves/2){
				clave = claves.back();
				claves.pop_back();
				mitadDerClaves->push_front(clave);
			}
			contador++;
		}
	}else {
		while (contador < cantDeClaves){
			if (contador > cantDeClaves/2){
				clave = claves.back();
				claves.pop_back();
				mitadDerClaves->push_front(clave);
			}
			contador++;
		}
	}

    return mitadDerClaves;
}

list<unsigned int> * NodoInterno::getMitadDerechaHijos(){

	//Elimina la ultima mitad de las claves del nodo y los devuelve
	list<unsigned int>::iterator it;
	int cantDeHijos = this->hijos.size();
	int contador = 0;
	list<unsigned int>* mitdaDerHijos = new list<unsigned int> ();
	unsigned int indice;

	if (cantDeHijos%2==0){
		while (contador<=cantDeHijos){
			if (contador>cantDeHijos/2){
				indice = hijos.back();
				hijos.pop_back();
				mitdaDerHijos->push_front(indice);
			}
			contador++;
		}
	}else {
		while (contador<cantDeHijos){
			if (contador>cantDeHijos/2){
				indice = hijos.back();
				hijos.pop_back();
				mitdaDerHijos->push_front(indice);
			}
			contador++;
		}
	}

	return mitdaDerHijos;
}

/*void NodoInterno::borrarClave(int indice){

	VER SI SE USA **************
	int indicador = 0;
	list<Clave>::iterator it_claves;
	it_claves = claves.begin();
	while (it_claves != claves.end()){
		if (indicador == indice){
			claves.erase(it_claves);
		}
		indicador++;
		it_claves++;
	}
} */

void NodoInterno::borrarClave(Clave clave){

/* Borra la clave, si no la encuenta borra la menor que le sigue,
 * si todas son mayores no hace nada
 * si es la mayor borra la ultima
 */
	list<Clave>::iterator it_claves;
	it_claves = claves.begin();
	Clave claveActual;
	bool encontrado = false;

	while ((it_claves != claves.end()) && !(encontrado)){
		claveActual = *it_claves;
		if (claveActual == clave){
			claves.erase(it_claves);
			encontrado = true;
		}else if (claveActual > clave){
			if (it_claves != claves.begin()){
				it_claves--;
				claves.erase(it_claves);
				encontrado = true;
			}
		}
		it_claves++;
     }
	//Si llego hasta aca es la ultima
	if(!encontrado)	claves.pop_back();
}

void NodoInterno::borrarReferencia(unsigned int hijo){

	list<unsigned int>::iterator it_hijos;
	it_hijos = hijos.begin();
	while (it_hijos != hijos.end()){
		if (*it_hijos == hijo)
			hijos.erase(it_hijos);
		it_hijos++;
    }
}


void NodoInterno::agregarHijoAlFinal(unsigned int hijo){

	hijos.push_back(hijo);
}

void NodoInterno::agregarReferencia(Clave clave, unsigned int nodo){

	int indice = this->buscarClave(clave);
	this->agregarClave(clave);
	bool seAgrego = false;

	//Es el mayor, agrego al final
	if (indice==-1){
		this->agregarHijoAlFinal(nodo);
		seAgrego = true;
    }
	//Si no es el mayor itero hasta encontrar la posicion correspondiente
	int indicador = 0;
	list<unsigned int>::iterator it_hijos;
	it_hijos = hijos.begin();
	while ((it_hijos != hijos.end()) && !(seAgrego)){
		it_hijos++;
		if (indicador == indice){
			hijos.insert(it_hijos,nodo);
		}
		indicador++;
	}
}

unsigned int NodoInterno::getUltimoNodo(){

	return hijos.back();
}

unsigned int NodoInterno::buscarNodo(int indice){

	int contador = 0;
	unsigned int retorno = 0;
	bool encontrado = false;
	list<unsigned int>::iterator it_hijos;
	it_hijos = hijos.begin();

	while ((it_hijos != hijos.end()) && !(encontrado)){
		if (contador == indice){
			retorno = *it_hijos;
			encontrado = true;
		}
		contador++;
		it_hijos++;
	}

	return retorno;
}

unsigned int NodoInterno::getNodoAnteriorA(unsigned int indice){

	// Devuelve el indice anterior al nodo que le pasas,
	// si no lo encuentra, devuelve 0

	int nodoAnterior = 0;
	list<unsigned int>::iterator it_hijos;
	it_hijos = hijos.begin();

	while (it_hijos != hijos.end()){
		if (*it_hijos == indice){
			return nodoAnterior;
		}
		nodoAnterior = *it_hijos;
		it_hijos++;
	}

	return nodoAnterior;
}

