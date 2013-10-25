#include "Usuario.h"

Usuario::Usuario(){
	
}

string Usuario::serializar(){
	return "";
}

int Usuario::getDni(){
	return this->dni;
}

string Usuario::getNombre(){
	return this->nombre;
}

string Usuario::getApellido(){
	return this->apellido;
}

string Usuario::getProvincia(){
	return this->provincia;
}

string Usuario::getContrasena(){
	return this->contrasena;
}
string Usuario::getTipo(){
	return this->tipo;
}

vector<string> Usuario::getEmails(){
	return this->emails;
}

void Usuario::setDni(int dni){
	this->dni = dni;
}

void Usuario::setNombre(string nombre){
	this->nombre = nombre;
}

void Usuario::setApellido(string apellido){
	this->apellido = apellido;
}

void Usuario::setProvincia(string provincia){
	this->provincia = provincia;
}

void Usuario::setContrasena(string contrasena){
	this->contrasena = contrasena;
}

void Usuario::setTipo(string tipo){
	this->tipo = tipo; 
}

void Usuario::setEmail(string email){
	if(this->emails.size() == 3){
		//Ya no puedo guardar mas emails. Tiro excepcion
	}else{
		this->emails.push_back(email);
	}
}


Usuario::~Usuario(){
	
}