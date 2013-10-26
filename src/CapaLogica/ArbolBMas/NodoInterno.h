/*
 * NodoInterno.h
 *
 *  Created on: 13/10/2013
 *      Author: juanma
 */

#ifndef NODOINTERNO_H_
#define NODOINTERNO_H_


#include <list>
#include <iostream>
#include "Clave.h"
#include "Nodo.h"

using namespace std;

class NodoInterno: public Nodo {

private:

	list<unsigned int> hijos;
	list<Clave> claves;

	static unsigned int tamanioMaximoBloque;
	static unsigned int tamanioMaximoClave;

public:
        NodoInterno();
        NodoInterno(ArchivoBloque * arch);
        virtual ~NodoInterno();

        int alta(string cancion, string valor);
        void persistir(ArchivoBloque* archivo);

        static NodoInterno* hidratar(char* bloque, unsigned int indice);
        static NodoInterno* cargar(ArchivoBloque* archivo, unsigned int indice);
        bool capacidadMinima();
        bool estaVacio();
        int tamanioOcupado();

        int getCantidadDeClaves();
        void agregarReferencia(Clave clave, unsigned int nodo);

        void PartirNodoADerecha(NodoInterno* hermano_derecho, NodoInterno* padre);
        void agregarClave(Clave clave);
        void borrarClave(Clave clave);
        void borrarReferencia(unsigned int hijo);
        void agregarHijoAlFinal(unsigned int hijo);
        void agregarClaves(list<Clave> lista_claves);
        void agregarHijos(list<unsigned int> lista_hijos);
        unsigned int buscarNodo(int indice);
        int buscarClave(Clave clave);
        int persistirReferenciasANodosInternos(char* bloque);
        void setReferenciaAIzq(Nodo* nodo);
        void setReferenciaADer(Nodo* nodo);

        list<Clave> * getMitadDerechaClaves();
        list<unsigned int> * getMitadDerechaHijos();
        Clave getClaveDelMedio();
        unsigned int getUltimoNodo();
        unsigned int getNodoAnteriorA(unsigned int indice);
        list<Clave> getClaves();
        list<unsigned int> getHijos();
        Clave getClave(int indice);
        void setHijos(list<unsigned int> hijos);
        void setClaves(list<Clave> claves);
        Clave getPrimerClave();
        Clave getUltimaClave();
        bool hayOverflow();
        bool hayUnderflow();

};

#endif /* NODOINTERNO_H_ */
