#include "Categoria.h"

Categoria::Categoria(){
	
}

string Categoria::serializar(){
	return "";
}

void Categoria::desSerializar(string aDeserealizar){
	
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