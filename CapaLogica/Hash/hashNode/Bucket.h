/*
 * BUCKET.h
 *
 *  Created on: 08/04/2010
 *      Author: gonzalo
 */

#ifndef BUCKET_H_
#define BUCKET_H_

using namespace std;

#include <list>
#include <string.h>
#include <algorithm>
#include <iostream>
#include "../../Registro.h"
#include "../../ContextoHash.h"
#include "../exceptions/ExceptionBucketVacio.h"
#include "../exceptions/ExceptionElementoNoEncontrado.h"
#include "../exceptions/ExceptionElementoKeyYaIngresado.h"
#include "../utilidades/Serializador.h"
#include "../Serializable.h"

class Bucket : public Serializable {

public:
	Bucket();
	Bucket(const void* aDesSerializar);
	~Bucket();

	friend ostream& operator<< (ostream& out, const Bucket& ptrObj);

	TablaDeFrecuencias* obtenerValor(const Contexto& key);
	void eliminar(const ContextoHash& key);
	void insertar(const Contexto& key,const TablaDeFrecuencias& valor);
	void modificar(const ContextoHash& key,const TablaDeFrecuencias& valor);

	const list<Registro*> getElementos()const;
	void vaciarElementos();

	//Serializable
	virtual int getTamanio()const;
	virtual void serializar(void* aSerializar);
protected:
	virtual void desSerializar(const void* aDesSerializar);

private:
	list<Registro*> elementos;
};

#endif /* BUCKET_H_ */
