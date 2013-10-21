/*
 * NodoHoja.h
 *
 *  Created on: 13/10/2013
 *      Author: juanma
 */

#ifndef NODOHOJA_H_
#define NODOHOJA_H_
#include "Nodo.h"


class NodoHoja: public Nodo {

	private:
        int persistirRegistros(char * bloque);

        unsigned int referenciaAlSiguiente;
        list<RegistroArbol*> * elementos;


public:
		NodoHoja();
        NodoHoja(ArchivoBloque * arch);
        virtual ~NodoHoja();
        void persistir(ArchivoBloque *archivo);
        int agregar(Clave clave, string valor);
        int agregar(RegistroArbol * reg);
        int agregar(list<RegistroArbol*> *lista);
        int baja (Clave clave, string valor);

        bool tieneLaClave(Clave clave);
        bool hayOverflow();
        bool hayUnderflow();
        bool capacidadMinima();
        bool estaVacio();
        int tamanioOcupado();
        void mostrar();

        int cantidadDeRegistros();
        string buscarClave(Clave clave);
        static NodoHoja* hidratar(char* bloque, unsigned int indice);
        static NodoHoja* cargar(ArchivoBloque* archivo, unsigned int indice);

        void setSiguiente (unsigned int nrobloque);
        void setClavesYDatos(list<RegistroArbol*> * lista);

        unsigned int getSiguiente();
        RegistroArbol* getRegistro(Clave clave);
        string getPrimerClave();
        string getClaveDelMedio();
        list<RegistroArbol*> * getClavesYDatos();
        list<RegistroArbol*> * getMitadDerecha();

};


#endif /* NODOHOJA_H_ */
