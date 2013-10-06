/*
 * Salvable.h
 *
 *  Created on: 08/04/2010
 *      Author: gonzalo
 */

#ifndef SALVABLE_H_
#define SALVABLE_H_

#include "../ArchivoBloque.h"
#include "exceptions/ExceptionOverflowTamBloque.h"
#include "Serializable.h"
#include <string>

using namespace std;

class Salvable {

private:
	ArchivoBloque* archivo;

public:
	Salvable(const string& ruta,const bool& sobreEscribir,const short& tamElem);
	virtual ~Salvable();

protected:
	void* leerBloque(const int& numBloque)const;
	void escribirBloque(void* aGuardar, const int tamanio, const int& numBloque);
	void escribirBloque(Serializable* aGuardar, const int& numBloque);
	const unsigned int getCantidadDeBloques()const;
	const int getTamanioBloque()const;
};

#endif /* SALVABLE_H_ */
