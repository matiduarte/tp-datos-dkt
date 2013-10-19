/*
 * NodoInterno.h
 *
 *  Created on: 13/10/2013
 *      Author: juanma
 */

#ifndef NODOINTERNO_H_
#define NODOINTERNO_H_
#include "Nodo.h"
#include <list>
#include <iostream>
#include "Clave.h"

using namespace std;

class NodoInterno: public Nodo {

private:
	list<unsigned int> hijos;
	list<Clave> claves;

public:
                NodoInterno();
        NodoInterno(ArchivoBloque * arch);
        virtual ~NodoInterno();

        int alta(string cancion, string valor);
        void persistir(ArchivoBloque* archivo);

        static NodoInterno* hidratar(char* bloque, unsigned int indice);
        static NodoInterno* cargar(ArchivoBloque* archivo, unsigned int indice);
        bool overflow();
        bool underflow();
        bool capacidadMinima();
        bool isEmpty();
        int tamanioOcupado();

        void mostrar();
        int cantidadDeClaves();
        void agregarReferencia(Clave clave, unsigned int nodo);

        void PartirNodoADerecha(NodoInterno* hermano_derecho, NodoInterno* padre);
        void agregarClave(Clave clave);
        void borrarClave(Clave clave);
        void borrarClave(int indice);
        void borrarReferencia(unsigned int hijo);
        void agregarHijoAlFinal(unsigned int hijo);
        void agregarClaves(list<Clave> lista_claves);
        void agregarHijos(list<unsigned int> lista_hijos);
        unsigned int buscarNodo(int indice);
        int buscarClave(Clave clave);

        void setReferenciaAIzq(Nodo * nodo);
        void setReferenciaADer(Nodo * nodo);

        list<Clave> * getMitadDerechaClaves();
        list<unsigned int> * getMitadDerechaHijos();
        Clave getClaveDelMedio();
        unsigned int getUltimoNodo();
        unsigned int getNodoAnteriorA(unsigned int indice);
        list<Clave> getClaves();
        list<unsigned int> getHijos();
        Clave getClave(int index);
        void setHijos(list<unsigned int> hijos);
        void setClaves(list<Clave> claves);
        Clave getPrimerClave();
        Clave getUltimaClave();
};

#endif /* NODOINTERNO_H_ */
