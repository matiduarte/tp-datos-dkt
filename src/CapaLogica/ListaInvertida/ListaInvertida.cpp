#include "ListaInvertida.h"

ListaInvertida::ListaInvertida(const string& rutaArchLista){
	archivo = new ArchivoRegVariable(rutaArchLista);
}

unsigned int ListaInvertida::insertar(string idPalabra, string valor){
	RegistroLista* regLista = new RegistroLista(idPalabra, valor);
	return archivo->escribir((char*)regLista->serializar().c_str());
}
void ListaInvertida::modificar(int posLista, string valor){
		//TODO: falta metodo de Marian
}

string ListaInvertida::obtener(unsigned int posicionLista){
	string buffer = string(this->archivo->leer(posicionLista));
	RegistroLista* reg = new RegistroLista(buffer);
	return reg->getValor();
}

ListaInvertida::~ListaInvertida(){
//	if (tabla){
//		delete tabla;
//	}
//	if (interpreteNodo){
//		delete interpreteNodo;
//	}
}
