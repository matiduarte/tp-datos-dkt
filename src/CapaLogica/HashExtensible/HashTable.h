#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include "Salvable.h"
#include "HashClave.h"
#include "ElementKey.h"
#include "Convertidor.h"
#include "ExceptionTamTabla.h"

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
	int getElement(int pos);
	void setElement(int elem, int pos);
	void guardarTamTabla();

public:
	HashTable(const string& ruta, const bool& sobreEscribir, const short& tamElemento);
	int getElement(const HashClave& key);
	void setElement(int elem, const HashClave& key);
	int nodoNuevo(int tamDispersion, const HashClave& key);
	int nodoLibre(int tamDispersion, const HashClave& key);

	friend ostream& operator << (ostream& o, HashTable& tabla);

	~HashTable();
};

#endif /* HASHTABLE_H_ */
