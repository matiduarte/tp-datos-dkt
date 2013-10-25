/*
 * ArbolBMas.cpp
 *
 *  Created on: 18/10/2013
 *      Author: matias
 */

#include "ArbolBMas.h"

ArbolBMas::ArbolBMas(){

}

ArbolBMas::ArbolBMas(string nombre, int tamanioBloque) {
	// TODO Auto-generated constructor stub
	this->archivo = new ArchivoBloque(nombre, tamanioBloque);
	unsigned int cantidadDeBloque = this->archivo->getCantidadBloques();
	if (cantidadDeBloque == 0){
		//es un archivo nuevo
		this->raiz = new NodoHoja(archivo);
		this->raiz->persistir(archivo);
	} else {
		//busco el bloque raiz
		this->raiz = Nodo::cargar(archivo, 0);
	}

}

ArbolBMas::~ArbolBMas() {
	// TODO Auto-generated destructor stub
	this->raiz->persistir(this->archivo);
	delete this->archivo;
	delete this->raiz;
}

void ArbolBMas::persistir(){
	this->raiz->persistir(this->archivo);
}

string ArbolBMas::buscarClave (Clave clave){

	string valor;
	//Si el nivel de la raiz es cero busco directamente ahi
	if (raiz->getNivel() == 0){
		valor = buscarEnLaRaizHoja(clave);
	}else{
		valor = buscarClaveRecursivo(clave, this->raiz);
	}

	return valor;
}

string ArbolBMas::buscarEnLaRaizHoja(Clave clave){

	return ((NodoHoja*)raiz)->buscarClave(clave);
}

string ArbolBMas::buscarClaveRecursivo(Clave clave, Nodo* nodoActual){


	string valor;
    unsigned int nodoNuevo = 0;
    if (nodoActual->getNivel() == 0){
    	//estoy en una hoja
    	valor = ((NodoHoja*)nodoActual)->buscarClave(clave);

    } else {
    		//estoy en un nodo interno
    		NodoInterno* nodoInt = (NodoInterno*)nodoActual;
    		int indice = nodoInt->buscarClave(clave);
    		//La clave es mayor a todas
    		if (indice == -1){
    			nodoNuevo = nodoInt->getUltimoNodo();
            } else {
            	nodoNuevo = nodoInt->buscarNodo(indice);
            }
    	Nodo* nodoNuevoHidratado = Nodo::cargar(this->archivo,nodoNuevo);
    	valor =  buscarClaveRecursivo(clave,nodoNuevoHidratado);
    }

    return valor;
}

int ArbolBMas::agregarValor(Clave clave, string valor){

	//Agrega una clave con su respectivo valor
	//Devuelve: 0 si el nodo no se actualizo
	//1 si el nodo se actualizo
	//3 si ya existe el valoe
	int resultado = 0;
	if (this->raiz->getNivel() == 0){
		//Si la raiz tiene 0 de nivel entonces es un nodo hoja
		resultado = ((NodoHoja*)raiz)->agregar(clave,valor);
		if (resultado == 2){
			//Significa que la raiz esta en overflow
			partirNodoRaiz();
		}
	}else{
		//Si la raiz no es hoja agrego recursivamente
		resultado = this->agregarRecursivamente(raiz, clave, valor);
		if (resultado == 2){
			//Significa que la raiz esta en overflow
			partirNodoRaiz();
		}
    }
	this->raiz->persistir(this->archivo);
	return resultado;
}


void ArbolBMas::partirNodoRaiz(){

	//Decide cual metodo hay que utilizar
	if (raiz->getNivel() == 0){
		partirNodoRaizHoja();
    } else {
    	partirNodoRaizInterno();
    }
}

void ArbolBMas::partirNodoRaizHoja(){

	/*
	 * Parte el nodo raiz hoja y persiste lo necesario.
	 * Deja el ex nodo raiz hoja como hijo izquierdo,
	 * un nuevo nodo interno como raiz,
	 * un nuevo nodo hoja derecho como hijo derecho.
	 *
	 * Deja solamente el nodo raiz cargado en memoria.
	 * Persiste los hijos.
	 */

	//Crea un nodo derecho
	NodoHoja * nodoDerecho = new NodoHoja(this->archivo);

	//Setea el siguiente del izquierdo como el derecho
	((NodoHoja*)this->raiz)->setSiguiente(nodoDerecho->getNumeroDeBloque());

	//Copio la referencia
	Nodo* nodoIzquierdo = this->raiz;
	//El nodo raiz ahora va a ser un nuevo nodo interno:
	this->raiz = new NodoInterno(this->archivo);
	//setea la referencia a izq de la raiz al nodo izq

	balancearADerecha((NodoHoja*)nodoIzquierdo,nodoDerecho, ((NodoInterno*)this->raiz));
	((NodoInterno*)this->raiz)->setReferenciaAIzq(nodoIzquierdo);
	((NodoInterno*)this->raiz)->setReferenciaADer(nodoDerecho);
	((NodoInterno*)this->raiz)->incrementarNivel();
	//persisto los nodos hijos, la raiz se persiste en otra funcion
	nodoIzquierdo->persistir(archivo);
	nodoDerecho->persistir(archivo);
	delete nodoIzquierdo;
	delete nodoDerecho;

}

