#include "RegistroLista.h"

RegistroLista::RegistroLista(const string aDeserealizar){	
	this->desSerializar(aDeserealizar);
}

RegistroLista::RegistroLista(string idPalabra, string valor): Serializable(";"){
	this->idPalabra = idPalabra;
	this->valor = valor;
}

string RegistroLista::serializar()const {
	string retorno;

	retorno = this->idPalabra ;
	retorno+= separadorElem;
	retorno+= this->valor;
	
	return retorno;
}

void RegistroLista::desSerializar(const string elemSerializado){
	string token;
	istringstream iss(elemSerializado);

	getline(iss, token, separadorElem.c_str()[0]);
	this->idPalabra = token;
	getline(iss, token, separadorElem.c_str()[0]);
	this->valor = token;
}

string RegistroLista::getValor(){
	return this->valor;
}

RegistroLista::~RegistroLista(){
}
