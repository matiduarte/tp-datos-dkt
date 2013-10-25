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
	short unsigned int cantidadDeBloque = this->archivo->getCantidadBloques();
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
	string claveAEliminar= clave.getClave();
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
	Clave clave = listaDer->front()->getClave();
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
	//Quedo la lista derecha con las claves mas pequeñas y la lista total con las mas grandes
	nodoIzq->setElementos(listaDer);
	nodoDer->setElementos(listaTotal);
	//Borro la ex clave del nodo de la derecha
	nodoPadre->borrarClave(clave.getClave());
	//Agrego la clave del primer elemento de la lista del nodo de la derecha
	nodoPadre->agregarClave(nodoDer->getElementos()->front()->getClaveEntera());
	//Si antes no estaba en underflow ahora tampoco, porque tiene la misma cantidad de claves.
	nodoIzq->persistir(this->archivo);
	nodoPadre->persistir(this->archivo);
	nodoDer->persistir(this->archivo);
	return 0;
}

int ArbolBMas::mergeNodoHojaIzquierdo(NodoInterno* nodoPadre,NodoHoja* nodoDer,NodoHoja* nodoIzq){

	return 0;
}

int ArbolBMas::equilibrarNodoHojaIzquierdo(NodoInterno* nodoPadre,NodoHoja* nodoDer,NodoHoja* nodoIzq){

	return 0;
}


int ArbolBMas::mergeNodoInternoDerecho(NodoInterno* nodoPadre, NodoInterno* nodoIzquierdo, NodoInterno* nodoDerecho){
	return 0;
}

int ArbolBMas::equilibrarNodoInternoDerecho(NodoInterno* nodoPadre, NodoInterno* nodoIzquierdo, NodoInterno* nodoDerecho){
	return 0;
}

int ArbolBMas::mergeNodoInternoIzquierdo(NodoInterno* nodoPadre, NodoInterno* nodoIzquierdo, NodoInterno* nodoDerecho){

	return 0;
}

int ArbolBMas::equilibrarNodoInternoIzquierdo(NodoInterno* nodoPadre, NodoInterno* nodoIzquierdo, NodoInterno* nodoDerecho){

	return 0;
}