void ArbolBMas::partirNodoRaizInterno() {

	unsigned int numeroDeBloqueAux;

	NodoInterno* nodoIzq = dynamic_cast<NodoInterno*>(this->raiz);
	NodoInterno* nodoDer = dynamic_cast<NodoInterno*>(new NodoInterno(this->archivo));
	//Creo un nuevo NodoInterno para la nueva raiz
	NodoInterno* nueavaRaiz = new NodoInterno(this->archivo);

	//La raiz sera un nivel mas alto a lo que era
	nueavaRaiz->setNivel((nodoIzq->getNivel()) + 1);
	//El nuevo hermano derecho tendra el mismo nivel que el izquierdo (ex raiz)
	nodoDer->setNivel(nodoIzq->getNivel());

	//Reemplazo el numero de bloque de la antigua raiz con el nuevo asignado a la creacion de la raiz
	//De esta manera la raiz tendra siempre como numero de bloque el valor 0
	numeroDeBloqueAux = nueavaRaiz->getNumeroDeBloque();
	nueavaRaiz->setNumeroDeBloque(nodoIzq->getNumeroDeBloque());
	nodoIzq->setNumeroDeBloque(numeroDeBloqueAux);

	//El nodo izquierdo se parte a la mitad dandole la primera clave de esa particion a la raiz yn
	//el resto a su hermano, luego setea como ultimo hijo de la raiz al nodo derecho
	nodoIzq->PartirNodoADerecha(nodoDer, nueavaRaiz);
	this->raiz = nueavaRaiz;

	//Persiste los hijos, la raiz se persiste en otro lado
	nodoIzq->persistir(archivo);
	nodoDer->persistir(archivo);
	delete nodoIzq;
	delete nodoDer;
}

void ArbolBMas::balancearADerecha(NodoHoja * nodoIzq, NodoHoja* nodoDer, NodoInterno * nodoPadre){

	Clave claveMedio = nodoIzq->getClaveDelMedio();
	list<RegistroArbol*> *mitadDerechaNodoIzq = nodoIzq->getMitadDerecha();

	nodoDer->agregar(mitadDerechaNodoIzq);

	nodoPadre->agregarClave(claveMedio);
	//Cambio el numero de bloque del izq al padre.
	unsigned int indiceNodoIzq = nodoIzq->getNumeroDeBloque();
	unsigned int indiceNodoDer = nodoPadre->getNumeroDeBloque();
	//intercambio
	nodoIzq->setNumeroDeBloque(indiceNodoDer);
	nodoPadre->setNumeroDeBloque(indiceNodoIzq);

}

int ArbolBMas::borrarValor(Clave clave, string valor){

	int resultado = 0;

	//Estoy en una hoja, borro directamente
	if (this->raiz->getNivel() == 0) {
		//Se borra directamente y no chequeo undeflow
		resultado = ((NodoHoja*)this->raiz)->baja(clave, valor);
		if (resultado == 3)
			//Raiz en undeflow pero no importa
			resultado = 2;
	}else{
		//Si la raiz no es hoja borro recursivamente
		resultado = this->borrarRecursivamente(this->raiz, clave, valor);
		//Hubo subflujo en la raiz
		if (resultado == 3){
		if((this->raiz)->estaVacio()){
				//Intercambio al hijo por el izquierdo de la raiz
				unsigned int hijoUnico = ((NodoInterno*)this->raiz)->getUltimoNodo();
				Nodo * nuevaRaiz = Nodo::cargar(this->archivo, hijoUnico);
				delete this->raiz;
				this->raiz = nuevaRaiz;
				this->archivo->borrar(nuevaRaiz->getNumeroDeBloque());
				nuevaRaiz->setNumeroDeBloque(0);
			}
		}
	}

	this->raiz->persistir(this->archivo);
	return resultado;
}

