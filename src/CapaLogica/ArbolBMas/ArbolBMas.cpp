/*
 * ArbolBMas.cpp
 *
 *  Created on: 18/10/2013
 *      Author: matias
 */

#include "ArbolBMas.h"

ArbolBMas::ArbolBMas(){

}

ArbolBMas::ArbolBMas(string nombre) {
	// TODO Auto-generated constructor stub
	this->archivo = new ArchivoBloque(nombre);
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
		valor = ((NodoHoja*)raiz)->buscarClave(clave);
	}else{
		valor = this->buscarClaveRecursivamente(clave, this->raiz);
	}

	return valor;
}


void ArbolBMas::mostrarArbol(){

	if (raiz->getNivel()==0){
		((NodoHoja *)raiz)->mostrar();
		return;
    }else{
    	((NodoInterno * )raiz)->mostrar();
    	list<unsigned int> hijos = ((NodoInterno*)raiz)->getHijos();
    	list<unsigned int>::iterator it;
    	for (it = hijos.begin(); it != hijos.end(); it++){
    		Nodo* nodo = Nodo::cargar(this->archivo,*it);
    		mostrarArbolRecursivamente(nodo);
    	}
    }
}

void ArbolBMas::mostrarArbolRecursivamente(Nodo* nodo){

	if (nodo->getNivel()==0){
		//Es hoja muestro directamente
		((NodoHoja*)nodo)->mostrar();
		delete nodo;
    }else{
    	((NodoInterno*)nodo)->mostrar();
    	list<unsigned int> hijos = ((NodoInterno*)nodo)->getHijos();
    	list<unsigned int>::iterator it;
    	for (it = hijos.begin(); it != hijos.end(); it++){
    		Nodo* nodoHijo = Nodo::cargar(this->archivo,*it);
    		mostrarArbolRecursivamente(nodoHijo);
    	}
    	delete nodo;
    }

}
string ArbolBMas::buscarClaveRecursivamente(Clave clave, Nodo* nodoActual){


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
    	valor =  buscarClaveRecursivamente(clave,nodoNuevoHidratado);
    }

    return valor;
}

void ArbolBMas::agregarValor(Clave clave, string valor){

	//Agrega una clave con su respectivo valor
	// 0 --> Nodo no se actualizo
	// 1 --> Nodo se actualizo correctamente
	// 2 --> Raiz en Overflow
	// 3 --> El conjunto clave - valor ya existe
	int resultado = 0;
	if (this->raiz->getNivel() == 0){
		//Es una hoja, agrego directamente
		resultado = ((NodoHoja*)raiz)->agregarValor(clave,valor);
		if (resultado == 2){
			this->solucionarOverflowRaiz();
		}
	}else{
		//Si no es una hoja, agrego recursivamente
		resultado = this->agregarRecursivamente(raiz, clave, valor);
		if (resultado == 2){
			this->solucionarOverflowRaiz();
		}
    }
	this->raiz->persistir(this->archivo);
	return;
}


void ArbolBMas::solucionarOverflowRaiz(){

	//Decide cual metodo hay que utilizar
	if (raiz->getNivel() == 0){
		this->overflowRaizHoja();
    } else {
    	this->overflowRaizInterna();
    }
}

void ArbolBMas::overflowRaizHoja(){

	/*
	 *  Parte a la raiz dejando su contenido como hijo izquierdo.
	 *  Crea un nuevo nodo interno como raiz y
	 *  un nuevo nodo hoja como hijo derecho
	 *
	 */

	NodoHoja * nodoDerecho = new NodoHoja(this->archivo);

	//El contenido de la raiz pasa al hijo izq
	NodoHoja* nodoIzquierdo = ((NodoHoja*)this->raiz);

	//Seteo ref al siguiente
	nodoIzquierdo->setSiguiente(nodoDerecho->getNumeroDeBloque());

	this->raiz = new NodoInterno(this->archivo);

	this->balancearADerecha(nodoIzquierdo , nodoDerecho, ((NodoInterno*)this->raiz));

	//Seteo ref a hijos
	((NodoInterno*)this->raiz)->setReferenciaAIzq(nodoIzquierdo);
	((NodoInterno*)this->raiz)->setReferenciaADer(nodoDerecho);
	((NodoInterno*)this->raiz)->incrementarNivel();

	//Persisto
	nodoIzquierdo->persistir(archivo);
	nodoDerecho->persistir(archivo);

	delete nodoIzquierdo;
	delete nodoDerecho;

}

