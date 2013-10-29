#include "HashNodeInterpreter.h"

HashNodeInterpreter::HashNodeInterpreter(const string& ruta)
: Salvable(ruta){
	//escribo un bloque vacio...
	if (this->getCantidadDeBloques () == 0){
		nodo = new HashNode(1);
		escribirBloque(nodo->serializar(), 0);
		delete nodo;
	}
	nodo=NULL;
}

void HashNodeInterpreter::insertarElemento(const HashClave& key, const string& valor, const int numeroNodo){
    levantarNodo(numeroNodo);
    try {
		nodo->insertarElemento(key, valor);
        escribirBloque(nodo->serializar(), numeroNodo);
    } catch (ExceptionElementoKeyYaIngresado& e){
    	liberarNodo();
    	throw ExceptionElementoKeyYaIngresado("Clave " + key.serializar() + " ya fue ingresada.");
    } catch (ExcepcionOverflowTamBloque& e){
        throw ExceptionOverflowNodo(numeroNodo, nodo->getTamDispersion(), nodo->getElementos());
    }

    liberarNodo();
}

void HashNodeInterpreter::modificarElemento(const HashClave& key, const string& nvoValor, const int numeroNodo){
	levantarNodo(numeroNodo);
	try {
		nodo->modificarElemento(key,nvoValor);
		escribirBloque(nodo->serializar(),numeroNodo);
	} catch (ExceptionElementoNoEncontrado& e) {
		liberarNodo();
		throw e;
	} catch (ExcepcionOverflowTamBloque& e) {
		throw ExceptionOverflowNodo(numeroNodo, nodo->getTamDispersion(), nodo->getElementos());
	}
	liberarNodo();
}

void HashNodeInterpreter::eliminarElemento(const HashClave& key, const int numeroNodo){
	levantarNodo(numeroNodo);
	try {
		nodo->eliminarElemento(key);
	} catch (ExceptionElementoNoEncontrado& e) {
		liberarNodo();
		throw e;
	} catch (ExceptionUnderflowNodo& e) {
		//Guardo el nodo utilizado, borre un elemento (me quedo el nodo vacio)
		escribirBloque(nodo->serializar(),numeroNodo);
		liberarNodo();
		throw e;
	}
	//Guardo el nodo utilizado, borre un elemento
	escribirBloque(nodo->serializar(),numeroNodo);
	liberarNodo();
}

string HashNodeInterpreter::buscarElemento(const HashClave& key, const int numeroNodo){
	levantarNodo(numeroNodo);
	string retorno;
	try {
		//Busco la Key
		retorno=nodo->buscarElemento(key);
	} catch (ExceptionElementoNoEncontrado& e) {
		liberarNodo();
		throw e;
	}

	liberarNodo();
	return retorno;
}

void HashNodeInterpreter::redispersarNodo(const list<pair<HashElement,bool> >&  bulkInsert,const int numNodoViejo,const int numNodoNuevo){
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
		for (list<pair<HashElement,bool> >::const_iterator it = bulkInsert.begin(); it!=bulkInsert.end(); it++) {
			// Va al nuevo?
			if (it->second) {
				insertoNuevo=true;
				nodoNuevo.insertarElemento(it->first.getClave(),it->first.getValor());
			}else{
				insertoViejo=true;
				nodo->insertarElemento(it->first.getClave(),it->first.getValor());
			}
		}
	}catch (ExceptionElementoKeyYaIngresado& e) {
		//Si pasa esto que rompa todo.
		throw e;
	}

	//XOR de Nuevo y viejo
	bool hayOverflow=insertoNuevo ^ insertoViejo;

	if (hayOverflow) {
		//Cual rompio ? --> el unico al que se inserto todo..
		if(insertoNuevo){
			//Escribo el nodo viejo Vacio
			escribirBloque(nodo->serializar(),numNodoViejo);
			liberarNodo();
			//Instancio en mem el HashNodo vacio
			nodo= new HashNode(nuevoTamDispersion);
		}else{
			escribirBloque(nodoNuevo.serializar(),numNodoNuevo);
		}
		//Redismensionar
		list<HashElement> elementos;
		for (list<pair<HashElement,bool> >::const_iterator it = bulkInsert.begin(); it!=bulkInsert.end(); it++) {
			elementos.push_back(it->first);
		}
		throw ExceptionOverflowNodo(insertoNuevo? numNodoNuevo: numNodoViejo,nuevoTamDispersion,elementos);

	}else{
		//Va todo divino , no hay overflow
		escribirBloque(nodoNuevo.serializar(),numNodoNuevo);
		escribirBloque(nodo->serializar(),numNodoViejo);
		liberarNodo();
	}
}

void HashNodeInterpreter::actualizarNodoLiberado(int numeroNodo){
	levantarNodo(numeroNodo);
	nodo->liberoYResolvi();
	escribirBloque(nodo->serializar(), numeroNodo);
	liberarNodo();
}

void HashNodeInterpreter::levantarNodo(const int & numeroNodo){
    string buf;
    leerBloque(numeroNodo, &buf);
    nodo = new HashNode(buf);
}

void HashNodeInterpreter::liberarNodo(){
	delete nodo;
	nodo=NULL;
}

HashNodeInterpreter::~HashNodeInterpreter(){
	if (nodo) {
		delete nodo;
	}
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