int ArbolBMas::agregarRecursivamente (Nodo* nodoActual, Clave clave, string valor){

	//Recorro los nodos hasta llegar a la hoja a donde se agrega el dato
	int estado = 0;
	if (nodoActual->getNivel() == 0){
		//Si es un nodo hoja
		int retorno = ((NodoHoja*)nodoActual)->agregar(clave,valor);
		if (retorno != 2)
			((NodoHoja*)nodoActual)->persistir(archivo);
		return retorno;
    }else{
    	//Es nodo interno
    	//Obtengo su indice
    	int indiceNodoInterno = ((NodoInterno*)nodoActual)->buscarClave(clave);
    	unsigned int indiceArchivo = 0;
    	if (indiceNodoInterno == -1){
    		//Si el indice en el nodo interno es -1 significa que la clave es mayor a todo lo que tiene el nodo
    	 	indiceArchivo = ((NodoInterno*)nodoActual)->getUltimoNodo();
    	} else {
    		//Si no es -1 obtengo el nodo con el indice que tengo
    		indiceArchivo = ((NodoInterno*)nodoActual)->buscarNodo(indiceNodoInterno);
    	}
    	//Hidrato el nodo
    	Nodo* nodoNuevo = Nodo::cargar(this->archivo,indiceArchivo);
    	estado = agregarRecursivamente(nodoNuevo, clave, valor);
    	if (estado == 2){
    		//Si estado es 2, hay overflow me fijo si hay que partir un nodo hoja o
    		//un nodo interno
    		if(nodoNuevo->getNivel() == 0){
    			return partirNodoHoja((NodoInterno*)nodoActual,(NodoHoja*)nodoNuevo);
    		}else{
    			return partirNodoInterno((NodoInterno*)nodoActual,(NodoInterno*)nodoNuevo);
    		}
    	}
    }

	return estado;
}


int ArbolBMas::partirNodoHoja(NodoInterno* nodoPadre, NodoHoja* nodoHijo){

	//Parte el nodo hoja, creando un nuevo nodo hoja y mandando la clave correspondiente
	//al nodo padre. Persiste a los nodos hijos, el nodo padre solo si no queda en overflow
	//Si queda el padre en overflow devuelve 2, sino 0

	//El nodo hijo tiene overflow
	//Recupero la clave del medio para mandarsela al nodo padre
	Clave clave = nodoHijo->getClaveDelMedio();
	NodoHoja * nuevoNodo = new NodoHoja(this->archivo);
	//Agrego la clave y la referencia al nuevo nodo en el padre
	nodoPadre->agregarReferencia(clave , nuevoNodo->getNumeroDeBloque());
	//Agrego la mitad derecha del nodo hijo al nuevo nodo
	list<RegistroArbol*> * mitadDerecha = nodoHijo->getMitadDerecha();
	list<RegistroArbol*>::iterator it;
	for (it = mitadDerecha->begin(); it != mitadDerecha->end(); it++){
		nuevoNodo->agregar(*it);
    }
	//Seteo la referencia del viejo hijo al nuevo y del nuevo al siguiente
	//del viejo
	unsigned int viejoSiguiente = nodoHijo->getSiguiente();
	nodoHijo->setSiguiente(nuevoNodo->getNumeroDeBloque());
	nuevoNodo->setSiguiente(viejoSiguiente);

	//Persisto los hijos y los borro de memoria.
	nodoHijo->persistir(this->archivo);
	nuevoNodo->persistir(this->archivo);
	delete nuevoNodo;
	delete nodoHijo;
	if (nodoPadre->hayOverflow()){
		return 2;
	} else {
		nodoPadre->persistir(this->archivo);
		return 0;
    }
}


int ArbolBMas::partirNodoInterno(NodoInterno* nodoPadre, NodoInterno* nodoHijo){

	//Parte el nodo interno, creando un nuevo nodo interno y mandando la clave correspondiente
	//al nodo padre. Persiste a los nodos hijos, el nodo padre solo si no queda en overflow
	//Si queda el padre en overflow devuelve 2, sino 0

	//El nodo hijo tiene overflow
	//Recupero la clave del medio para mandarsela al nodo padre
	Clave clave = nodoHijo->getClaveDelMedio();
	NodoInterno * nodoNuevo = new NodoInterno(this->archivo);
	//Agrego la clave y la referencia al nuevo nodo en el padre
	nodoPadre->agregarReferencia(clave,nodoNuevo->getNumeroDeBloque());
	nodoHijo->borrarClave(clave);
	//Agrego la mitad derecha del nodo hijo al nuevo nodo
	list<Clave> * mitadDerechaClaves = nodoHijo->getMitadDerechaClaves();

	list<unsigned int> * mitadDerechaHijos = nodoHijo->getMitadDerechaHijos();

	list<Clave>::iterator it;
	for (it = mitadDerechaClaves->begin(); it != mitadDerechaClaves->end(); it++){
		nodoNuevo->agregarClave(*it);
    }
	list<unsigned int>::iterator it0;
	for (it0 = mitadDerechaHijos->begin(); it0 != mitadDerechaHijos->end(); it0++){
		nodoNuevo->agregarHijoAlFinal(*it0);
    }
	nodoNuevo->setNivel(nodoHijo->getNivel());

	//Persisto los hijos y los borro de memoria.
	nodoHijo->persistir(this->archivo);
	nodoNuevo->persistir(this->archivo);
	nodoPadre->persistir(this->archivo);
	delete nodoNuevo;
	delete nodoHijo;
	if (nodoPadre->hayOverflow()){
		return 2;
	} else {
		return 0;
    }
}