void ArbolBMas::overflowRaizInterna() {

	NodoInterno* nodoIzq = ((NodoInterno*)this->raiz);
	NodoInterno* nodoDer = new NodoInterno(this->archivo);
	//Creo un nuevo nodo interno para la raiz
	NodoInterno* nueavaRaiz = new NodoInterno(this->archivo);

	//La raiz crece un nivel
	nueavaRaiz->setNivel((nodoIzq->getNivel()) + 1);
	//Hermano derecho mismo nivel que el izquierdo
	nodoDer->setNivel(nodoIzq->getNivel());

	// Dado que la raiz siempre se encuentra en el bloque cero
	// Seteo la nuevaRaiz a esa posicion
	// Y la vieja al numero de bloque con el que se creo la nueva raiz
	unsigned int numeroDeBloqueAux = nueavaRaiz->getNumeroDeBloque();
	nueavaRaiz->setNumeroDeBloque(nodoIzq->getNumeroDeBloque());
	nodoIzq->setNumeroDeBloque(numeroDeBloqueAux);

	nodoIzq->PartirNodoADerecha(nodoDer, nueavaRaiz);
	this->raiz = nueavaRaiz;

	//Persiste los hijos
	nodoIzq->persistir(archivo);
	nodoDer->persistir(archivo);
	delete nodoIzq;
	delete nodoDer;
}

void ArbolBMas::balancearADerecha(NodoHoja * nodoIzq, NodoHoja* nodoDer, NodoInterno * nodoPadre){


	Clave claveMedio = nodoIzq->getClaveDelMedio();
	list<RegistroArbol*> *mitadDerechaNodoIzq = nodoIzq->getMitadDerecha();

	nodoDer->agregarLista(mitadDerechaNodoIzq);
	nodoPadre->agregarClave(claveMedio);

	unsigned int indiceNodoIzq = nodoIzq->getNumeroDeBloque();
	unsigned int indiceNodoDer = nodoPadre->getNumeroDeBloque();

	nodoIzq->setNumeroDeBloque(indiceNodoDer);
	nodoPadre->setNumeroDeBloque(indiceNodoIzq);

}

void ArbolBMas::borrarValor(Clave clave, string valor){

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
	return;
}

int ArbolBMas::agregarRecursivamente (Nodo* nodoActual, Clave clave, string valor){

	//Recorro los nodos hasta llegar a la hoja a donde se agrega el dato
	int estado = 0;
	if (nodoActual->getNivel() == 0){
		//Si es un nodo hoja
		int retorno = ((NodoHoja*)nodoActual)->agregarValor(clave,valor);
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
    			return this->overflowNodoHoja((NodoInterno*)nodoActual,(NodoHoja*)nodoNuevo);
    		}else{
    			return this->overflowNodoInterno((NodoInterno*)nodoActual,(NodoInterno*)nodoNuevo);
    		}
    	}
    }

	return estado;
}


