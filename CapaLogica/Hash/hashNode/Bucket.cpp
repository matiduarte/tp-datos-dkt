/*
 * Registro.cpp
 *
 *  Created on: 08/04/2010
 *      Author: gonzalo
 */

#include "Bucket.h"

Bucket::Bucket():Serializable(){
	//crea un Bucket vacio, sin elementos...
}

Bucket::Bucket(const void* aDesSerializar){
	desSerializar(aDesSerializar);
}

void Bucket::insertar(const Contexto& key, const TablaDeFrecuencias& valor){
	//Yo inserto si despues no alcanza el tamaño para guardar vemo...

	//Lo Busco, si ya existe en el bucket no lo puedo ingresar Nuevamente
	TablaDeFrecuencias* enCasoDeError;
	try {
		enCasoDeError=obtenerValor(key);
	} catch (ExceptionElementoNoEncontrado e) {
		//Lo ingreso
		Registro* insertar=new Registro(valor,key);
		elementos.push_back(insertar);
	}
}

void Bucket::eliminar(const ContextoHash& key){
	bool flagElimino=false;

	list<Registro*>::iterator it;
	it = elementos.begin();
	while( it != elementos.end() && !flagElimino ) {
		if (((*it)->getContexto2()) == key){
			delete *it;
			it=elementos.erase(it);
			flagElimino=true;
		}else{
			++it;
		}
	}
	if (!flagElimino) {
		throw ExceptionElementoNoEncontrado("No se encontro el elemento con dicha clave.");
	}else{
		//Me quedo sin elementos? tengo q borrar el bucket tambien
		if (flagElimino && elementos.size()==0) {
			throw ExceptionBucketVacio();
		}
	}
}

void Bucket::vaciarElementos(){
	elementos.clear();
}

const list<Registro*> Bucket::getElementos()const{
	return elementos;
}

void Bucket::modificar(const ContextoHash& key, const TablaDeFrecuencias& valor){
	//Busco
	list<Registro*>::iterator it;
	bool flagBreak=false;
	it = elementos.begin();
	while( (it) != elementos.end() &&  !flagBreak) {
		if (((*it)->getContexto2()) == key){
			(*it)->setInfo(valor);
			flagBreak=true;
		}else{
			++it;
		}
	}
	if (!flagBreak) {
		throw ExceptionElementoNoEncontrado("No se encontro el elemento con dicha clave.");
	}
}

TablaDeFrecuencias* Bucket::obtenerValor(const Contexto& key){
	list<Registro*>::iterator it;

	it = elementos.begin();
	while( it != elementos.end() ) {
		if (key == ((*it)->getContexto2())){
			return ((*it)->getInfo2());
		}
		++it;
	}
	throw ExceptionElementoNoEncontrado("No se encontro el elemento con dicha clave.");
}


ostream& operator<< (ostream& os, const Bucket& ptrObj){
	if (ptrObj.elementos.empty()) {
		os << "\t\t"  << "Vacio" << endl;
	}

	for (list<Registro*>::const_iterator it = ptrObj.elementos.begin(); it!=ptrObj.elementos.end(); it++) {
		os << "\t"  << *(*it);
	}

	return os;
}

int Bucket::getTamanio()const{
	int tam = 0;
	//cantidad de elementos...
	tam += sizeof(int);
	for (list<Registro*>::const_iterator it = elementos.begin(); it!=elementos.end(); it++) {
		//tamaño del elemento...
		tam += sizeof(int);
		//elemento...
		tam += (*it)->getTamanio();
	}
	return tam;
}

//CantElem|Long1|Elem1|Long2|Elem2|...
void Bucket::serializar(void* aSerializar) {
	char* pos = (char*)aSerializar;

	//guardo la cantidad de elementos...
	Serializador::serializarInt(elementos.size(), pos);
	pos += sizeof(int);
	for (list<Registro*>::iterator it = elementos.begin(); it!=elementos.end(); it++) {
		int tam = (*it)->getTamanio();
		//guardo el tamaño del elemento...
		Serializador::serializarInt(tam, pos);
		pos += sizeof(int);
		//guardo el elemento...
		(*it)->deshidratar( pos );
		pos += tam;
	}
}

//CantElem|Long1|Elem1|Long2|Elem2|...
void Bucket::desSerializar(const void* aDesSerializar){
	char* pos = (char*)aDesSerializar;

	unsigned int cantElem = Serializador::desSerializarInt(pos);
	pos += sizeof(int);
	for(unsigned int i=0;i<cantElem;++i){
		int longElem = Serializador::desSerializarInt(pos);
		char* elemSerializado = new char[longElem];
		pos += sizeof(int);
		memcpy(elemSerializado, pos, longElem);
		Registro* elem= new Registro(elemSerializado);
		elementos.push_back(elem);
		pos += longElem;
		delete[] elemSerializado;
	}
}

Bucket::~Bucket(){
	//Vacio la lista destruyendo los elementos
	while(!elementos.empty()){
		delete elementos.back();
		elementos.pop_back();
	}
	elementos.clear();
}
