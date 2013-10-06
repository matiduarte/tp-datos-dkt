/*
 * HashNodeInterpreter.cpp
 *
 *  Created on: 01/04/2010
 *      Author: gonzalo
 */

#include "HashNodeInterpreter.h"

HashNodeInterpreter::HashNodeInterpreter(const string& ruta, const bool& sobreEscribir, const short& tamElem)
: Salvable(ruta,sobreEscribir,tamElem){
	//escribo un bloque vacio...
	if (sobreEscribir){
		nodo = new HashNode(1);
		guardarNodo(nodo,0);
		//escribirBloque(nodo,0);
		delete nodo;
	}
	nodo=NULL;
}

void HashNodeInterpreter::insertarElemento(const ContextoHash& key, const TablaDeFrecuencias& valor, const int numeroNodo){
    levantarNodo(numeroNodo);
    try {
		nodo->insertarElemento(key, valor);
		guardarNodo(nodo, numeroNodo);
    } catch (ExceptionElementoKeyYaIngresado e){
    	liberarNodo();
    	throw ExceptionElementoKeyYaIngresado("La Clave ya fue ingresada.");
    } catch (ExceptionOverflowTamBloque e){
        throw ExceptionOverflowNodo(numeroNodo, nodo->getTamDispersion(), nodo->getElementos());
    }

    liberarNodo();
}

void HashNodeInterpreter::modificarElemento(const ContextoHash& key, const TablaDeFrecuencias& nvoValor, const int numeroNodo){
	levantarNodo(numeroNodo);
	try {
		nodo->modificarElemento(key,nvoValor);
		//escribirBloque(nodo,numeroNodo);
		guardarNodo(nodo, numeroNodo);
	} catch (ExceptionElementoNoEncontrado e) {
		liberarNodo();
		throw e;
	} catch (ExceptionOverflowTamBloque e) {
		throw ExceptionOverflowNodo(numeroNodo, nodo->getTamDispersion(), nodo->getElementos());
	}
	liberarNodo();
}

void HashNodeInterpreter::eliminarElemento(const ContextoHash& key, const int numeroNodo){
	levantarNodo(numeroNodo);
	try {
		nodo->eliminarElemento(key);
	} catch (ExceptionElementoNoEncontrado e) {
		liberarNodo();
		throw e;
	} catch (ExceptionUnderflowNodo e) {
		//Guardo el nodo utilizado, borre un elemento (me quedo el nodo vacio)
		//escribirBloque(nodo,numeroNodo);
		guardarNodo(nodo, numeroNodo);
		liberarNodo();
		throw e;
	}
	//Guardo el nodo utilizado, borre un elemento
	//escribirBloque(nodo,numeroNodo);
	guardarNodo(nodo, numeroNodo);
	liberarNodo();
}

TablaDeFrecuencias* HashNodeInterpreter::buscarElemento(const ContextoHash& key, const int numeroNodo){
	levantarNodo(numeroNodo);
	TablaDeFrecuencias* retorno = NULL;
	try {
		//Busco la Key
		retorno=nodo->buscarElemento(key);
	} catch (ExceptionElementoNoEncontrado e) {
		liberarNodo();
		throw e;
	}

	liberarNodo();
	return retorno;
}

void HashNodeInterpreter::redispersarNodo(const list<pair<Registro*,bool> >&  bulkInsert,const int numNodoViejo,const int numNodoNuevo){
	//Cambio el tam de dispersion
	nodo->desbordoYResolvi();
	int nuevoTamDispersion = nodo->getTamDispersion();
	//Nodo nuevo a guardar
	HashNode nodoNuevo(nuevoTamDispersion);
	//Vacio el nodo viejo que esta en memoria...
	nodo->vaciarElementos();

	//Inserto
	bool insertoNuevo=false;
	bool insertoViejo=false;

	try{
		for (list<pair<Registro*,bool> >::const_iterator it = bulkInsert.begin(); it!=bulkInsert.end(); it++) {
			// Va al nuevo?
			if (it->second) {
				insertoNuevo=true;
				nodoNuevo.insertarElemento(it->first->getContexto(),it->first->getInfo());
			}else{
				insertoViejo=true;
				nodo->insertarElemento(it->first->getContexto(),it->first->getInfo());
			}
		}
	}catch (ExceptionElementoKeyYaIngresado e) {
		//Si pasa esto que rompa todo.
		throw e;
	}

	//XOR de Nuevo y viejo
	bool hayOverflow=insertoNuevo ^ insertoViejo;

	if (hayOverflow) {
		//Cual rompio ? --> el unico al que se inserto todo..
		if(insertoNuevo){
			//Escribo el nodo viejo Vacio
			//escribirBloque(nodo,numNodoViejo);
			guardarNodo(nodo, numNodoViejo);
			liberarNodo();
			//Instancio en mem el HashNodo vacio
			nodo= new HashNode(nuevoTamDispersion);
		}else{
			//escribirBloque(&nodoNuevo,numNodoNuevo);
			guardarNodo(&nodoNuevo,numNodoNuevo);
		}
		//Redismensionar
		list<Registro*> elementos;
		for (list<pair<Registro*,bool> >::const_iterator it = bulkInsert.begin(); it!=bulkInsert.end(); it++) {
			elementos.push_back(it->first);
		}
		throw ExceptionOverflowNodo(insertoNuevo? numNodoNuevo: numNodoViejo,nuevoTamDispersion,elementos);

	}else{
		//Va to do divino , no hay overflow
		//escribirBloque(&nodoNuevo,numNodoNuevo);
		guardarNodo(&nodoNuevo,numNodoNuevo);
		//escribirBloque(nodo,numNodoViejo);
		guardarNodo(nodo, numNodoViejo);
		liberarNodo();
	}
}

void HashNodeInterpreter::actualizarNodoLiberado(int numeroNodo){
	levantarNodo(numeroNodo);
	nodo->liberoYResolvi();
	//escribirBloque(nodo, numeroNodo);
	guardarNodo(nodo, numeroNodo);
	liberarNodo();
}

void HashNodeInterpreter::levantarNodo(const int & numeroNodo){
	void* deserializar = leerBloque(numeroNodo);
	nodo = new HashNode(deserializar);
	delete[] (char*)deserializar;
}

void HashNodeInterpreter::liberarNodo(){
	if (nodo){
		delete nodo;
	}
	nodo=NULL;
}

void HashNodeInterpreter::guardarNodo(HashNode* nodo, int numeroNodo){
	unsigned int tam = nodo->getTamanio();
	char* buffer = new char[tam];
	memset(buffer,0,tam);
	nodo->serializar(buffer);
    escribirBloque(buffer, tam, numeroNodo);
    if (buffer){
    	delete[] buffer;
    	buffer = NULL;
    }
}

HashNodeInterpreter::~HashNodeInterpreter(){
	if (nodo) {
		delete nodo;
	}
}

string HashNodeInterpreter::imprimirNodo(int numeroNodo){
	levantarNodo(numeroNodo);
	stringstream s;
	if (nodo){
		s << (*nodo) << endl;
		liberarNodo();
	}
	return s.str();

}

ostream& operator<< (ostream& os,HashNodeInterpreter& ptrObj){
	//Para cada Nodo del archivoBloque
	os << "Archivo: \n";
	for (unsigned int bloque = 0; bloque < ptrObj.getCantidadDeBloques(); ++bloque) {
		ptrObj.levantarNodo(bloque);
		os << "\tNumero Nodo: " << bloque << "\n";
		os << *ptrObj.nodo << endl;
		ptrObj.liberarNodo();
	}
	return os;
}