int ArbolBMas::overflowNodoHoja(NodoInterno* nodoPadre, NodoHoja* nodoHijo){

	/* Parte la hoja, creando una nueva y subiendo la clave puente
	 * hacia el padre. El hijo esta con sobreflujo.
	 * Retorno:
	 * 0--> No hay overflow
	 * 2--> Hay overflow
	 */

	Clave clave = nodoHijo->getClaveDelMedio();
	NodoHoja * nuevoNodo = new NodoHoja(this->archivo);
	//Se agregan la clave y la referencias al padre
	nodoPadre->agregarReferencia(clave , nuevoNodo->getNumeroDeBloque());
	//Agrego la mitad derecha del nodo hijo al nuevo nodo
	list<RegistroArbol*> * mitadDerecha = nodoHijo->getMitadDerecha();
	list<RegistroArbol*>::iterator it;
	for (it = mitadDerecha->begin(); it != mitadDerecha->end(); it++){
		nuevoNodo->agregarRegistro(*it);
    }
	//Seteo las referencias
	unsigned int siguiente = nodoHijo->getSiguiente();
	nodoHijo->setSiguiente(nuevoNodo->getNumeroDeBloque());
	nuevoNodo->setSiguiente(siguiente);

	//Persisto los hijos
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


int ArbolBMas::overflowNodoInterno(NodoInterno* nodoPadre, NodoInterno* nodoHijo){

	/*
	 * Parte el nodo interno, creando uno nuevo y subiendo la clave puente
	*  hacia el padre. El hijo esta con sobreflujo
	*  Retorno:
	*  0 --> No hay overflow
	*  2 --> Hay overflow
	*/

	Clave clave = nodoHijo->getClaveDelMedio();
	NodoInterno * nodoNuevo = new NodoInterno(this->archivo);
	//Agrego clave y referencias al padre
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

	//Persisto los hijos
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
	//Si es hoja, borro directamente
	if (nodoActual->getNivel() == 0){
		int retorno = ((NodoHoja*)nodoActual)->baja(clave, valor);
		((NodoHoja*)nodoActual)->persistir(this->archivo);
		return retorno;
    }else{
    	//Si no es hoja, busco su indice
    	int indiceNodoInt = ((NodoInterno*)nodoActual)->buscarClave(clave);
    	unsigned int indiceArchivo = 0;
    	//Clave mayor a todas
    	if (indiceNodoInt == -1){
    		indiceArchivo = ((NodoInterno*)nodoActual)->getUltimoNodo();
    	} else {
    		indiceArchivo = ((NodoInterno*)nodoActual)->buscarNodo(indiceNodoInt);
    	}
    	Nodo* nodoNuevo = Nodo::cargar(this->archivo,indiceArchivo);
    	estado = borrarRecursivamente (nodoNuevo, clave, valor);
    	//Hay underflow
    	if (estado == 3){
    		if (nodoNuevo->getNivel() == 0){
    			//Es hoja
    			return this->solucionarUnderflowHoja((NodoInterno*)nodoActual, (NodoHoja*)nodoNuevo);
    		}else{
    			//Es un nodo interno
    			return this->solucionarUnderflowInterno((NodoInterno*)nodoActual,(NodoInterno*) nodoNuevo);
    		}
    	}
     }

	return estado;
}


int ArbolBMas::solucionarUnderflowHoja(NodoInterno* nodoPadre, NodoHoja* nodoUnderflow){

	// Busco un nodo para equilibrar o hacer merge
	if (nodoUnderflow->getSiguiente()){
		//Si tiene hermando derecho lo cargo directamente
		Nodo* nodoParaBalancear = Nodo::cargar(this->archivo,nodoUnderflow->getSiguiente());
		//Si tiene capacidad minima tengo que hacer merge
		if (((NodoHoja*)nodoParaBalancear)->capacidadMinima()){
			return this->mergeNodoHoja(nodoPadre,(NodoHoja*)nodoParaBalancear,nodoUnderflow);
		}else{
			//Si no balanceo
			return this->balancearNodoHoja(nodoPadre,nodoUnderflow,(NodoHoja*)nodoParaBalancear);
		}
    }else{
    	//No tiene hermano derecho, busco el izquierdo
    	unsigned int indiceNodoBuscado =  nodoPadre->getNodoAnteriorA(nodoUnderflow->getNumeroDeBloque());
    	Nodo* nodoParaBalancear = Nodo::cargar(this->archivo,indiceNodoBuscado);
    	if (((NodoHoja*)nodoParaBalancear)->capacidadMinima()){
    		return this->mergeNodoHoja(nodoPadre,nodoUnderflow,(NodoHoja*)nodoParaBalancear);
    	}else{
    		return this->balancearNodoHoja(nodoPadre,(NodoHoja*)nodoParaBalancear,nodoUnderflow);
    	}
    }
}

int ArbolBMas::solucionarUnderflowInterno(NodoInterno* nodoActual,NodoInterno* nodoUnderflow){

	// Busco un nodo para equilibrar o hacer merge
	Clave clave = nodoUnderflow->getClaveDelMedio();
	int indicador = nodoActual->buscarClave(clave);
	if (indicador != -1){
		//Si la clave no es la mayor de todas, existe hermano derecho
		indicador ++ ; //+1 para que agarre al derecho
		Nodo* nodoParaBalancearDer = Nodo::cargar(this->archivo,nodoActual->buscarNodo(indicador));

		if (((NodoInterno*)nodoParaBalancearDer)->capacidadMinima()){
			//Si tiene capacidad minima tengo que hacer merge
			return mergeNodoInternoDerecho(nodoActual, nodoUnderflow, (NodoInterno*)nodoParaBalancearDer);
		}else{
			//No esta con capacidad minima, balanceo
			return balancearNodoInternoDerecho(nodoActual, nodoUnderflow, (NodoInterno*)nodoParaBalancearDer);
		}
	}else{
		//No tengo hermano derecho, voy al izquierdo
		int cantClaves = nodoActual->getCantidadDeClaves();
		cantClaves--; //Indice del izquierdo
		Nodo* nodoParaBalancearIzq = Nodo::cargar(this->archivo, nodoActual->buscarNodo(cantClaves));

		if (((NodoInterno*)nodoParaBalancearIzq)->capacidadMinima()){
			//Hay capacidad minima, tengo que hacer merge
			return mergeNodoInternoIzquierdo (nodoActual, (NodoInterno*)nodoParaBalancearIzq, nodoUnderflow);
		}else{
			//Balanceo
			return balancearNodoInternoIzquierdo (nodoActual, (NodoInterno*)nodoParaBalancearIzq, nodoUnderflow);
		}
	}
}

int ArbolBMas::mergeNodoHoja(NodoInterno* nodoPadre, NodoHoja* nodoUnderflow, NodoHoja* nodoParaBalancear){


	list<RegistroArbol*> *listaElementos = nodoUnderflow->getElementos();

	nodoParaBalancear->agregarLista(listaElementos);

	unsigned int siguiente = nodoUnderflow->getSiguiente();
	nodoParaBalancear->setSiguiente(siguiente);

	//Busco la primer clave de la lista derecha,
	//que la tengo que borrar en el nodo padre
	Clave clave = (listaElementos->front())->getClave();
	Clave claveAEliminar= clave;
	//Busco el indice del nodo correspondiente a la clave
	unsigned int i = nodoUnderflow->getNumeroDeBloque();
	//La borro
	nodoPadre->borrarClave(claveAEliminar);
	nodoPadre->borrarReferencia(i);
	//Borro el que estaba en underflow
	unsigned int numeroDeBloque = nodoUnderflow->getNumeroDeBloque();

	this->archivo->borrar(numeroDeBloque);

	nodoParaBalancear->persistir(this->archivo);
	nodoPadre->persistir(this->archivo);

	delete nodoUnderflow;
		if (nodoPadre->hayUnderflow()) return 3;
	return 0;
}

int ArbolBMas::balancearNodoHoja(NodoInterno* nodoPadre,NodoHoja* nodoIzq, NodoHoja* nodoDer){

	//Consigo los elementos de los dos hijos
	list<RegistroArbol*> * listaIzq = nodoIzq->getElementos();
	list<RegistroArbol*> * listaDer = nodoDer->getElementos();

	Clave clave = listaDer->front()->getClaveEntera();
	//Agrego todo al hijo izquierdo
	listaIzq->splice(listaIzq->end(), *listaDer);
	//Busco el tamanio total de la lista
	int tamanioTotal = 0;
	int sumador = 0;
	list<RegistroArbol*>::iterator it;
	for (it = listaIzq->begin(); it != listaIzq->end(); it++){
		tamanioTotal += (*it)->cantidadDeBytesOcupados();
    }

	//Recorro la lista a la mitad y la agrego a la lista derecha
	int contador = 0;
	for (it = listaIzq->begin(); it != listaIzq->end(); it++){
		sumador += (*it)->cantidadDeBytesOcupados();
		if (sumador <= tamanioTotal/2){
			//Agregar a la lista
			contador++;
			listaDer->push_back(*it);
		}else {break;}
    }

	//Elimino la mitad del comienzo
	for (int i = 0; i <contador; i++){
		listaIzq->pop_front();
    }
	// La lista derecha tiene las claves mas chicas
	nodoIzq->setElementos(listaDer);
	nodoDer->setElementos(listaIzq);
	//Elimino la clave del padre
	nodoPadre->borrarClave(clave);
	//Agrego la nueva clave al padre
	nodoPadre->agregarClave(nodoDer->getElementos()->front()->getClaveEntera());


	nodoIzq->persistir(this->archivo);
	nodoPadre->persistir(this->archivo);
	nodoDer->persistir(this->archivo);

	return 0;
}



int ArbolBMas::mergeNodoInternoDerecho(NodoInterno* nodoPadre, NodoInterno* nodoUnderflow, NodoInterno* nodoDer){


	list<Clave> clavesDerechas = nodoDer->getClaves();
	list<unsigned int> listaHijosDer = nodoDer->getHijos();
	//Bajo la clave puente entre el padre y el hijo izquierdo
	Clave primeraClave = clavesDerechas.front();
	int indiceClaveAEliminar = nodoPadre->buscarClave(primeraClave);
	//Quiero la anterior a la que es mas grande que la primer clave de la derecha
	indiceClaveAEliminar--;
	Clave claveABajar;
	if (indiceClaveAEliminar == -2 ){
		//Clave mayor a todas
		claveABajar = nodoPadre->getUltimaClave();
    }else if(indiceClaveAEliminar == -1){
    	//Es la primera
    	claveABajar = nodoPadre->getPrimerClave();
    	}else{
    		claveABajar = nodoPadre->getClave(indiceClaveAEliminar);
    	}

	//Agrego al hijo izq la clave y la borro del padre
	nodoUnderflow->agregarClave(claveABajar);
	nodoPadre->borrarClave(claveABajar);
	nodoPadre->borrarReferencia(nodoDer->getNumeroDeBloque());
	//Agrego claves y ref a hijos
	nodoUnderflow->agregarClaves(clavesDerechas);
	nodoUnderflow->agregarHijos(listaHijosDer);
	//Borro el nodo derecho
	this->archivo->borrar(nodoDer->getNumeroDeBloque());
	delete nodoDer;
	//Persisto
	nodoPadre->persistir(this->archivo);
	nodoUnderflow->persistir(this->archivo);

	if (nodoPadre->hayUnderflow()) return 3;

	return 0;
}

int ArbolBMas::balancearNodoInternoDerecho(NodoInterno* nodoPadre, NodoInterno* nodoIzq, NodoInterno* nodoDer){

	//Nodo izquierdo en underflow
	list<Clave> clavesIzquierdas = nodoIzq->getClaves();
	list<Clave> clavesDerechas = nodoDer->getClaves();
	list<unsigned int> hijosIzquierdos = nodoIzq->getHijos();
	list<unsigned int> hijosDerechos = nodoDer->getHijos();

	int tamanioTotal = 0;
	int tamanioActualIzq = 0;
	list<Clave>::iterator it;
	//Calculo el tamanio de las claves izquierdas y el de ambas (total)
	for (it = clavesIzquierdas.begin(); it != clavesIzquierdas.end(); it++){
		tamanioTotal += (*it).getTamanioClave();
		tamanioActualIzq += (*it).getTamanioClave();
    }
	for (it = clavesDerechas.begin(); it != clavesDerechas.end(); it++){
		tamanioTotal += (*it).getTamanioClave();
    }
	//Suma de los hijos izquierdos y derechos
	tamanioTotal += sizeof(unsigned int) * (hijosIzquierdos.size() + hijosDerechos.size());
	//Solo los hijos izquierdos
	tamanioActualIzq += sizeof(unsigned int)* hijosIzquierdos.size();

	//Baja la clave que corresponda del padre
	int indiceClaveABajar = nodoPadre->buscarClave(clavesDerechas.front());
	Clave claveABajar;
	if (indiceClaveABajar == -1)
		//Es la mayor
		claveABajar = nodoPadre->getUltimaClave();
    else
    	claveABajar = nodoPadre->getClave(indiceClaveABajar);


	//Agrega la clave a eliminar y la referencia de la derecha a la izquierda
	unsigned int ref = hijosDerechos.front();
	hijosIzquierdos.push_back(ref);
	tamanioActualIzq += sizeof(unsigned int);
	hijosDerechos.pop_front();
	clavesIzquierdas.push_back(claveABajar);
	tamanioActualIzq += claveABajar.getTamanioClave();
	nodoPadre->borrarClave(claveABajar);


	//Paso de derecha a izquierda la clave y la referencia
	while(tamanioActualIzq < tamanioTotal/2){
		Clave clave = clavesDerechas.front();
		unsigned int ref = hijosDerechos.front();
		tamanioActualIzq += sizeof(unsigned int);
		tamanioActualIzq += clave.getTamanioClave();
		clavesIzquierdas.push_back(clave);
		hijosIzquierdos.push_back(ref);
		clavesDerechas.pop_front();
		hijosDerechos.pop_front();
	}

	//Subo al padre la clave de la izquierda de la lista derecha y la elimino
	Clave claveASubir = clavesDerechas.front();
	nodoPadre->agregarClave(claveASubir);
	clavesDerechas.pop_front();

	nodoIzq->setClaves(clavesIzquierdas);
	nodoIzq->setHijos(hijosIzquierdos);
	nodoDer->setClaves(clavesDerechas);
	nodoDer->setHijos(hijosDerechos);

	//Persisto
	nodoPadre->persistir(this->archivo);
	nodoDer->persistir(this->archivo);
	nodoIzq->persistir(this->archivo);

	if (nodoPadre->hayUnderflow()) return -1;
        return 0;
}

int ArbolBMas::mergeNodoInternoIzquierdo(NodoInterno* nodoPadre, NodoInterno* nodoIzq, NodoInterno* nodoUnderflow){


	list<Clave> clavesDerechas = nodoUnderflow->getClaves();
	list<unsigned int> hijosDerechos = nodoUnderflow->getHijos();
	//Bajo la clave puente entre el padre y el hijo izquierdo
	//Como es la clave de mas a la derecha agarro la ultima
	Clave claveAEliminar = nodoPadre->getClave(nodoPadre->getCantidadDeClaves() - 1);
	//Agrego al hijo izq la clave y la borro del padre
	nodoIzq->agregarClave(claveAEliminar);
	nodoPadre->borrarClave(claveAEliminar);
	nodoPadre->borrarReferencia(nodoUnderflow->getNumeroDeBloque());
	//Agrego clave y ref a los hijos
	nodoIzq->agregarClaves(clavesDerechas);
	nodoIzq->agregarHijos(hijosDerechos);
	//Borro el nodo derecho
	this->archivo->borrar(nodoUnderflow->getNumeroDeBloque());
	delete nodoUnderflow;
	//Persisto
	nodoPadre->persistir(this->archivo);
	nodoIzq->persistir(this->archivo);

	//Si hay underflow devuelvo 3
	if (nodoPadre->hayUnderflow()) return 3;

	return 0;
}

int ArbolBMas::balancearNodoInternoIzquierdo(NodoInterno* nodoPadre, NodoInterno* nodoIzq, NodoInterno* nodoDer){

	//Nodo derecho en underflow
	list<Clave> clavesIzquierdas = nodoIzq->getClaves();
	list<Clave> clavesDerechas = nodoDer->getClaves();
	list<unsigned int> hijosIzquierdos = nodoIzq->getHijos();
	list<unsigned int> hijosDerechos = nodoDer->getHijos();

	int tamanioTotal = 0;
	int tamanioActualDer = 0;
	list<Clave>::iterator it;
	//Calculo tamanio total y el de la derecha
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
	if (indiceClaveABajar == -1)
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

	//Paso de derecha a izquierda la clave y la referencia
	while(tamanioActualDer <= tamanioTotal/2){
		Clave clave = clavesIzquierdas.back();
		unsigned int ref = hijosIzquierdos.back();
		tamanioActualDer += sizeof(unsigned int);
		tamanioActualDer += clave.getTamanioClave();
		clavesDerechas.push_front(clave);
		hijosDerechos.push_front(ref);
		clavesIzquierdas.pop_back();
		hijosIzquierdos.pop_back();
    }

	//Subo al padre la clave de la derecha de la lista izquierda y la elimino
	Clave claveASubir = clavesIzquierdas.back();
	nodoPadre->agregarClave(claveASubir);
	clavesIzquierdas.pop_back();


	nodoIzq->setClaves(clavesIzquierdas);
	nodoIzq->setHijos(hijosIzquierdos);
	nodoDer->setClaves(clavesDerechas);
	nodoDer->setHijos(hijosDerechos);

	//Persisto
	nodoPadre->persistir(this->archivo);
	nodoDer->persistir(this->archivo);
	nodoIzq->persistir(this->archivo);

	if (nodoPadre->hayUnderflow()) return -1;
        return 0;
}


/*
 * devuelve una lista con los elementos con la clave igual a "clave".
 */
list<string> * ArbolBMas::elementosConIgualClave(Clave clave){

	list<string>* listaAdevolver = new list<string>();
	list<string>* listaAux = new list<string>();

//si la raiz es hoja,resuelve; sino recorre todas las hojas.
	if( this->raiz->getNivel() == 0){
		return ((NodoHoja*)this->raiz)->buscarYlistar(clave);
	}
		else{
			Nodo *nodoAux = new Nodo();
			nodoAux = (this->raiz);
			while (nodoAux->getNivel() != 0){
				nodoAux = Nodo::cargar(this->archivo,((NodoInterno*)nodoAux)->getHijos().front());
			}

			while(((NodoHoja*)nodoAux)->getSiguiente() != 0){
				listaAux = ((NodoHoja*)nodoAux)->buscarYlistar(clave);
				listaAdevolver->splice(listaAdevolver->end(),*listaAux);
			nodoAux = Nodo::cargar(this->archivo,((NodoHoja*)nodoAux)->getSiguiente());
			}
			listaAux = ((NodoHoja*)nodoAux)->buscarYlistar(clave);
			listaAdevolver->splice(listaAdevolver->end(),*listaAux);
			return listaAdevolver;
		}

}

