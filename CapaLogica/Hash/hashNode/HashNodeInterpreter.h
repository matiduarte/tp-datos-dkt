/*
 * HashNodeInterpreter.h
 *
 *  Created on: 01/04/2010
 *      Author: gonzalo
 */

#ifndef HASHNODEINTERPRETER_H_
#define HASHNODEINTERPRETER_H_

#include "HashNode.h"
#include "../Salvable.h"
#include "../exceptions/ExceptionElementoNoEncontrado.h"
#include "../exceptions/ExceptionOverflowNodo.h"
#include "../exceptions/ExceptionUnderflowNodo.h"

#include <utility>
#include <string>

using namespace std;

class HashNodeInterpreter : public Salvable {

private:
    void levantarNodo(const int & numeroNodo);
    void liberarNodo();
    void guardarNodo(HashNode* nodo, int numeroNodo);

protected:
    HashNode* nodo;

public:
    HashNodeInterpreter(const string & ruta, const bool & sobreEscribir, const short  & tamElemento);
    virtual ~HashNodeInterpreter();
    void insertarElemento(const ContextoHash& key, const TablaDeFrecuencias& valor, const int numeroNodo);
	void modificarElemento(const ContextoHash& key, const TablaDeFrecuencias& nvoValor, const int numeroNodo);
	void eliminarElemento(const ContextoHash& key, const int numeroNodo);
	TablaDeFrecuencias* buscarElemento(const ContextoHash& key, const int numeroNodo);

	void redispersarNodo(const list<pair<Registro*,bool> >&  bulkInsert,const int numNodoViejo,const int numNodoNuevo);
	void actualizarNodoLiberado(int numeroNodo);

	string imprimirNodo(int numeroNodo);

	friend ostream& operator<< (ostream& out, HashNodeInterpreter& ptrObj);
};

#endif /* HASHNODEINTERPRETER_H_ */
