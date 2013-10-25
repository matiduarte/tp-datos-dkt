#include "Indice.h" 

Indice::Indice(){
	
}

void Indice::agregarUsuario(Usuario usuario){
	
	this->indiceUsuario->insertarElemento(Convertidor::intToString(usuario.getDni()),usuario.serializar());
	//TODO:tengo que incertar en indice secundario arbol la provincia y el dni
	
	//TODO:tengo que incertar en el indice secunadrio el tipo y el dni
	//Creo que no tengo que hacer nada mas cuando se crea un usuario
}

Indice::~Indice(){
	
}