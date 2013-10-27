#include "Indice.h" 

Indice::Indice(){
	
}

void Indice::agregarUsuario(Usuario* usuario){
	this->indiceUsuario->insertarElemento(Convertidor::intToString(usuario->getDni()),usuario->serializar());
	this->indiceUsuarioPorProvincia->agregarValor(*(new Clave(usuario->getProvincia())),Convertidor::intToString(usuario->getDni()));
	this->indiceUsuarioPorTipo->agregarValor(*(new Clave(usuario->getTipo())),Convertidor::intToString(usuario->getDni()));
	//Creo que no tengo que hacer nada mas cuando se crea un usuario
}

void Indice::modificarUsuario(Usuario* usuario){
	//Obtengo el usuario anterior
	Usuario* usuarioAnterior = new Usuario();
	string aDeserealizar = this->indiceUsuario->buscarElemento(Convertidor::intToString(usuario->getDni()));
	usuarioAnterior->desSerializar(aDeserealizar);
	
	this->indiceUsuario->modificarElemento(Convertidor::intToString(usuario->getDni()),usuario->serializar());
	
	if(usuarioAnterior->getProvincia() != usuario->getProvincia()){
		//Como cambio la provincia tengo el elminar el antiguo registros del indice secundario y agrego uno nuevo
		this->indiceUsuarioPorProvincia->borrarValor(*(new Clave(usuario->getProvincia())),Convertidor::intToString(usuario->getDni()));
		this->indiceUsuarioPorProvincia->agregarValor(*(new Clave(usuario->getProvincia())),Convertidor::intToString(usuario->getDni()));

	}
	if(usuarioAnterior->getTipo() != usuario->getTipo()){
		//Como cambio el tipo tengo el elminar el antiguo registros del indice secundario y agrego uno nuevo
		this->indiceUsuarioPorTipo->borrarValor(*(new Clave(usuario->getTipo())),Convertidor::intToString(usuario->getDni()));
		this->indiceUsuarioPorTipo->agregarValor(*(new Clave(usuario->getTipo())),Convertidor::intToString(usuario->getDni()));

	}
}

Indice::~Indice(){
	
}