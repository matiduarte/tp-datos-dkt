#ifndef BUCKET_H_
#define BUCKET_H_

using namespace std;

#include <list>
#include <string.h>
#include <algorithm>
#include <iostream>
#include "HashElement.h"
#include "HashClave.h"
#include "ExceptionBucketVacio.h"
#include "ExceptionElementoNoEncontrado.h"
#include "ExceptionElementoKeyYaIngresado.h"

class Bucket : public Serializable {

public:
	Bucket(const string aDesSerializar);
	~Bucket();

	virtual string serializar() const;
	friend ostream& operator<< (ostream& out, const Bucket& ptrObj);

	string obtenerValor(const HashClave& key)const;
	void eliminar(const HashClave& key);
	void insertar(const HashClave& key,const string& valor);
	void modificar(const HashClave& key,const string& nvoValor);

	const list<HashElement> getElementos()const;
	void vaciarElementos();

protected:
	virtual void desSerializar(const string data);

private:
	list<HashElement> elementos;
};

#endif /* BUCKET_H_ */
