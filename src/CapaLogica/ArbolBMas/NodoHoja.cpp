/*
 * NodoHoja.cpp
 *
 *  Created on: 13/10/2013
 *      Author: juanma
 */

#include "NodoHoja.h"

unsigned int NodoHoja::tamanioMaximoBloque;

NodoHoja::NodoHoja(){
    this->referenciaAlSiguiente = 0;
    this->elementos = new list<RegistroArbol*>();

	try{
		LectorConfig* lector = LectorConfig::getLector(rutaConfig);
    	this->tamanioMaximoBloque = lector->stringToInt(lector->getValor("tamanioBloque"));
	}
	catch(Excepcion &e){

		this->tamanioMaximoBloque = TAMANIOBLOQUE_DEFAULT;
	}
}

NodoHoja::NodoHoja(ArchivoBloque* archivo) {
        // TODO Auto-generated constructor stub
        char bloque[archivo->getTamanoBloque()];
        unsigned int numeroDeBloque = archivo->escribir(bloque);
        this->setNumeroDeBloque(numeroDeBloque);
        this->referenciaAlSiguiente = 0;
        this->elementos = new list<RegistroArbol*>();

    	try{
    		LectorConfig* lector = LectorConfig::getLector(rutaConfig);
        	this->tamanioMaximoBloque = lector->stringToInt(lector->getValor("tamanioBloque"));
    	}
    	catch(Excepcion &e){

    		this->tamanioMaximoBloque = TAMANIOBLOQUE_DEFAULT;
    	}
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

NodoHoja* NodoHoja::cargar(ArchivoBloque* archivo, unsigned int nroDeBloque){

	char* bloque = new char[archivo->getTamanoBloque()];
	archivo->leer(bloque, nroDeBloque);
	return NodoHoja::hidratar(bloque);
}

void NodoHoja::mostrar(){
        cout << "Nro de nodo: ";
        cout << this->getNumeroDeBloque();
        cout << " ,";
        list<RegistroArbol*>::iterator it;
        for (it = elementos->begin(); it != elementos->end(); it++){
                (*it)->mostrar();
        }
        cout << ", ";
        cout << this->referenciaAlSiguiente;
        cout << "\n";

}
void NodoHoja::persistir(ArchivoBloque *& archivo){
	//persisto de esta forma: [ nivel|cantidadDeElementos|numeroDeBloque|registros|referenciaAlSiguiente ]
	//persisto nivel, cantidadDeElementos, NumeroDeBloque:

        char bloqueApersistir[tamanioMaximoBloque];
        unsigned int bytesOcupados, nivel, cantidadDeElementos, numeroDeBloque, referenciaAlSiguiente;
        bytesOcupados = 0;
        nivel = this->getNivel();
        cantidadDeElementos = this->getCantidadDeElementos();
        numeroDeBloque = this->getNumeroDeBloque();
        referenciaAlSiguiente = this->getSiguiente();


        memcpy(bloqueApersistir, (char *)&nivel, sizeof(unsigned int) );
        bytesOcupados += sizeof(unsigned int);

        memcpy(bloqueApersistir+bytesOcupados, (char *)&cantidadDeElementos, sizeof(unsigned int) );
        bytesOcupados += sizeof(unsigned int);

        memcpy(bloqueApersistir+bytesOcupados, (char *)&numeroDeBloque, sizeof(unsigned int) );
        bytesOcupados += sizeof(unsigned int);

        //Persisto la referencia al siguiente
        memcpy(bloqueApersistir+bytesOcupados,(char *)&referenciaAlSiguiente,sizeof(unsigned int));
        bytesOcupados += sizeof (unsigned int);

        //Persisto los registros
        bytesOcupados += this->persistirRegistros(bloqueApersistir + bytesOcupados);



        archivo->reescribir(bloqueApersistir, numeroDeBloque);

}


NodoHoja* NodoHoja::hidratar(char* bloque){
	//hidrato en este orden: [ nivel|cantidadDeElementos|numeroDeBloque|registros|referenciaAlSiguiente ]

		unsigned int nivel, cantidadDeElementos, numeroDeBloque, referenciaAlSiguiente;
		NodoHoja* nodoAdevolver= new NodoHoja();

        unsigned int bytesRecorridos=0;


        memcpy((char *) &nivel ,bloque, sizeof (unsigned int));
        nodoAdevolver->setNivel(nivel);
        bytesRecorridos+=sizeof (unsigned int);

        memcpy((char *)&cantidadDeElementos, bloque + bytesRecorridos,sizeof (unsigned int));
        nodoAdevolver->setCantidadDeElementos(cantidadDeElementos);
        bytesRecorridos+= sizeof (unsigned int);

        memcpy((char *)&numeroDeBloque, bloque + bytesRecorridos,sizeof (unsigned int));
               nodoAdevolver->setNumeroDeBloque(numeroDeBloque);
               bytesRecorridos+= sizeof (unsigned int);

        //hidrato la referencia al siguiente:
        memcpy( (char *)&referenciaAlSiguiente ,bloque + bytesRecorridos , sizeof (unsigned int));
        nodoAdevolver->setSiguiente(referenciaAlSiguiente);
        bytesRecorridos+=sizeof (unsigned int);


        //hidrato los registros:

        RegistroArbol* registroAux = new RegistroArbol();
        RegistroArbol* registroHidratado;
        for(unsigned int i=0; i<cantidadDeElementos; i++){
			registroHidratado = (registroAux->hidratar(bloque + bytesRecorridos));
			nodoAdevolver->elementos->push_back(registroHidratado);
			bytesRecorridos+= registroHidratado->cantidadDeBytesOcupados();
        }




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
        //1 si el nodo se actualizo
        //2 si desbordo
        //3 si ya existe el valor que quiero agregar
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
                this->incrementarCantidadDeElementos();
        }
        if (hayOverflow()){
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



RegistroArbol* NodoHoja::getRegistro(Clave clave){
        //Devuelve el registro correspondiente a la clave
        list<RegistroArbol*>::iterator it;
        RegistroArbol * reg_actual = new RegistroArbol();
        for (it = this->elementos->begin(); it != this->elementos->end(); it++){
                reg_actual = *it;
                Clave aux = (reg_actual)->getClaveEntera();
                if (aux.getClave() == clave.getClave()){
                        return reg_actual;
                }
        }
        return reg_actual;
}

bool NodoHoja::hayOverflow(){

	int tamanioMaximo = getTamanioOverflow();

	return ((tamanioMaximo) < (this->tamanioOcupado()));
}

bool NodoHoja::hayUnderflow(){

	int tamanioMinimo = getTamanioUnderflow();

	return ((tamanioMinimo) > (this->tamanioOcupado()));
}

bool NodoHoja::estaVacio(){
	return this->getElementos()->empty();}


int NodoHoja::baja(Clave clave, string valor){
        //Elimino el registro completo del valor requerido.
        //Devuelve 3 si quedo en underflow
        //Devuelve 2 si se borro exitosamente
        // 1 si no contiene esa clave o valor
        // 0 si hubo un error
        int indicador = 1;
        if (this->tieneLaClave(clave)){
                //Si tiene la clave la borro
                RegistroArbol * reg = getRegistro(clave);
                indicador = reg->borrar(clave,valor);
                //Si el registro quedo vacio tengo que eliminarlo
                if (indicador == 3){
                        //Lo elimino de la lista
                        this->getElementos()->remove(reg);
                        //Y de memoria
                        delete reg;
                        // Seteo el indicador para que informe que se
                        // borro exitosamente
                        indicador = 2;

                        //decremento la cantidad de elementos
                        this->decrementarCantidadDeElementos();
                }
        } else{
                //Si no tiene la clave devuelve 1
                indicador = 1;
        }
        if (this->hayUnderflow()){ //Si es raiz no
                indicador = 3;
        }
        return indicador;
}

void NodoHoja::setElementos(list<RegistroArbol*> * listaElementos){
	this->elementos = listaElementos;
	this->setCantidadDeElementos(listaElementos->size());
}


list<RegistroArbol*> * NodoHoja::getElementos(){
	return this->elementos;
}

bool NodoHoja::capacidadMinima(){
        //Devuelve true si se encuentra en la capacidad minima
        int aux = tamanioMaximoBloque * 0.25;
        RegistroArbol *reg;
        reg = this->getElementos()->back();
        int tamUltimoRegistro = reg->cantidadDeBytesOcupados();

        if (aux >= (this->tamanioOcupado()) - tamUltimoRegistro){
                return true;
        }
        return false;
}



int NodoHoja::cantidadDeRegistros(){
        //Devuelve la cantidad de registros en el nodo
        return this->getElementos()->size();
}



string NodoHoja::buscarClave (Clave clave){
        //Busca en todos los registros del nodo si tiene la clave. Si la tiene devuelve
        //la lista correspondiente al registro
        list<RegistroArbol*>::iterator it;
        RegistroArbol * registro;
        Clave claveAux;
        for (it = this->getElementos()->begin(); it != this->getElementos()->end(); ++it){
                registro = *it;
                claveAux  = registro->getClave();
                if (claveAux==clave){
                        return registro->getValor();
                }
        }

        return "NO EXISTE"; //ARREGLAR ESTO, HACER ALGO PORQUE NO EXISTE MAS
}


int NodoHoja::agregar(RegistroArbol * reg){
        //Devuelve: 0 si el nodo no se actualizo
        //                      1 si el nodo se actualizo
        //                      2 si desbordo
        //                      3 si ya existe el valor que quiero agregar
        //Si ya tengo una clave igual, agrego a ese registro.
        int indicador = 1;
        if (this->tieneLaClave(reg->getClave())){
                RegistroArbol * registro = this->getRegistro(reg->getClave());
                registro->setValor(reg->getValor());

        } else{
                this->getElementos()->push_back(reg);
                this->incrementarCantidadDeElementos();
        }
        if (hayOverflow()){
                indicador = 2;
        }
        return indicador;

}


int NodoHoja::agregar(list<RegistroArbol*> * lista){
        list<RegistroArbol*>::iterator it;
        RegistroArbol* registro;
        for (it = lista->begin(); it != lista->end(); it++){
                registro = *it;
                this->agregar(registro);
        }
        if (hayOverflow()) return 2;
        return 1;
}


Clave NodoHoja::getPrimerClave(){
        return (this->getElementos()->front())->getClaveEntera();
}

list<RegistroArbol*> * NodoHoja::getMitadDerecha(){
        //Elimina la ultima mitad de los registros del nodo y los devuelve.
        list<RegistroArbol*>::iterator it;
        int cantidad = cantidadDeRegistros();
        int aux = 0;
        RegistroArbol* reg;
        list <RegistroArbol*>* mitad_derecha = new list<RegistroArbol*> ();
        while(aux <= cantidad){
                if (aux > cantidad/2){
                        reg = this->getElementos()->back();
                        this->getElementos()->pop_back();
                        mitad_derecha->push_front(reg);
                        this->decrementarCantidadDeElementos();
                }
                aux ++;
        }

        return mitad_derecha;
}

Clave NodoHoja::getClaveDelMedio(){

	//Funcion que se llama si el nodo esta en overflow. Devuelve la clave del medio para
	//insertarla en algun futuro nodo padre
	list<RegistroArbol*>::iterator it;
	int cantidad = this->getElementos()->size();
	int aux = 0;
	Clave claveRetorno;
	for (it = this->getElementos()->begin(); it != this->getElementos()->end(); it++){
		aux++;
		if (aux > cantidad/2){
			claveRetorno = (*it)->getClaveEntera();
			break;
        }
	}

	return claveRetorno;
}
//devuelve la lista de los valores de los registros con clave "clave".
list<string> * NodoHoja::buscarYlistar(Clave clave){

	list<string>* listaAdevolver = new list<string>();
	list<RegistroArbol*>::iterator it;
	RegistroArbol * registro;
	Clave claveAux;
	it = this->getElementos()->begin();

	while ( it != this->getElementos()->end()){
	        registro = *it;
	        claveAux  = registro->getClave();
	        if (claveAux==clave){
	        listaAdevolver->push_back(registro->getValor());
	        }
	        it++;
	}
	return listaAdevolver;
}
