#ifndef ARCHIVOBLOQUES_H_
#define ARCHIVOBLOQUES_H_

using namespace std;

#include "ExceptionOverflowTamBloque.h"
#include "Archivo.h"

/**
 * Class ArchivoBloque
 *
 * Esta clase se encarga del manejo de un archivo ablocado en disco. Su estructura est� dada por bloques de
 * tama�o fijo, el cual se puede especificar al momento de la creaci�n.
 * Tanto para lectura como para escritura, los distintos bloques pueden ser accedidos directamente a trav�s del
 * n�mero de bloque.
 *
 */
class ArchivoBloque : public Archivo{

private:
	streamsize tamBloque;
    char *buffer; // se usa para la lectura

public:
	ArchivoBloque(const string& r,const short& t,const bool& sobreEscribir);
	void EscribirBloque(const string& bloque,const int& numBloque);
	const char* LeerBloque(const int& numBloque);
	const unsigned int getCantidadDeBloques ();
	~ArchivoBloque();
};

#endif /* ARCHIVOBLOQUES_H_ */
