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

void Indice::elimininarUsuario(Usuario* usuario){
	this->indiceUsuario->elminarElemento(Convertidor::intToString(usuario->getDni()));
	this->indiceUsuarioPorProvincia->borrarValor(*(new Clave(usuario->getProvincia())),Convertidor::intToString(usuario->getDni()));
	this->indiceUsuarioPorTipo->borrarValor(*(new Clave(usuario->getTipo())),Convertidor::intToString(usuario->getDni()));
}

void Indice::agregarServicio(Servicio* servicio){
	this->indiceServicio->insertarElemento(Convertidor::intToString(servicio->getId()),servicio->serializar());
	vector<Categoria*> categorias = servicio->getCategorias();
	
	//Agrego al indice secundario referencias por cada categoria
	for(unsigned int i=0; i < categorias.size();i++){
		Categoria* catActual = categorias.at(i);
		this->indiceServicioPorCategoria->agregarValor(*(new Clave(catActual->getNombre())),Convertidor::intToString(servicio->getId()));
	}
}

void Indice::agregarCategoriaServicio(Categoria* categoria, Servicio* servicio){
	this->indiceServicioPorCategoria->borrarValor(*(new Clave(categoria->getNombre())),Convertidor::intToString(servicio->getId()));
}

void Indice::eliminarServicio(Servicio* servicio){
	this->indiceUsuario->elminarElemento(Convertidor::intToString(servicio->getId()));
	vector<Categoria*> categorias = servicio->getCategorias();
	
	//Elimino al indice secundario referencias de cada categoria
	for(unsigned int i=0; i < categorias.size();i++){
		Categoria* catActual = categorias.at(i);
		this->indiceServicioPorCategoria->borrarValor(*(new Clave(catActual->getNombre())),Convertidor::intToString(servicio->getId()));
	}
}

void Indice::agregarConsulta(Consulta* consulta){
	this->indiceConsulta->insertarElemento(Convertidor::intToString(consulta->getId()),consulta->serializar());
	//La clave se forma con un string con el idServicio y idUsuario -> idServicio+ separador + idUsuario
	string claveString = Convertidor::intToString(consulta->getIdServicio()) + separadorCamposClave + Convertidor::intToString(consulta->getIdUsuario()); 
	Clave* claveArbol = new Clave(claveString);
	this->indiceConsultaPorIdServicioIdUsuario->agregarValor(*claveArbol,consulta->serializar());
}

void Indice::modificarConsulta(Consulta* consulta){
	
}

Indice::~Indice(){
	
}