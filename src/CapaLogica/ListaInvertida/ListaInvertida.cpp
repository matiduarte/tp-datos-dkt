#include "ListaInvertida.h"

ListaInvertida::ListaInvertida(const string& rutaArchLista){
	archivo = new ArchivoRegVariable(rutaArchLista);
}

unsigned int ListaInvertida::insertar(string idPalabra, string valor){
	RegistroLista* regLista = new RegistroLista(idPalabra, valor);
	return archivo->escribir((char*)regLista->serializar().c_str());
}
unsigned int ListaInvertida::modificar(int posLista, string valor){
	string buffer = string(this->archivo->sacar(posLista));
	RegistroLista* reg = new RegistroLista(buffer);
	return this->insertar(reg->getIdElemento(),valor);
}

string ListaInvertida::obtener(unsigned int posicionLista,int* nuevaPosicion){
	string buffer = string(this->archivo->sacar(posicionLista));
	RegistroLista* reg = new RegistroLista(buffer);
	posicionLista = this->insertar(reg->getIdElemento(),reg->getValor());
	return reg->getValor();
}

void ListaInvertida::borrar(unsigned int posicionLista){
	this->archivo->borrar(posicionLista);
}

ListaInvertida::~ListaInvertida(){
//	if (tabla){
//		delete tabla;
//	}
//	if (interpreteNodo){
//		delete interpreteNodo;
//	}
}
