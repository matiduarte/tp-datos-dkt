
#include "Bucket.h"

Bucket::Bucket(const string aDesSerializar):Serializable(){
	desSerializar(aDesSerializar);
}

void Bucket::insertar(const HashClave& key, const string& valor){
	//Yo inserto si despues no alcanza el tamaño para guardar vemo...

	//Lo Busco, si ya existe en el bucket no lo puedo ingresar Nuevamente
	string enCasoDeError;
	try {
		enCasoDeError=obtenerValor(key);
	} catch (ExceptionElementoNoEncontrado e) {
		//Lo ingreso
		HashElement insertar(key,valor);
		elementos.push_back(insertar);
	}

	if (!enCasoDeError.empty()) {
		throw ExceptionElementoKeyYaIngresado("Clave " + key.serializar() +
					" ya fue ingresada con valor " + enCasoDeError);
	}

}

void Bucket::eliminar(const HashClave& key){
	bool flagElimino=false;

	list<HashElement>::iterator it;
	it = elementos.begin();
	while( it != elementos.end() && !flagElimino ) {
		if ((*it)==key){
			it=elementos.erase(it);
			flagElimino=true;
		}else{
			++it;
		}
	}
	if (!flagElimino) {
		throw ExceptionElementoNoEncontrado("No se encontro el elemento con clave " + key.serializar());
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

const list<HashElement> Bucket::getElementos()const{
	return elementos;
}

void Bucket::modificar(const HashClave& key, const string& nvoValor){
	//Busco
	list<HashElement>::iterator it;
	bool flagBreak=false;
	it = elementos.begin();
	while( it != elementos.end() &&  !flagBreak) {
		if ((*it)==key){
			it->setValor(nvoValor);
			flagBreak=true;
		}else{
			++it;
		}
	}
	if (!flagBreak) {
		throw ExceptionElementoNoEncontrado("No se encontro el elemento con clave " + key.serializar());
	}
}

string Bucket::obtenerValor(const HashClave& key)const{
	list<HashElement>::const_iterator it;

	it = elementos.begin();
	while( it != elementos.end() ) {
		if ((*it)==key){
			return (it->getValor());
		}
		++it;
	}
	throw ExceptionElementoNoEncontrado("No se encontro el elemento con clave " + key.serializar());
}


ostream& operator<< (ostream& os, const Bucket& ptrObj){
	if (ptrObj.elementos.empty()) {
		os << "\t\t"  << "Vacio" << endl;
	}

	for (list<HashElement>::const_iterator it = ptrObj.elementos.begin(); it!=ptrObj.elementos.end(); it++) {
		os << "\t"  << *it;
	}

	return os;
}

string Bucket::serializar()const{
	//Acelerar Respuesta
	if (elementos.empty()) {
		return "";
	}
	bool first=true;
	string retorno;

	for (list<HashElement>::const_iterator it = elementos.begin(); it!=elementos.end(); it++) {
		if (first) {
			retorno+=it->serializar();
			first=false;
		} else{
			retorno+=separadorElem + it->serializar();
		}
	}
	return retorno;
}

// cadena : 1;2@2;3@4;5
// valores separados por @
// key;Valor@key;Valor
void Bucket::desSerializar(const string data){
	//Para no tener un constructor vacio
	if (data.empty()){
		return;
	}

	string token;
	istringstream iss(data);
	while (getline(iss, token, separadorElem.c_str()[0])){
		HashElement insertar(token);
		elementos.push_back(insertar);
	}
}

Bucket::~Bucket(){
	//Vacio la lista destruyendo los elementos
	elementos.clear();
}
