#include "PedidoCotizacion.h"

PedidoCotizacion::PedidoCotizacion(){

}

string PedidoCotizacion::serializar(){
	string retorno="";
    retorno = StringUtil::int2string(this->id);
    retorno += separadorCamposEntidades;
    retorno += StringUtil::int2string(this->idServicio);
    retorno += separadorCamposEntidades;
    retorno += StringUtil::int2string(this->idUsuario);
    retorno += separadorCamposEntidades;
    retorno += this->pedido;
    retorno += separadorCamposEntidades;
    retorno += this->fechaPedido;
    retorno += separadorCamposEntidades;
    retorno += this->horaPedido;
    return retorno;
}

void PedidoCotizacion::desSerializar(string aDeserealizar){

	vector<string> atributos = StringUtil::split(aDeserealizar,separadorCamposEntidades);
	this->id = StringUtil::str2int(atributos.at(0));
	this->idServicio = StringUtil::str2int(atributos.at(1));
	this->idUsuario = StringUtil::str2int(atributos.at(2));
	this->pedido = atributos.at(3);
	this->fechaPedido = atributos.at(4);
	this->horaPedido = atributos.at(5);
}

int PedidoCotizacion::getId(){
	return this->id;
}

int PedidoCotizacion::getIdServicio(){
	return this->idServicio;
}

int PedidoCotizacion::getIdUsuario(){
	return this->idUsuario;
}

string PedidoCotizacion::getPedido(){
	return this->pedido;
}

string PedidoCotizacion::getFechaPedido(){
	return this->fechaPedido;
}

string PedidoCotizacion::getHoraPedido(){
	return this->horaPedido;
}

void PedidoCotizacion::setId(int id){
	this->id = id;
}

void PedidoCotizacion::setIdServicio(int idServicio){
	this->idServicio = idServicio;
}

void PedidoCotizacion::setIdUsuario(int idUsuario){
	this->idUsuario = idUsuario;
}

void PedidoCotizacion::setPedido(string consulta){
	this->pedido = pedido;
}

void PedidoCotizacion::setFechaPedido(string fecha){
	this->fechaPedido = fecha;
}

void PedidoCotizacion::setHoraPedido(string hora){
	this->horaPedido = hora;
}

PedidoCotizacion::~PedidoCotizacion(){
	
}
