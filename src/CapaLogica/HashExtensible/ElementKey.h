#ifndef ELEMENTKEY_H_
#define ELEMENTKEY_H_

#include <string>
#include "Serializable.h"
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

template <typename T>

class ElementKey: public Serializable {

protected:
	T valor;

public:
	ElementKey(T valor, bool desSerializar);
	//ElementKey(const string aDesSerializar);
	ElementKey(const ElementKey& elCopia); //Constructor copia
	~ElementKey();

	bool operator ==  (const ElementKey<T>& other) const;
	bool operator !=  (const ElementKey<T>& other) const;
	ElementKey<T>& operator=(const ElementKey<T>& elem);

	T& operator=(const T& elem);

	const T& getValor()const;

	virtual string serializar()const;

	friend ostream& operator<< (ostream& os, const ElementKey<T>& ptrObj){
		os << ptrObj.valor;
		return os;
	}

protected:
	virtual void desSerializar(const string aDesSerializar);

};

//Implementacion...

template <typename T>
ElementKey<T>::ElementKey(T nValor, bool desSerializar){
	valor = nValor;
}

template <typename T>
ElementKey<T>::ElementKey(const ElementKey<T>& elCopia){
	valor = elCopia.valor;
}

//template <typename T>
//ElementKey<T>::ElementKey(const string aDesSerializar){
//	valor = NULL;
//	desSerializar(aDesSerializar);
//}

template <typename T>
ElementKey<T>::~ElementKey(){
}

template <typename T>
bool ElementKey<T>::operator ==  (const ElementKey<T>& other) const{
	return valor == other.valor;
}


template <typename T>
bool ElementKey<T>::operator !=  (const ElementKey<T>& other) const{
	return !operator ==(other);
}

template <typename T>
ElementKey<T>& ElementKey<T>::operator=(const ElementKey<T>& elem){
	if (&elem != this) {
		valor = elem.valor;
	}
	return *this;

}


template <typename T>
T& ElementKey<T>::operator=(const T& elem){
	valor = elem.valor;

	return *this;
}

template <typename T>
const T& ElementKey<T>::getValor()const{
	return valor;
}



template <typename T>
string ElementKey<T>::serializar()const{
	if (valor == "") {
		//No deberia poder pasar esto
		return "NULL";
	}

	string strRetorno;
	ostringstream retorno;
	retorno << (T)valor;
	strRetorno = retorno.str();
	return strRetorno;
}

template <typename T>
void ElementKey<T>::desSerializar(const string aDesSerializar){
	T intermedio;

	istringstream stream;
	stream.str(aDesSerializar);
	stream >> intermedio;

	valor = (intermedio);
}





#endif /* ELEMENTKEY_H_ */