int ArbolBMas::borrarRecursivamente(Nodo* nodoActual, Clave clave, string valor){

	int estado = 0;
	//Si es un nodo hoja hay que dar de baja desde ahi
	if (nodoActual->getNivel() == 0){
		int retorno = ((NodoHoja*)nodoActual)->baja(clave, valor);
		((NodoHoja*)nodoActual)->persistir(this->archivo);
		return retorno;
    }else{
    	//Si es un nodo interno obtengo su indice
    	int indiceNodoInt = ((NodoInterno*)nodoActual)->buscarClave(clave);
    	unsigned int indiceArchivo = 0;
    	if (indiceNodoInt == -1){
    		//Si el indice en el nodo interno es -1
    		//la clave es la mayor de todas
    		indiceArchivo = ((NodoInterno*)nodoActual)->getUltimoNodo();
    	} else {
    		//Si no es -1 obtengo el nodo con el indice que tengo
    		indiceArchivo = ((NodoInterno*)nodoActual)->buscarNodo(indiceNodoInt);
    	}
    	//Hidrato el nodo
    	Nodo* nodoNuevo = Nodo::cargar(this->archivo,indiceArchivo);
    	estado = borrarRecursivamente (nodoNuevo, clave, valor);
    	if (estado == 3){
    		//Si el estado es 3, hay underflow
    		//Me fijo si es en un nodo interno o un hoja
    		if (nodoNuevo->getNivel() == 0){
    			//Nivel 0, es una hoja
    			return this->balancearNodoHoja((NodoInterno*)nodoActual, (NodoHoja*)nodoNuevo);
    		}else{
    			//Es un nodo interno
    			return this->balancearNodoInterno((NodoInterno*)nodoActual,(NodoInterno*) nodoNuevo);
    		}
    	}
     }

	return estado;
}


int ArbolBMas::balancearNodoHoja(NodoInterno* nodoActual, NodoHoja* nodoNuevo){

	//Nodo nuevo esta en underflow
	//Tengo que conseguir otro nodo para balancearlo o hacer merge
	if (nodoNuevo->getSiguiente()){
		//Si tiene siguiente (a derecha) lo cargo y listo
		Nodo* nodoABalancear = Nodo::cargar(this->archivo,nodoNuevo->getSiguiente());
		// Tengo 2 casos, si el nodoBalanceo se encuentra en capacidad minima
		// hay que hacer un merge
		if (((NodoHoja*)nodoABalancear)->capacidadMinima()){
			//Hay que hacer un merge
			return mergeNodoHojaDerecho(nodoActual,nodoNuevo,(NodoHoja*)nodoABalancear);
		}else{
			//Balanceo
			return equilibrarNodoHojaDerecho(nodoActual,nodoNuevo,(NodoHoja*)nodoABalancear);
		}
    }else{
    	// Si no tiene nodo a derecha busco al hijo izquierdo del padre
    	unsigned int indiceNodoBalanceado =  nodoActual->getNodoAnteriorA(nodoNuevo->getNumeroDeBloque());
    	Nodo* nodoABalancear = Nodo::cargar(this->archivo,indiceNodoBalanceado);
    	if (((NodoHoja*)nodoABalancear)->capacidadMinima()){
    		//Hay que hacer un merge
    		return mergeNodoHojaIzquierdo(nodoActual,nodoNuevo,(NodoHoja*)nodoABalancear);
    	}else{
    		return equilibrarNodoHojaIzquierdo(nodoActual,nodoNuevo,(NodoHoja*)nodoABalancear);
    	}
    }
}

