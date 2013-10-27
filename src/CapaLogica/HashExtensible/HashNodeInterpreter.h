#ifndef HASHNODEINTERPRETER_H_
#define HASHNODEINTERPRETER_H_

#include "HashNode.h"
#include "Salvable.h"
#include "ExceptionElementoNoEncontrado.h"
#include "../../CapaFisica/Excepciones/ExcepcionOverflowTamBloque.h"
#include "ExceptionOverflowNodo.h"
#include "ExceptionUnderflowNodo.h"

#include <utility>
#include <string>

using namespace std;

class HashNodeInterpreter : public Salvable {

private:
    void levantarNodo(const int & numeroNodo);
    void liberarNodo();

protected:
    HashNode* nodo;

public:
    HashNodeInterpreter(const string & ruta, const bool & sobreEscribir);
    virtual ~HashNodeInterpreter();
    void insertarElemento(const HashClave& key, const string& valor, const int numeroNodo);
	void modificarElemento(const HashClave& key, const string& nvoValor, const int numeroNodo);
	void eliminarElemento(const HashClave& key, const int numeroNodo);
	string buscarElemento(const HashClave& key, const int numeroNodo);

	void redispersarNodo(const list<pair<HashElement,bool> >&  bulkInsert,const int numNodoViejo,const int numNodoNuevo);
	void actualizarNodoLiberado(int numeroNodo);

	friend ostream& operator<< (ostream& out, HashNodeInterpreter& ptrObj);
};

#endif /* HASHNODEINTERPRETER_H_ */
