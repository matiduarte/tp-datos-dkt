#ifndef HASH_H_
#define HASH_H_

#include "../Registro.h"
#include "hashTable/HashTable.h"
#include "hashNode/HashNodeInterpreter.h"
#include "exceptions/ExceptionOverflowNodo.h"
#include "../ContextoHash.h"
#include "../Estructura.h"
#include "../ActualizarEstructuraExc.h"

#include <utility>

using namespace std;

class Hash : public Estructura {

private:
	HashTable* tabla;
	HashNodeInterpreter* interpreteNodo;

	void resolverOverflowNodo(const ExceptionOverflowNodo e, const ContextoHash& key);

public:
	Hash(const string& pathArchTabla, const string& pathArchNodos, const int& tamElemTabla, const int& tamElemNodo, bool sobreEscribir);

	virtual bool insertar (const Contexto & c, const TablaDeFrecuencias& t);
	virtual bool modificarRegistro(const Contexto& clave, const TablaDeFrecuencias& info) ;
	virtual bool actualizar (std::vector<Registro>& registros ) throw ( ActualizarEstructuraExc );
	virtual TablaDeFrecuencias* buscar ( const Contexto& c );
	virtual bool eliminar(const Registro & r);

	string imprimirNodo(int numeroNodo);

	friend ostream& operator << (ostream& o, Hash& tabla);
	~Hash();
};

#endif /* HASH_H_ */