int ArbolBMas::balancearNodoInterno(NodoInterno* nodoActual,NodoInterno* nodoNuevo){

	//Nodo nuevo tiene underflow
	//Tengo que conseguir un nodo para balancearlo o hacer merge
	Clave clave = nodoNuevo->getClaveDelMedio();
	int indicador = nodoActual->buscarClave(clave);
	if (indicador != -1){
		//Si la clave del medio del hijo no es mayor a todas,
		//tengo un nodo siguiente a la derecha
		indicador ++ ; //Porque queda en la direccion a la izq
		Nodo* nodoDerecho = Nodo::cargar(this->archivo,nodoActual->buscarNodo(indicador));

		if (((NodoInterno*)nodoDerecho)->capacidadMinima()){
			//Si el siguiente a la derecha tiene capacidad minima, hago merge
			return mergeNodoInternoDerecho(nodoActual, nodoNuevo, (NodoInterno*)nodoDerecho);
		}else{
			//Sino hago balanceo
			return equilibrarNodoInternoDerecho(nodoActual, nodoNuevo, (NodoInterno*)nodoDerecho);
		}
	}else{
		//No tengo un nodo siguiente a la derecha, sino a la izquierda
		int cantClaves = nodoActual->getCantidadDeClaves();//Este es el indice del ultimo
		cantClaves--; //Indice del anteultimo
		Nodo* nodoIzquierdo = Nodo::cargar(this->archivo, nodoActual->buscarNodo(cantClaves));

		if (((NodoInterno*)nodoIzquierdo)->capacidadMinima()){
			return mergeNodoInternoIzquierdo (nodoActual, (NodoInterno*)nodoIzquierdo, nodoNuevo);
		}else{
			return equilibrarNodoInternoIzquierdo (nodoActual, (NodoInterno*)nodoIzquierdo, nodoNuevo);
		}
	}
}

int ArbolBMas::mergeNodoHojaDerecho(NodoInterno* nodoPadre, NodoHoja* nodoIzq, NodoHoja* nodoDer){

	/* Hace un merge al nodo de la derecha,
	*  devuelve 1 si el padre queda en overflow, 0 si todo esta bien.
	*/
	list<RegistroArbol*> *listaDer = nodoDer->getElementos();
	//Agrego todo al nodo de la izq
	nodoIzq->agregar(listaDer);
	nodoIzq->setSiguiente(nodoDer->getSiguiente());
	//Busco la primer clave de la lista derecha,
	//que la tengo que borrar en el nodo padre
	Clave clave = (listaDer->front())->getClave();
	Clave claveAEliminar= clave;
	//Busco el indice del nodo correspondiente a la clave
	unsigned int i = nodoDer->getNumeroDeBloque();
	//La borro
	nodoPadre->borrarClave(claveAEliminar);
	nodoPadre->borrarReferencia(i);
	//Borro el de la derecha
	this->archivo->borrar(nodoDer->getNumeroDeBloque());

	nodoIzq->persistir(this->archivo);
	nodoPadre->persistir(this->archivo);
	delete nodoDer;
		if (nodoPadre->hayUnderflow()) return 3;
	return 0;
}

int ArbolBMas::equilibrarNodoHojaDerecho(NodoInterno* nodoPadre,NodoHoja* nodoIzq, NodoHoja* nodoDer){

	//Saco de los nodos sus respectivas listas y las 'concateno'
	list<RegistroArbol*> * listaTotal = nodoIzq->getElementos();
	list<RegistroArbol*> * listaDer = nodoDer->getElementos();
	//Consigo la clave del nodo de la derecha porque despues me va a servir
	Clave clave = listaDer->front()->getClaveEntera();
	listaTotal->splice(listaTotal->end(), *listaDer);
	//Me fijo cual es el tamaño total de los registros de la lista
	int tamanioTotal = 0;
	int sumador = 0;
	list<RegistroArbol*>::iterator it;
	for (it = listaTotal->begin(); it != listaTotal->end(); it++){
		tamanioTotal += (*it)->cantidadDeBytesOcupados();
    }
	//Recorro la lista con un sumador
	//Cuando el sumador llega a la mitad del tam total parto la lista
	//Le doy una a un nodo y la otra al otro
	int contador = 0;
	for (it = listaTotal->begin(); it != listaTotal->end(); it++){
		sumador += (*it)->cantidadDeBytesOcupados();
		if (sumador <= tamanioTotal/2){
			//Agregar a la lista
			contador++;
			listaDer->push_back(*it);
		}else {break;}
    }
	for (int i = 0; i <contador; i++){
		listaTotal->pop_front();
    }
	// Quedo la lista derecha con las claves mas pequeñas
	// y la lista total con las mas grandes
	nodoIzq->setElementos(listaDer);
	nodoDer->setElementos(listaTotal);
	//Borro la ex clave del nodo de la derecha
	nodoPadre->borrarClave(clave);
	//Agrego la clave del primer elemento de la lista del nodo de la derecha
	nodoPadre->agregarClave(nodoDer->getElementos()->front()->getClaveEntera());
	//Si antes no estaba en underflow ahora tampoco, porque tiene la misma cantidad de claves.
	nodoIzq->persistir(this->archivo);
	nodoPadre->persistir(this->archivo);
	nodoDer->persistir(this->archivo);
	return 0;
}

