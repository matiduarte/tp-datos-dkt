#include "Consulta.h"

Consulta::Consulta(){
	
}

string Consulta::serializar(){
	return "";
}

void Consulta::desSerializar(string aDeserealizar){
}

int Consulta::getId(){
	return this->id;
}

int Consulta::getIdServicio(){
	return this->idServicio;
}

int Consulta::getIdUsuario(){
	return this->idUsuario;
}

string Consulta::getConsulta(){
	return this->consulta;
}

string Consulta::getFechaConsulta(){
	return this->fechaConsulta;
}

string Consulta::getHoraConsulta(){
	return this->horaConsulta;
}

string Consulta::getRespuesta(){
	return this->respuesta;
}
string Consulta::getFechaRespuesta(){	
	return this->fechaRespuesta;
}

string Consulta::getHoraRespuesta(){
	return this->horaRespuesta;
}
bool Consulta::getOculta(){
	return this->oculta;
}

void Consulta::setId(int id){
	this->id = id;
}

void Consulta::setIdServicio(int idServicio){
	this->idServicio = idServicio;
}

void Consulta::setIdUsuario(int idUsuario){
	this->idUsuario = idUsuario;
}

void Consulta::setConsulta(string consulta){
	this->consulta = consulta;
}

void Consulta::setFechaConsulta(string fechaConsulta){
	this->fechaConsulta = fechaConsulta;
}

void Consulta::setHoraConsulta(string horaConsulta){
	this->horaConsulta = horaConsulta;
}

void Consulta::setRespuesta(string respuesta){
	this->respuesta = respuesta;
}

void Consulta::setFechaRespuesta(string fechaRespuesta){
	this->fechaRespuesta = fechaRespuesta;
}

void Consulta::setHoraRespuesta(string horaRespuesta){
	this->horaRespuesta = horaRespuesta;
}

void Consulta::setOculta(bool oculta){
	this->oculta = oculta;
}

Consulta::~Consulta(){
	
}