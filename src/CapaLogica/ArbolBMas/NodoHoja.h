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
        static unsigned int tamanioMaximoBloque;

public:
        NodoHoja();
        NodoHoja(ArchivoBloque* arch);
        virtual ~NodoHoja();
        void persistir(ArchivoBloque* &archivo);
        int agregarValor(Clave clave, string valor);
        int agregarRegistro(RegistroArbol * reg);
        int agregarLista(list<RegistroArbol*> *lista);
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
        static NodoHoja* hidratar(char* bloque);
        static NodoHoja* cargar(ArchivoBloque* archivo, unsigned int nroDeBloque);
        void setSiguiente (unsigned int nrobloque);
        void setElementos(list<RegistroArbol*> * lista);
        unsigned int getSiguiente();
        RegistroArbol* getRegistro(Clave clave);
        Clave getPrimerClave();
        Clave getClaveDelMedio();
        list<RegistroArbol*> * getElementos();
        list<RegistroArbol*> * getMitadDerecha();
        list<string> * buscarYlistar(Clave clave);
};


#endif /* NODOHOJA_H_ */
