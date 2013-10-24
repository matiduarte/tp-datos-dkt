/*
 * Salvable.h
 *
 *  Created on: 08/04/2010
 *      Author: gonzalo
 */

#ifndef SALVABLE_H_
#define SALVABLE_H_

#include "../../CapaFisica/Archivos/ArchivoBloque/ArchivoBloque.h"
#include "../../CapaFisica/Excepciones/ExcepcionOverflowTamBloque.h"
#include <string>

using namespace std;

class Salvable {

private:
	ArchivoBloque* archivo;

public:
	Salvable(const string& ruta,const bool& sobreEscribir,const short& tamElem);
	virtual ~Salvable();

protected:
	void leerBloque(const int& numBloque,string* buffer)const;
	void escribirBloque(const string& bloque, const unsigned int& numBloque);
	const unsigned int getCantidadDeBloques ()const;
};

#endif /* SALVABLE_H_ */