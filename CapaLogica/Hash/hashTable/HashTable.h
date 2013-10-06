#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include "../Salvable.h"
#include "../../Registro.h"
#include "../../ContextoHash.h"
#include "../utilidades/Convertidor.h"
#include "../utilidades/Serializador.h"
#include "../exceptions/ExceptionTamTabla.h"

#include <list>
#include <stdlib.h>
#include <cstdlib>

using namespace std;

class HashTable : public Salvable {

private:
	int tamTabla;

	void duplicarTabla();
	void reducirTabla();
	int buscarNodoVacio();
	void recorrerActualizandoTabla(const int& posInicial, const int& salto, const int& nuevoElem);
	bool tablaEspejo();
	void guardarTamTabla();
	void guardarElem(int elem, int pos);

public:

	HashTable(const string& ruta, const bool& sobreEscribir, const short& tamElemento);
	int nodoNuevo(int tamDispersion, const ContextoHash& key);
	int nodoLibre(int tamDispersion, const ContextoHash& key);
	int getElement(int pos);
	void setElement(int elem, int pos);
	int getElement(const ContextoHash& key);
	void setElement(int elem, const ContextoHash& key);


	friend ostream& operator << (ostream& o, HashTable& tabla);

	~HashTable();
};

#endif /* HASHTABLE_H_ */
