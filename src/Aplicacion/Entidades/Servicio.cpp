#include "Servicio.h"

Servicio::Servicio(){
	
}

string Servicio::serializar(){
	return "";
}

void Servicio::desSerializar(string aDeserealizar){
	
}

int Servicio::getId(){
	return this->id;
}

int Servicio::getIdProveedor(){
	return this->idProveedor;
}

string Servicio::getNombre(){
	return this->nombre;
}

string Servicio::getDescripcion(){
	return this->descripcion;
}

string Servicio::getTipo(){
	return this->tipo;
}

vector<Categoria*> Servicio::getCategorias(){
	return this->categorias;
}

void Servicio::setId(int id){
	this->id = id;
}

void Servicio::setIdProveedor(int idProveedor){
	this->idProveedor = idProveedor;
}

void Servicio::setNombre(string nombre){
	this->nombre = nombre;
}

void Servicio::setDescripcion(string descripcion){
	this->descripcion = descripcion;
}

void Servicio::setTipo(string tipo){
	this->tipo = tipo; 
}

void Servicio::setCategoria(Categoria* categoria){
	this->categorias.push_back(categoria);
}

Servicio::~Servicio(){
	
}