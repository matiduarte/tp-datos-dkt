#ifndef HASH_H_
#define HASH_H_

#include "HashElement.h"
#include "HashTable.h"
#include "HashNodeInterpreter.h"
#include "ExceptionOverflowNodo.h"

#include <utility>

using namespace std;

class Hash {

private:
	HashTable* tabla;
	HashNodeInterpreter* interpreteNodo;

	int hashearClave(const int& clave);
	void resolverOverflowNodo(const ExceptionOverflowNodo e, const HashClave& key);

public:
	Hash(const string& pathArchTabla, const string& pathArchNodos);
	void insertarElemento(const string& clave, const string& valor);
	void modificarElemento(const string& clave, const string& valor);
	string buscarElemento(const string& clave);
	void elminarElemento(const string& clave);

	friend ostream& operator << (ostream& o, Hash& tabla);

	~Hash();
};

#endif /* HASH_H_ */
