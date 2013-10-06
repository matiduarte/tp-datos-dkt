#ifndef HASHNODE_H_
#define HASHNODE_H_

#include <string.h>
#include <string>
#include "Bucket.h"
#include "../../ContextoHash.h"
#include "../../Registro.h"
#include "../Serializable.h"
#include "../utilidades/Serializador.h"
#include "../utilidades/Convertidor.h"
#include "../exceptions/ExceptionElementoNoEncontrado.h"
#include "../exceptions/ExceptionElementoKeyYaIngresado.h"
#include "../exceptions/ExceptionUnderflowNodo.h"
#include "../exceptions/ExceptionOverflowNodo.h"

using namespace std;

class HashNode :public Serializable{

private:
	int tamDispersion;
	Bucket* elementos;

public:
	HashNode(const void* aDesSerializar);
	HashNode(int dispersion);
	~HashNode();

	friend ostream& operator<< (ostream& out, const HashNode& ptrObj);

	const int getTamDispersion()const;

	TablaDeFrecuencias* buscarElemento(const ContextoHash& key)const;
	void eliminarElemento(const ContextoHash& key);
	void insertarElemento(const ContextoHash& key, const TablaDeFrecuencias& valor);
	void modificarElemento(const ContextoHash& key, const TablaDeFrecuencias& nvoValor);

	//Devuelvo una copia al listado Completo
	const list<Registro*> getElementos()const;
	void vaciarElementos();
	void desbordoYResolvi();
	void liberoYResolvi();

	//Serializable
	virtual int getTamanio()const;
	virtual void serializar(void* aSerializar);
protected:
	virtual void desSerializar(const void* aDesSerializar);

};

#endif /* HASHNODE_H_ */
