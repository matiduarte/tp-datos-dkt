#include "Salvable.h"

using namespace std;

Salvable::Salvable(const string& ruta){
	//archivo = new ArchivoBloque(ruta, tamElem, sobreEscribir);
	archivo = new ArchivoBloque(ruta);
}

void Salvable::leerBloque(const int& numBloque,string* buffer)const{
    char* bloque = new char[archivo->getTamanoBloque()];
	archivo->leer(bloque, numBloque);
	*buffer = string(bloque);
}

void Salvable::escribirBloque(const string& bloque, const unsigned int& numBloque){
	try {
//		if(numBloque < archivo->getCantidadBloques() && archivo->getCantidadBloques() > 0){
//			archivo->borrar(numBloque);
//		}
		archivo->reescribir((char*)bloque.c_str(),numBloque);
	} catch (ExcepcionOverflowTamBloque& e) {
		throw e;
	}catch(ExcepcionBloqueInexistente& ex){
		archivo->escribir((char*)bloque.c_str());
	}

}

const unsigned int Salvable::getCantidadDeBloques ()const{
	return archivo->getCantidadBloques();
}

Salvable::~Salvable(){
	if (archivo) {
		delete archivo;
	}
}

