#include "RegistroLista.h"

RegistroLista::RegistroLista(const string aDeserealizar): Serializable(";"){	
	this->desSerializar(aDeserealizar);
}

RegistroLista::RegistroLista(string idElemento, string valor): Serializable(";"){
	this->idElemento = idElemento;
	this->valor = valor;
}

string RegistroLista::serializar()const {
	string retorno;

	retorno = this->idElemento ;
	retorno+= separadorElem;
	retorno+= this->valor;
	retorno+='\0';
	return retorno;
}

void RegistroLista::desSerializar(const string elemSerializado){
	string token;
	istringstream iss(elemSerializado);

	getline(iss, token, separadorElem.c_str()[0]);
	this->idElemento = token;
	getline(iss, token, separadorElem.c_str()[0]);
	this->valor = token;
}

string RegistroLista::getIdElemento(){
	return this->idElemento;
}

string RegistroLista::getValor(){
	return this->valor;
}

void RegistroLista::setValor(string valor){
	this->valor = valor;
}

RegistroLista::~RegistroLista(){
}