int ArbolBMas::mergeNodoHojaIzquierdo(NodoInterno* nodoPadre,NodoHoja* nodoDer,NodoHoja* nodoIzq){

	//El nodo de la derecha es el que esta en underflow, hace merge con el nodo de la izquierda
	//Devuelve 1 si el padre queda en over, 0 si todo esta bien
	//Saco la lista del nodo de la derecha
	list<RegistroArbol*> * listaDer = nodoDer->getElementos();
	//Agrego todo al nodo de la izq
	nodoIzq->agregar(listaDer);
	nodoIzq->setSiguiente(nodoDer->getSiguiente());
	//Busco la primer clave de la lista derecha, que la tengo que borrar en el nodo padre
	Clave clave = (listaDer->front())->getClaveEntera();
	Clave claveABorrar = clave;
	//Busco el indice del nodo correspondiente a la clave
	unsigned int i = nodoDer->getNumeroDeBloque();
	//La borro
	nodoPadre->borrarClave(claveABorrar);
	nodoPadre->borrarReferencia(i);
	//Borro el de la derecha
	this->archivo->borrar(nodoDer->getNumeroDeBloque());
	//Persisto
	nodoIzq->persistir(this->archivo);
	nodoPadre->persistir(this->archivo);
	delete nodoDer;
	if (nodoPadre->hayUnderflow()) return 3;
	return 0;

}

int ArbolBMas::equilibrarNodoHojaIzquierdo(NodoInterno* nodoPadre, NodoHoja* nodoDerecho, NodoHoja* nodoIzq){

	//Saco de los nodos sus respectivas listas y las 'concateno'
	list<RegistroArbol*> *listaTotal = nodoIzq->getElementos();
	list<RegistroArbol*> *listaDer = nodoDerecho->getElementos();
	//Consigo la clave del nodo de la derecha porque despues me va a servir
	Clave clave =  listaDer->front()->getClave();
	Clave claveABorrar = clave;
	listaTotal->splice(listaTotal->end(), *listaDer);
	//Me fijo cual es el tamaño total de los registros de la lista
	int tamanio_total=0;
	int sumador = 0;
	list<RegistroArbol*>::iterator it;
	for (it = listaTotal->begin(); it != listaTotal->end(); it++){
		tamanio_total += (*it)->cantidadDeBytesOcupados();
    }
	//Recorro la lista con un sumador
	//Cuando el sumador llega a la mitad del tam total parto la lista
	//Le doy una a un nodo y la otra al otro
	for (it = listaTotal->begin(); it != listaTotal->end(); it++){
		sumador += (*it)->cantidadDeBytesOcupados();
		if (sumador<= tamanio_total/2){
			//Agregar a la lista
			listaDer->push_back(*it);
			it = listaTotal->erase(it);
		}else {break;}
	}
	//Quedo la lista derecha con las claves mas pequeñas y la lista total con las mas grandes
	nodoIzq->setElementos(listaDer);
	nodoDerecho->setElementos(listaTotal);
	//Borro la ex clave del nodo de la derecha
	nodoPadre->borrarClave(claveABorrar);
	//Agrego la clave del primer elemento de la lista del nodo de la derecha
	nodoPadre->agregarClave(nodoDerecho->getElementos()->front()->getClaveEntera());
	//Si antes no estaba en underflow ahora tampoco, porque tiene la misma cantidad de claves.
	nodoIzq->persistir(this->archivo);
	nodoPadre->persistir(this->archivo);
	nodoDerecho->persistir(this->archivo);
	return 0;
}


