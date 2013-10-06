#include "Hash.h"

Hash::Hash(const string& pathArchTabla, const string& pathArchNodos, const int& tamElemTabla, const int& tamElemNodo, bool sobreEscribir){
	//creo la tabla y el interpreter...
	tabla = new HashTable(pathArchTabla, sobreEscribir, tamElemTabla);
	interpreteNodo = new HashNodeInterpreter(pathArchNodos, sobreEscribir, tamElemNodo);
}

bool Hash::insertar(const Contexto & c,const TablaDeFrecuencias& valor){
	//Levantar el numero de nodo de la tabla
	int numeroNodo = (tabla->getElement(c));
	//Insertar en el nodo con el numero
	try {
		interpreteNodo->insertarElemento(c, valor, numeroNodo);
	} catch (ExceptionOverflowNodo e) {
		resolverOverflowNodo(e, c);
	} catch (ExceptionElementoKeyYaIngresado e){
		return false;
	}

	return true;
}

bool Hash::modificarRegistro(const Contexto& clave, const TablaDeFrecuencias& info) {
	int numeroNodo = (tabla->getElement(clave));
	try {
		interpreteNodo->modificarElemento(clave, info, numeroNodo);
	} catch (ExceptionElementoNoEncontrado e){
		return false;
	} catch (ExceptionOverflowNodo e){
		resolverOverflowNodo(e, clave);
	}
	return true;
}

TablaDeFrecuencias* Hash::buscar ( const Contexto& c ) {
	int numeroNodo = (tabla->getElement(c));
	TablaDeFrecuencias* retorno=NULL;
	try{
		retorno =interpreteNodo->buscarElemento((ContextoHash)c, numeroNodo);
	} catch (ExceptionElementoNoEncontrado e){
	}

	return retorno;
}

bool Hash::eliminar(const Registro & r){
	ContextoHash key=r.getContexto();
	int numeroNodo = (tabla->getElement(key));
	try {
		interpreteNodo->eliminarElemento(key, numeroNodo);
	} catch (ExceptionElementoNoEncontrado e){
		return false;
	} catch (ExceptionUnderflowNodo e){
		//si el nodo quedo vacio, le aviso a la tabla que hay un nodo libre para que intente liberarlo...
		int tamDispersion = e.getTamDispersion();
		int actualizarNodo = tabla->nodoLibre(tamDispersion, key);
		//si libero el nodo...
		if (actualizarNodo != -1){
			interpreteNodo->actualizarNodoLiberado(actualizarNodo);
		}
	}
	return true;
}

void Hash::resolverOverflowNodo(const ExceptionOverflowNodo e, const ContextoHash& key){
	list<Registro*> lista = e.getElementos();
	list<pair<Registro*,bool> > listaPair;

	int numeroNodoNuevo = tabla->nodoNuevo(e.getTamDispersion(), key);
	list<Registro*>::iterator it = lista.begin();
	while(it != lista.end()){
		int nodoActual = tabla->getElement((*it)->getContexto());
		bool nuevo = false;
		if (nodoActual == numeroNodoNuevo){
			nuevo = true;
		}
		pair<Registro*,bool> elemPair((*it), nuevo);
		listaPair.push_back(elemPair);
		it++;
	}
	try {
		interpreteNodo->redispersarNodo(listaPair,e.getPosBloque(), numeroNodoNuevo);
	} catch (ExceptionOverflowNodo e) {
		resolverOverflowNodo(e, key);
	}
}

string Hash::imprimirNodo(int numeroNodo){
	return interpreteNodo->imprimirNodo(numeroNodo);
}

ostream& operator << (ostream& o, Hash& hash){
	o << *(hash.tabla);
	o << "\n";
	o << *(hash.interpreteNodo);
	return o;
}

bool Hash::actualizar (std::vector<Registro>& registros ) throw ( ActualizarEstructuraExc ){
	for ( unsigned int i = 0; i< registros.size(); i++ ){
		TablaDeFrecuencias* tablaParam = registros[i].getInfo2();
		Contexto contextoParam = registros[i].getContexto2();

		//intengo modificar el registro...
		if (!this->modificarRegistro ( contextoParam, *tablaParam)){
			//si no existe, lo inserto...
			if (!this->insertar(contextoParam, *tablaParam)){
				//si no pude insertar, lanzo exception...
				throw ActualizarEstructuraExc("Tamanio del bloque insuficiente");
			}
		}
		delete tablaParam;
	}
	return true;
}

Hash::~Hash(){
	delete tabla;
	delete interpreteNodo;
}
