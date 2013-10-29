#include "Hash.h"

Hash::Hash(const string& pathArchTabla, const string& pathArchNodos){
	//creo la tabla y el interpreter...
	tabla = new HashTable(pathArchTabla);
	interpreteNodo = new HashNodeInterpreter(pathArchNodos);
}

void Hash::insertarElemento(const string& clave,const string& valor){
	HashClave key(clave,false);
	//Levantar el numero de nodo de la tabla
	int numeroNodo = (tabla->getElement(key));
	//Insertar en el nodo con el numero
	try {
		interpreteNodo->insertarElemento(key, valor, numeroNodo);
	} catch (ExceptionOverflowNodo e) {
		resolverOverflowNodo(e, key);
	} catch (ExceptionElementoKeyYaIngresado e){
		throw e;
	}
}

void Hash::modificarElemento(const string& clave, const string& valor){
	HashClave key(clave,false);
	int numeroNodo = (tabla->getElement(key));
	try {
		interpreteNodo->modificarElemento(key, valor, numeroNodo);
	} catch (ExceptionElementoNoEncontrado e){
		throw e;
	} catch (ExceptionOverflowNodo e){
		resolverOverflowNodo(e, key);
	}
}

string Hash::buscarElemento(const string& clave){
	HashClave key(clave,false);
	int numeroNodo = (tabla->getElement(key));
	try{
		return interpreteNodo->buscarElemento(key, numeroNodo);
	} catch (ExceptionElementoNoEncontrado e){
		throw e;
	}
}

void Hash::elminarElemento(const string& clave){
	HashClave key(clave,false);
	int numeroNodo = (tabla->getElement(key));
	try {
		interpreteNodo->eliminarElemento(key, numeroNodo);
	} catch (ExceptionElementoNoEncontrado e){
		throw e;
	} catch (ExceptionUnderflowNodo e){
		//si el nodo quedo vacio, le aviso a la tabla que hay un nodo libre para que intente liberarlo...
		int tamDispersion = e.getTamDispersion();
		int actualizarNodo = tabla->nodoLibre(tamDispersion, key);
		//si libero el nodo...
		if (actualizarNodo != -1){
			interpreteNodo->actualizarNodoLiberado(actualizarNodo);
		}
	}
}

void Hash::resolverOverflowNodo(const ExceptionOverflowNodo e, const HashClave& key){
	list<HashElement> lista = e.getElementos();
	list<pair<HashElement,bool> > listaPair;

	int numeroNodoNuevo = tabla->nodoNuevo(e.getTamDispersion(), key);
	list<HashElement>::iterator it = lista.begin();
	while(it != lista.end()){
		int nodoActual = tabla->getElement(it->getClave());
		bool nuevo = false;
		if (nodoActual == numeroNodoNuevo){
			nuevo = true;
		}
		pair<HashElement,bool> elemPair(*it, nuevo);
		listaPair.push_back(elemPair);
		it++;
	}
	try {
		interpreteNodo->redispersarNodo(listaPair,e.getPosBloque(), numeroNodoNuevo);
	} catch (ExceptionOverflowNodo e) {
		resolverOverflowNodo(e, key);
	}
}

ostream& operator << (ostream& o, Hash& hash){
	o << *(hash.tabla);
	o << "\n";
	o << *(hash.interpreteNodo);
	return o;
}

Hash::~Hash(){
	if (tabla){
		delete tabla;
	}
	if (interpreteNodo){
		delete interpreteNodo;
	}
}
