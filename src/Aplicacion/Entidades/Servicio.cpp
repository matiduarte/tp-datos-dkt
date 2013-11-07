#include "Servicio.h"

Servicio::Servicio(){
	
}

string Servicio::serializar(){
	string retorno;
    retorno = StringUtil::int2string(this->id);
    retorno += separadorCamposEntidades;
    retorno += StringUtil::int2string(this->idProveedor);;
    retorno += separadorCamposEntidades;
    retorno += this->nombre;
    retorno += separadorCamposEntidades;
    retorno += this->descripcion;
    retorno += separadorCamposEntidades;
    retorno += this->tipo;
    retorno += separadorCamposEntidades;
    
    //Tengo el puntero a la lista invertida de categorias
    retorno += StringUtil::int2string(this->posicionCategorias);
    return retorno;
}

void Servicio::desSerializar(string aDeserealizar){
	vector<string> atributos = StringUtil::split(aDeserealizar,separadorCamposEntidades);
	this->id = StringUtil::str2int(atributos.at(0));
	this->idProveedor = StringUtil::str2int(atributos.at(1));
	this->nombre = atributos.at(2);
	this->descripcion = atributos.at(3);
	this->tipo = atributos.at(4);
	this->posicionCategorias = StringUtil::str2int(atributos.at(5));
}

string Servicio::serializarCategorias(){
	string retorno;
	for(unsigned int i=0; i<this->categorias.size();i++){
    	retorno += StringUtil::int2string(this->categorias.at(i)->getId());
    	retorno += separadorCamposEntidades;
    }
	
    return retorno;
}

void Servicio::deserializarCategorias(string aDeserializar){
	vector<string> catIds = StringUtil::split(aDeserializar,separadorCamposEntidades);
	for(unsigned int i=0; i<catIds.size();i++){
		Categoria* catAct = new Categoria();
		catAct->setId(StringUtil::str2int(catIds.at(i)));
		if(catAct->getId() > 0){;
			this->setCategoria(catAct);
		}
	}
}

void Servicio::eliminarCategoria(Categoria* categoria){
	for(unsigned int=0; i<this->categorias.size();i++){
		if(this->categorias.at(i)->getId() == categoria->getId()){
			//Es la categoria que tengo que eliminar
			this->categorias.erase(this->categorias.begin()+i);
		}
	}
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

int Servicio::getPosicionCategorias(){
	return this->posicionCategorias;
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

void Servicio::setPosicionCategorias(int posicion){
	this->posicionCategorias = posicion;
}

void Servicio::setCategoria(Categoria* categoria){
	this->categorias.push_back(categoria);
}

void Servicio::setCategorias(vector<Categoria*> categorias){
	this->categorias = categorias;
}

Servicio::~Servicio(){
	
}
