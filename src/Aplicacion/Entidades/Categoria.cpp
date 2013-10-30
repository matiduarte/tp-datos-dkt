#include "Categoria.h"

Categoria::Categoria(){
	
}

string Categoria::serializar(){
	string retorno;
    retorno = StringUtil::int2string(this->id);
    retorno += separadorCamposEntidades;
    retorno += this->nombre;
    retorno += separadorCamposEntidades;
    retorno += this->descripcion;
    
    return retorno;
}

void Categoria::desSerializar(string aDeserealizar){
	vector<string> atributos = StringUtil::split(aDeserealizar,separadorCamposEntidades);
	this->id = StringUtil::str2int(atributos.at(0));
	this->nombre = atributos.at(1);
	this->descripcion = atributos.at(2);
}

int Categoria::getId(){
	return this->id;
}

string Categoria::getNombre(){
	return this->nombre;
}

string Categoria::getDescripcion(){
	return this->descripcion;
}

void Categoria::setId(int id){
	this->id = id;
}

void Categoria::setNombre(string nombre){
	this->nombre = nombre;
}

void Categoria::setDescripcion(string descripcion){
	this->descripcion = descripcion;
}

Categoria::~Categoria(){
	
}