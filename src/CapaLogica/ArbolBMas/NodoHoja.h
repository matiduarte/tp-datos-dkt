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
        bool overflow();
        bool underflow();
        bool capacidadMinima();
        bool isEmpty();
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
private:
        int persistirRegistros(char * bloque);

        unsigned int referencia_al_siguiente;
        list<RegistroArbol*> * claves_datos;
};


#endif /* NODOHOJA_H_ */
