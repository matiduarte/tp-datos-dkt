/*
 * NodoHoja.cpp
 *
 *  Created on: 13/10/2013
 *      Author: juanma
 */

#include "NodoHoja.h"

NodoHoja::NodoHoja(){
    this->referenciaAlSiguiente = 0;
    this->elementos = new list<RegistroArbol*>();

}

NodoHoja::NodoHoja(ArchivoBloque * archivo) {
        // TODO Auto-generated constructor stub
        char bloque[2] = " ";
        unsigned int numeroDeBloque = archivo->escribir(bloque);
        this->setNumeroDeBloque(numeroDeBloque);
        this->referenciaAlSiguiente = 0;
        this->elementos = new list<RegistroArbol*>();

}

NodoHoja::~NodoHoja() {
        // TODO Auto-generated destructor stub
        list<RegistroArbol*>::iterator it;
        for (it = this->elementos->begin(); it != this->elementos->end(); it++){
                RegistroArbol* r = *it;
                delete r;
        }
        this->elementos->clear();
        delete this->elementos;

}


void NodoHoja::persistir(ArchivoBloque * archivo){
	//persisto de esta forma: [ nivel|cantidadDeElementos|numeroDeBloque|registros|referenciaAlSiguiente ]
	//persisto nivel, cantidadDeElementos, NumeroDeBloque:

        char bloqueApersistir[1024];
        unsigned int bytesOcupados, nivel, cantidadDeElementos, numeroDeBloque, referenciaAlSiguiente;
        bytesOcupados = 0;
        nivel = this->getNivel();
        cantidadDeElementos = this->getCantidadDeElementos();
        numeroDeBloque = this->getNumeroDeBloque();
        referenciaAlSiguiente = this->getSiguiente();


        memcpy(bloqueApersistir, (char *)&nivel, sizeof(unsigned int) );
        bytesOcupados += sizeof(unsigned int);

        memcpy(bloqueApersistir, (char *)&cantidadDeElementos, sizeof(unsigned int) );
        bytesOcupados += sizeof(unsigned int);

        memcpy(bloqueApersistir, (char *)&numeroDeBloque, sizeof(unsigned int) );
        bytesOcupados += sizeof(unsigned int);

        //Persisto los registros
        bytesOcupados += this->persistirRegistros(bloqueApersistir + bytesOcupados);

        //Persisto la referencia al siguiente
        memcpy(bloqueApersistir+bytesOcupados,(char *)&referenciaAlSiguiente,sizeof(unsigned int));
        bytesOcupados += sizeof (unsigned int);

        archivo->reescribir(bloqueApersistir,this->getNumeroDeBloque());

}


NodoHoja* NodoHoja::hidratar(char* bloque, unsigned int indice){
	//hidrato en este orden: [ nivel|cantidadDeElementos|numeroDeBloque|registros|referenciaAlSiguiente ]

		unsigned int nivel, cantidadDeElementos, numeroDeBloque, referenciaAlSiguiente;
		NodoHoja* nodoAdevolver= new NodoHoja();

        unsigned int bytesRecorridos=0;


        memcpy( &nivel ,bloque, sizeof (unsigned int));
        nodoAdevolver->setNivel(nivel);
        bytesRecorridos+=sizeof (unsigned int);

        memcpy(&cantidadDeElementos, bloque + bytesRecorridos,sizeof (unsigned int));
        nodoAdevolver->setCantidadDeElementos(cantidadDeElementos);
        bytesRecorridos+= sizeof (unsigned int);

        memcpy(&numeroDeBloque, bloque + bytesRecorridos,sizeof (unsigned int));
               nodoAdevolver->setNumeroDeBloque(numeroDeBloque);
               bytesRecorridos+= sizeof (unsigned int);

        //hidrato los registros:

        RegistroArbol* registroAux = new RegistroArbol();
        for(unsigned int i=0; i<cantidadDeElementos; i++){
			registroAux->hidratar(bloque + bytesRecorridos);
			nodoAdevolver->elementos->push_back(registroAux);
			bytesRecorridos+= registroAux->cantidadDeBytesOcupados();
        }



        //hidrato la referencia al siguiente:
        memcpy( &referenciaAlSiguiente ,bloque, sizeof (unsigned int));
        nodoAdevolver->setSiguiente(referenciaAlSiguiente);
        bytesRecorridos+=sizeof (unsigned int);

        return nodoAdevolver;


}


