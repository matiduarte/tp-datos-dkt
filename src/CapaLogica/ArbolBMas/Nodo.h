/*
 * Nodo.h
 *
 *  Created on: 11/10/2013
 *      Author: juanma
 */

#ifndef NODO_H_
#define NODO_H_

#include "../../CapaFisica/Archivos/ArchivoBloque/ArchivoBloque.h"
#include "Clave.h"
#include "string.h"
#include "RegistroArbol.h"

class Nodo {

private:
	unsigned int nivel;
	unsigned int cantidadDeElementos;
	unsigned int NumeroDeBloque;

public:
	Nodo();
	virtual ~Nodo();

	/*getters y setters*/
	unsigned int getCantidadDeElementos();
	unsigned int getNivel();
	unsigned int getNumeroDeBloque();
	void setNumeroDeBloque(unsigned int numeroDeBloque);
	void setNivel(unsigned int nivel);
	void setCantidadDeElementos(unsigned int cantidad);

	/*----------------*/


	static void hidratar(char* bloque);
	virtual void persistir(ArchivoBloque* archivo);
	static Nodo* cargar(ArchivoBloque* arch, unsigned int indice);
	int agregar(string clave, unsigned int valor);
	virtual int tamanioOcupado();
	virtual bool estaVacio();
	void incrementarNivel();
	void decrementarNivel();
    int getTamanioUnderflow();
    int getTamanioOverflow();
    virtual bool hayOverflow();
    virtual bool hayUnderflow();



};


#endif /* NODO_H_ */
