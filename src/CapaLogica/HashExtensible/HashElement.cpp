#include "HashElement.h"

const string& HashElement::getValor()const{
	return valor;
}

const HashClave& HashElement::getClave()const{
	return *clave;
}

void HashElement::setValor(const string& nvoValor){
	valor=nvoValor;
}

//Dos elementos son iguales si sus Key son iguales
bool HashElement::operator==  (const HashElement& other) const{
	return *clave == *other.clave;
}

bool HashElement::operator==  (const HashClave& key) const{
	return key ==(*clave);
}

HashElement::HashElement(const HashClave& key, const string val): Serializable(";"){
	clave = new HashClave(key);
	valor = val;
}

HashElement::HashElement(const HashElement& old): Serializable(";"){
	clave = new HashClave(*(old.clave));
	valor = old.valor;
}

HashElement::HashElement(const string aDesSerializar): Serializable(";"){
	desSerializar(aDesSerializar);
}

ostream& operator<< (ostream& os, const HashElement& ptrObj){
	os << "\t" <<  *ptrObj.clave << "\t" << " Valor :" << ptrObj.valor << endl;
	return os;
}

string HashElement::serializar()const {
	string retorno;

	retorno=clave->serializar();
	retorno+= separadorElem;
	retorno+=valor;
	return retorno;
}

void HashElement::desSerializar(const string elemSerializado){
	string token;
	istringstream iss(elemSerializado);

	getline(iss, token, separadorElem.c_str()[0]);
	clave = new HashClave(token,true);

	getline(iss, token, separadorElem.c_str()[0]);
	valor = token;
}
HashElement::~HashElement(){
	if (clave) {
		delete clave;
	}
}