int ArbolBMas::mergeNodoInternoDerecho(NodoInterno* nodoPadre, NodoInterno* nodoIzq, NodoInterno* nodoDer){

	//El nodo izquierdo tiene underflow.
	//Devuelve -1 si el padre queda en underflow
	list<Clave> clavesDerechas = nodoDer->getClaves();
	list<unsigned int> listaHijosDer = nodoDer->getHijos();
	//Voy a tener que bajar la clave que 'une' a los dos nodos en el padre, al hijo izq
	Clave primeraClave = clavesDerechas.front();
	int indiceClaveBaja = nodoPadre->buscarClave(primeraClave);
	indiceClaveBaja--; //porque quiero la anterior a la que es mas grande que la primer clave de la derecha
	Clave claveABajar;
	if (indiceClaveBaja == -2 ){//significa que buscar clave dio -1, osea que es la mayor de todas
		claveABajar = nodoPadre->getUltimaClave();
    }else if(indiceClaveBaja == -1){//Significa que buscar clave dio 0, osea q es la primera
    	claveABajar = nodoPadre->getPrimerClave();
    	}else{
    		claveABajar = nodoPadre->getClave(indiceClaveBaja);
    	}
        //Agrego al hijo izq la clave y la borro del padre
        nodoIzq->agregarClave(claveABajar);
        nodoPadre->borrarClave(claveABajar);
        nodoPadre->borrarReferencia(nodoDer->getNumeroDeBloque());
        //Como sabemos que las claves se ingresan ordenadas, podemos agregar los hijos al final simplemente
        nodoIzq->agregarClaves(clavesDerechas);
        nodoIzq->agregarHijos(listaHijosDer);
        //Borro el nodo derecho
        this->archivo->borrar(nodoDer->getNumeroDeBloque());
        delete nodoDer;
        //Persisto los otros
        nodoPadre->persistir(this->archivo);
        nodoIzq->persistir(this->archivo);
        if (nodoPadre->hayUnderflow()) return 3;
        return 0;
}

int ArbolBMas::equilibrarNodoInternoDerecho(NodoInterno* nodoPadre, NodoInterno* nodoIzq, NodoInterno* nodoDer){

	//Se llama cuando el nodo izquierdo tiene underflow
	list<Clave> clavesIzquierdas = nodoIzq->getClaves();
	list<Clave> clavesDerechas = nodoDer->getClaves();
	list<unsigned int> hijosIzquierdos = nodoIzq->getHijos();
	list<unsigned int> hijosDerechos = nodoDer->getHijos();
	//Creo los tamanios totales y de la izq
	int tamanioTotal = 0;
	int tamanioActualIzq = 0;
	list<Clave>::iterator it;
	for (it = clavesIzquierdas.begin(); it != clavesIzquierdas.end(); it++){
		tamanioTotal += (*it).getTamanioClave();
		tamanioActualIzq += (*it).getTamanioClave();
    }
	for (it = clavesDerechas.begin(); it != clavesDerechas.end(); it++){
		tamanioTotal += (*it).getTamanioClave();
    }
	tamanioTotal += sizeof(unsigned int) * (hijosIzquierdos.size() + hijosDerechos.size());
	tamanioActualIzq += sizeof(unsigned int)* hijosIzquierdos.size();

	//Bajar la clave que corresponda del padre

	int indiceClaveABajar = nodoPadre->buscarClave(clavesDerechas.front());
	Clave claveABajar;
	if (indiceClaveABajar == -1)//Si es mayor a todo...
		claveABajar = nodoPadre->getUltimaClave();
    else
    	claveABajar = nodoPadre->getClave(indiceClaveABajar);

	//Deja la union de las listas en un estado valido
	//Agrega clave a bajar y la referencia de la derecha a la izquierda
	//Borra estas 2 en sus originales
	unsigned int ref = hijosDerechos.front();
	hijosIzquierdos.push_back(ref);
	tamanioActualIzq += sizeof(unsigned int);
	hijosDerechos.pop_front();
	clavesIzquierdas.push_back(claveABajar);
	tamanioActualIzq += claveABajar.getTamanioClave();
	nodoPadre->borrarClave(claveABajar);

	// Hasta aca quedo un hueco en el nodo padre.
	// Falta una clave que se agrega despues.

	while(tamanioActualIzq < tamanioTotal/2){
		//pasar de derecha a izquierda el par ref-clave
		Clave clave = clavesDerechas.front();
		unsigned int ref = hijosDerechos.front();
		tamanioActualIzq += sizeof(unsigned int);
		tamanioActualIzq += clave.getTamanioClave();
		clavesIzquierdas.push_back(clave);
		hijosIzquierdos.push_back(ref);
		clavesDerechas.pop_front();
		hijosDerechos.pop_front();
	}

	// Cuando termino subo al padre la clave de la izquierda
	// de la lista derecha y la popeo
	Clave claveASubir = clavesDerechas.front();
	nodoPadre->agregarClave(claveASubir);
	clavesDerechas.pop_front();

	nodoIzq->setClaves(clavesIzquierdas);
	nodoIzq->setHijos(hijosIzquierdos);
	nodoDer->setClaves(clavesDerechas);
	nodoDer->setHijos(hijosDerechos);

	nodoPadre->persistir(this->archivo);
	nodoDer->persistir(this->archivo);
	nodoIzq->persistir(this->archivo);

	if (nodoPadre->hayUnderflow()) return -1;//No deberia suceder
        return 0;
}