unsigned int NodoHoja::getSiguiente(){
        return this->referenciaAlSiguiente;
}


int NodoHoja::persistirRegistros(char* bloque){
        //Persiste los registros, devuelve la cantidad de bytes que fueron escritos.
        int contador = 0;
        list<RegistroArbol*>::iterator it;
        for (it = this->elementos->begin(); it != this->elementos->end(); it++){
                contador += (*it)->persistir(bloque+contador);
        }
        return contador;
}


void NodoHoja::setSiguiente (unsigned int nrobloque){
        this->referenciaAlSiguiente = nrobloque;
}


int NodoHoja::agregar(Clave clave, string valor){
        //Devuelve: 0 si el nodo no se actualizo
        //                      1 si el nodo se actualizo
        //                      2 si desbordo
        //                      3 si ya existe el id que quiero agregar
        //Si ya tengo una clave igual, agrego a ese registro.
        int indicador = 1;
        if (this->tieneLaClave(clave)){
                RegistroArbol * registro = this->getRegistro(clave);
                indicador = registro->agregar(clave,valor);
        } else{
                bool agregado=false;
                list<RegistroArbol*>::iterator it;
                RegistroArbol * reg = new RegistroArbol();
                indicador = reg->agregar(clave ,valor);
                it=this->elementos->begin();

                while (  (agregado==false) && (it!=this->elementos->end())  ){ //Busco el lugar que le corresponde en la lista
                        if (clave<( (*it)->getClave() )){
                                this->elementos->insert(it,reg);
                                agregado=true;
                        }
                        it++;
                }
                if (agregado==false)
                        this->elementos->push_back(reg);
        }
        if (overflow()){
                indicador = 2;
        }
        return indicador;
}

bool NodoHoja::tieneLaClave(Clave clave){
        Clave *claveAux = new Clave();
        list<RegistroArbol*>::iterator it;

        for (it = this->elementos->begin(); it != this->elementos->end(); ++it){
                claveAux->setClave((*it)->getClave());
                if (claveAux->getClave() == clave.getClave()){
                        return true;
                }
        }
        return false;
}


int NodoHoja::tamanioOcupado(){
        //Devuelve el tamaño del nodo en el bloque
        int bytes_ocupados = 0;
        //itero la lista de registros sumando los bytes ocupados por cada uno
        list<RegistroArbol*>::iterator it;
        for (it = this->elementos->begin(); it != this->elementos->end(); it++){
                bytes_ocupados += (*it)->cantidadDeBytesOcupados();
        }
        bytes_ocupados += sizeof(unsigned int); //Referencia al siguiente
        bytes_ocupados += sizeof(unsigned int); //nro de bloque
        bytes_ocupados += sizeof(unsigned int); //nivel
        bytes_ocupados += sizeof(unsigned int); //cantidadDeElementos

        return bytes_ocupados;
}

bool NodoHoja::overflow(){
        int aux = 1024 * 0.8;
        if (aux >= this->tamanioOcupado()){
                return false;
        }else {
                return true;
        }
}

RegistroArbol* NodoHoja::getRegistro(Clave clave){
        //Devuelve el registro correspondiente a la clave
        list<RegistroArbol*>::iterator it;
        RegistroArbol * reg_actual = NULL;
        for (it = this->elementos->begin(); it != this->elementos->end(); it++){
                reg_actual = *it;
                Clave aux = (reg_actual)->getClave();
                if (aux == clave){
                        return reg_actual;
                }
        }
        return reg_actual;
}

