#include "Usuario.h"

Usuario::Usuario(){
	
}

string Usuario::serializar(){
	string retorno;
    retorno = StringUtil::int2string(this->dni);
    retorno += separadorCamposEntidades;
    retorno += this->nombre;
    retorno += separadorCamposEntidades;
    retorno += this->apellido;
    retorno += separadorCamposEntidades;
    retorno += this->provincia;
    retorno += separadorCamposEntidades;
    retorno += this->contrasena;
    retorno += separadorCamposEntidades;
    retorno += this->tipo;
    for(unsigned int i=0; i<this->emails.size();i++){
    	retorno += separadorCamposEntidades;
    	retorno += this->emails.at(i);
    }
    return retorno;
}

void Usuario::desSerializar(string aDeserealizar){
	vector<string> atributos = StringUtil::split(aDeserealizar,separadorCamposEntidades);
	this->dni = StringUtil::str2int(atributos.at(0));
	this->nombre = atributos.at(1);
	this->apellido = atributos.at(2);
	this->provincia = atributos.at(3);
	this->contrasena = atributos.at(4);
	this->tipo = atributos.at(5);
	for(unsigned int i=6; i<atributos.size();i++){
		this->setEmail(atributos.at(i));
	}
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