int ArbolBMas::mergeNodoInternoIzquierdo(NodoInterno* nodoPadre, NodoInterno* nodoIzq, NodoInterno* nodoDer){

	//El nodo derecho tiene underflow
	//Devuelve -1 si el padre queda en underflow
	list<Clave> lista_derecha_claves = nodoDer->getClaves();
	list<unsigned int> lista_derecha_hijos = nodoDer->getHijos();
	//Voy a tener que bajar la clave que 'une' a los dos nodos en el padre, al hijo izq
	//Como es la clave de mas a la derecha (en este caso) agarro la ultima clave
	Clave claveABajar = nodoPadre->getClave(nodoPadre->getCantidadDeClaves() - 1);
	//Agrego al hijo izq la clave y la borro del padre
	nodoIzq->agregarClave(claveABajar);
	nodoPadre->borrarClave(claveABajar);
	nodoPadre->borrarReferencia(nodoDer->getNumeroDeBloque());
	//Como sabemos que las claves se ingresan ordenadas, podemos agregar los hijos al final simplemente
	nodoIzq->agregarClaves(lista_derecha_claves);
	nodoIzq->agregarHijos(lista_derecha_hijos);
	//Borro el nodo derecho
	this->archivo->borrar(nodoDer->getNumeroDeBloque());
	delete nodoDer;
	//Persisto los otros
	nodoPadre->persistir(this->archivo);
	nodoIzq->persistir(this->archivo);
	if (nodoPadre->hayUnderflow()) return 3;

	return 0;
}

int ArbolBMas::equilibrarNodoInternoIzquierdo(NodoInterno* nodoPadre, NodoInterno* nodoIzq, NodoInterno* nodoDer){

	//Se llama cuando nodo derecho tiene underflow
	list<Clave> clavesIzquierdas = nodoIzq->getClaves();
	list<Clave> clavesDerechas = nodoDer->getClaves();
	list<unsigned int> hijosIzquierdos = nodoIzq->getHijos();
	list<unsigned int> hijosDerechos = nodoDer->getHijos();
	//Creo los tamanios totales y de la izq
	int tamanioTotal = 0;
	int tamanioActualDer = 0;
	list<Clave>::iterator it;
	for (it = clavesDerechas.begin(); it != clavesDerechas.end(); it++){
		tamanioTotal += (*it).getTamanioClave();
		tamanioActualDer += (*it).getTamanioClave();
    }
	for (it = clavesIzquierdas.begin(); it != clavesIzquierdas.end(); it++){
		tamanioTotal += (*it).getTamanioClave();
    }
	tamanioTotal += sizeof(unsigned int) * (hijosIzquierdos.size() + hijosDerechos.size());
	tamanioActualDer += sizeof(unsigned int)* hijosDerechos.size();

	//Bajar la clave que corresponda del padre
	int indiceClaveABajar = nodoPadre->buscarClave(clavesIzquierdas.back());
	Clave claveABajar;
	if (indiceClaveABajar == -1)//Si es mayor a todo...
		claveABajar = nodoPadre->getUltimaClave();
	else
		claveABajar = nodoPadre->getClave(indiceClaveABajar);

	unsigned int ref = hijosIzquierdos.back();
	hijosDerechos.push_front(ref);
	tamanioActualDer += sizeof(unsigned int);
	hijosIzquierdos.pop_back();
	clavesDerechas.push_front(claveABajar);
	tamanioActualDer += claveABajar.getTamanioClave();
	nodoPadre->borrarClave(claveABajar);

	while(tamanioActualDer <= tamanioTotal/2){
		//pasar de izq a der el par clave-ref
		Clave clave = clavesIzquierdas.back();
		unsigned int ref = hijosIzquierdos.back();
		tamanioActualDer += sizeof(unsigned int);
		tamanioActualDer += clave.getTamanioClave();
		clavesDerechas.push_front(clave);
		hijosDerechos.push_front(ref);
		clavesIzquierdas.pop_back();
		hijosIzquierdos.pop_back();
    }

	//cuando termino subo al padre la clave de la derecha de la lista izquierda y la popeo
	Clave claveASubir = clavesIzquierdas.back();
	nodoPadre->agregarClave(claveASubir);
	clavesIzquierdas.pop_back();

	//No es necesario agregarlos porque el metodo se manejo siempre por referencia. No
	//son listas nuevas
	nodoIzq->setClaves(clavesIzquierdas);
	nodoIzq->setHijos(hijosIzquierdos);
	nodoDer->setClaves(clavesDerechas);
	nodoDer->setHijos(hijosDerechos);

	nodoPadre->persistir(this->archivo);
	nodoDer->persistir(this->archivo);
	nodoIzq->persistir(this->archivo);
	if (nodoPadre->hayUnderflow()) return -1;
        return 0;
}

