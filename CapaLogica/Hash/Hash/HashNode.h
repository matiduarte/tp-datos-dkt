#ifndef HASHNODE_H_
#define HASHNODE_H_

#include <string.h>
#include <string>
#include "Bucket.h"
#include "HashElement.h"
#include "HashClave.h"
#include "Serializable.h"
#include "Convertidor.h"
#include "ExceptionElementoNoEncontrado.h"
#include "ExceptionElementoKeyYaIngresado.h"
#include "ExceptionUnderflowNodo.h"
#include "ExceptionOverflowNodo.h"

using namespace std;

class HashNode :public Serializable{

private:
	int tamDispersion;
	Bucket* elementos;

public:
	HashNode(const string aDesSerializar);
	HashNode(int dispersion);
	~HashNode();

	virtual string serializar()const ;
	friend ostream& operator<< (ostream& out, const HashNode& ptrObj);

	const int getTamDispersion()const;

	const string buscarElemento(const HashClave& key)const;
	void eliminarElemento(const HashClave& key);
	void insertarElemento(const HashClave& key, const string& valor);
	void modificarElemento(const HashClave& key, const string& nvoValor);

	//Devuelvo una copia al listado Completo
	const list<HashElement> getElementos()const;
	void vaciarElementos();
	void desbordoYResolvi();
	void liberoYResolvi();

protected:
	virtual void desSerializar(const string data);

};

#endif /* HASHNODE_H_ */
