#include "HashNode.h"

HashNode::HashNode(int dispersion){
	this->tamDispersion = dispersion;
	elementos=new Bucket("");
}

HashNode::HashNode(const string aDesSerializar): Serializable(){
	desSerializar(aDesSerializar);
}

const int HashNode::getTamDispersion()const{
	return tamDispersion;
}

void HashNode::insertarElemento(const HashClave& key, const string& valor){
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

const list<HashElement> HashNode::getElementos()const{
	return elementos->getElementos();
}

void HashNode::eliminarElemento(const HashClave& key){
	try {
		elementos->eliminar(key);
	} catch (ExceptionElementoNoEncontrado e) {
		throw e;
	} catch (ExceptionBucketVacio e) {
		throw ExceptionUnderflowNodo(tamDispersion);
	}
}

void HashNode::modificarElemento(const HashClave& key, const string& nvoValor){
	try {
		elementos->modificar(key,nvoValor);
	} catch (ExceptionElementoNoEncontrado e) {
		throw e;
	}
}

const string HashNode::buscarElemento(const HashClave& key)const{
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

string HashNode::serializar()const{
	string retorno;
	retorno = Convertidor::intToString(tamDispersion);
	retorno += separadorElem;
	retorno += elementos->serializar();
	return retorno;
}

//GPV asumo que es asi la cadena : dispersion@Bucket
//valores separados por separadorElem
void HashNode::desSerializar(const string data){
	size_t pos = data.find_first_of(separadorElem, 0);
	string dispersion = data.substr(0, pos);
	string bucket = data.substr(pos+1, data.size()-pos+1);
	tamDispersion = Convertidor::stringToInt(dispersion);
	elementos = new Bucket(bucket);
}

HashNode::~HashNode(){
	if (elementos) {
		delete elementos;
	}
}
