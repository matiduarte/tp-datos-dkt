#include "HashNode.h"

HashNode::HashNode(int dispersion){
	this->tamDispersion = dispersion;
	//creo un Bucket vacio...
	elementos = new Bucket();
}

HashNode::HashNode(const void* aDesSerializar): Serializable(){
	desSerializar(aDesSerializar);
}

const int HashNode::getTamDispersion()const{
	return tamDispersion;
}

void HashNode::insertarElemento(const ContextoHash& key, const TablaDeFrecuencias& valor){
	try {
		elementos->insertar(key,valor);
	} catch (ExceptionElementoKeyYaIngresado e) {
		throw e;
	}
}

void HashNode::desbordoYResolvi(){
	//el nuevo tamdispersion va a ser el tamDispersionViejo*2...
	tamDispersion= tamDispersion*2;
}

void HashNode::liberoYResolvi(){
	//el nuevo tamdispersion va a ser el tamDispersionViejo/2...
	tamDispersion= tamDispersion/2;
}

void HashNode::vaciarElementos(){
	elementos->vaciarElementos();
}

const list<Registro*> HashNode::getElementos()const{
	return elementos->getElementos();
}

void HashNode::eliminarElemento(const ContextoHash& key){
	try {
		elementos->eliminar(key);
	} catch (ExceptionElementoNoEncontrado e) {
		throw e;
	} catch (ExceptionBucketVacio e) {
		throw ExceptionUnderflowNodo(tamDispersion);
	}
}

void HashNode::modificarElemento(const ContextoHash& key, const TablaDeFrecuencias& nvoValor){
	try {
		elementos->modificar(key,nvoValor);
	} catch (ExceptionElementoNoEncontrado e) {
		throw e;
	}
}

TablaDeFrecuencias* HashNode::buscarElemento(const ContextoHash& key)const{
	try {
		return elementos->obtenerValor(key);
	} catch (ExceptionElementoNoEncontrado e) {
		throw e;
	}
}

ostream& operator<< (ostream& os,const HashNode& ptrObj){
	os << "\tTamaño de dispersion : " << ptrObj.tamDispersion << endl;
	os << "\tElemenentos :" << endl;
	os <<  *ptrObj.elementos;
	return os;
}

//TamDispersion|Bucket
int HashNode::getTamanio()const{
	return (sizeof(int) + elementos->getTamanio());
}

void HashNode::serializar(void* aSerializar){
	char* pos = (char*)aSerializar;
	//tamaño de dispersion...
	Serializador::serializarInt(tamDispersion, pos);
	pos += sizeof(int);
	//bucket...
	elementos->serializar(pos);
}

void HashNode::desSerializar(const void* aDesSerializar){
	char* pos = (char*)aDesSerializar;
	tamDispersion = Serializador::desSerializarInt(pos);
	pos += sizeof(int);
	elementos = new Bucket(pos);
}

HashNode::~HashNode(){
	if (elementos) {
		delete elementos;
	}
}
