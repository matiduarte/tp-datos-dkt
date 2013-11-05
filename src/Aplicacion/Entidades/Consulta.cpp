#include "Consulta.h"

Consulta::Consulta(){
	
	LectorConfig* lector = LectorConfig::getLector(rutaConfig);
	int identificador = lector->stringToInt(lector->getValor("idUltimaConsulta"));
	identificador++;
	lector->setValor("idUltimaConsulta", lector->intToString(identificador));

	this->id = identificador;
	this->fechaConsulta = FechaYHora::setFechaAAAAMMDD();
	this->horaConsulta = FechaYHora::setHoraHHMM();
}

string Consulta::serializar(){
	string retorno;
    retorno = StringUtil::int2string(this->id);
    retorno += separadorCamposEntidades;
    retorno += StringUtil::int2string(this->idServicio);
    retorno += separadorCamposEntidades;
    retorno += StringUtil::int2string(this->idUsuario);
    retorno += separadorCamposEntidades;
    retorno += this->consulta;
    retorno += separadorCamposEntidades;
    retorno += this->fechaConsulta;
    retorno += separadorCamposEntidades;
    retorno += this->horaConsulta;
    retorno += this->respuesta;
    retorno += separadorCamposEntidades;
    retorno += this->fechaRespuesta;
    retorno += separadorCamposEntidades;
    retorno += this->horaRespuesta;
    retorno += separadorCamposEntidades;
    if(this->oculta){
    	retorno += "1";
    }else{
    	retorno += "0";
    }
    
    return retorno;
}

void Consulta::desSerializar(string aDeserealizar){
	vector<string> atributos = StringUtil::split(aDeserealizar,separadorCamposEntidades);
	this->id = StringUtil::str2int(atributos.at(0));
	this->idServicio = StringUtil::str2int(atributos.at(0));
	this->idUsuario = StringUtil::str2int(atributos.at(0));
	this->consulta = atributos.at(1);
	this->fechaConsulta = atributos.at(2);
	this->horaConsulta = atributos.at(3);
	this->respuesta = atributos.at(4);
	this->fechaRespuesta = atributos.at(5);
	this->horaRespuesta = atributos.at(6);
	this->oculta = false;
	string oculta = atributos.at(7);
	if(oculta == "1"){
		this->oculta = true;
	}
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

void Consulta::setIdServicio(int idServicio){
	this->idServicio = idServicio;
}

void Consulta::setIdUsuario(int idUsuario){
	this->idUsuario = idUsuario;
}

void Consulta::setConsulta(string consulta){
	this->consulta = consulta;
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
