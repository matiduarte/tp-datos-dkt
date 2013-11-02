#include "Indice.h" 

Indice::Indice(){
	LectorConfig* pLector = LectorConfig::getLector(rutaConfig_ArchivoBloque);
	string rutaTabla = pLector->getValor("pathArchivoTabla");
	string rutaNodos = pLector->getValor("pathArchivoNodos");
	
	//Indices principales
	this->indiceUsuario = new Hash(rutaBaseIndice+"Usuario"+rutaTabla,rutaBaseIndice+"Usuario"+rutaNodos);
	this->indiceServicio = new Hash(rutaBaseIndice+"Servicio"+rutaTabla,rutaBaseIndice+"Servicio"+rutaNodos);
	this->indiceConsulta = new Hash(rutaBaseIndice+"Consulta"+rutaTabla,rutaBaseIndice+"Consulta"+rutaNodos);
	this->indiceCategorias = new Hash(rutaBaseIndice+"Categoria"+rutaTabla,rutaBaseIndice+"Categoria"+rutaNodos);
	
	//Indices secundarios
	this->indiceUsuarioPorProvincia = new ArbolBMas(rutaBaseIndice+"ArbolUsuarioPorProvincia");
	this->indiceUsuarioPorTipo = new ArbolBMas(rutaBaseIndice+"ArbolUsuarioPorTipo");
	this->indiceServicioPorCategoria = new ArbolBMas(rutaBaseIndice+"ArbolServicioPorCategoria");
	this->indiceConsultaPorIdServicioIdUsuario = new ArbolBMas(rutaBaseIndice+"ArbolConsultaPorIdServicioIdUsuario");
	this->indiceConsultaPorIdServicioFechaHora = new ArbolBMas(rutaBaseIndice+"ArbolConsultaPorIdServicioFechaHora");
	
	//Listas
	this->listaCategoriasPorServicio = new ListaInvertida(rutaBaseIndice+"ListaCategoriasPorServicio");
	this->indiceOcurrenciasTerminos = new ListaInvertida(rutaBaseIndice+"ListaOcurrenciasPorTermino");
}

bool Indice::agregarUsuario(Usuario* usuario){
	try {
		this->indiceUsuario->insertarElemento(StringUtil::int2string(usuario->getDni()),usuario->serializar());
	} catch (ExceptionElementoKeyYaIngresado e){
			return false;
	}
	this->indiceUsuarioPorProvincia->agregarValor(*(new Clave(usuario->getProvincia())),StringUtil::int2string(usuario->getDni()));
	this->indiceUsuarioPorTipo->agregarValor(*(new Clave(usuario->getTipo())),StringUtil::int2string(usuario->getDni()));
	//Creo que no tengo que hacer nada mas cuando se crea un usuario
	
	return true;
}

void Indice::modificarUsuario(Usuario* usuario){
	//Obtengo el usuario anterior
	Usuario* usuarioAnterior = new Usuario();
	string aDeserealizar = this->indiceUsuario->buscarElemento(StringUtil::int2string(usuario->getDni()));
	usuarioAnterior->desSerializar(aDeserealizar);
	
	this->indiceUsuario->modificarElemento(StringUtil::int2string(usuario->getDni()),usuario->serializar());
	
	if(usuarioAnterior->getProvincia() != usuario->getProvincia()){
		//Como cambio la provincia tengo el elminar el antiguo registros del indice secundario y agrego uno nuevo
		this->indiceUsuarioPorProvincia->borrarValor(*(new Clave(usuario->getProvincia())),StringUtil::int2string(usuario->getDni()));
		this->indiceUsuarioPorProvincia->agregarValor(*(new Clave(usuario->getProvincia())),StringUtil::int2string(usuario->getDni()));

	}
	if(usuarioAnterior->getTipo() != usuario->getTipo()){
		//Como cambio el tipo tengo el elminar el antiguo registros del indice secundario y agrego uno nuevo
		this->indiceUsuarioPorTipo->borrarValor(*(new Clave(usuario->getTipo())),StringUtil::int2string(usuario->getDni()));
		this->indiceUsuarioPorTipo->agregarValor(*(new Clave(usuario->getTipo())),StringUtil::int2string(usuario->getDni()));

	}
}

void Indice::elimininarUsuario(Usuario* usuario){
	this->indiceUsuario->elminarElemento(StringUtil::int2string(usuario->getDni()));
	this->indiceUsuarioPorProvincia->borrarValor(*(new Clave(usuario->getProvincia())),StringUtil::int2string(usuario->getDni()));
	this->indiceUsuarioPorTipo->borrarValor(*(new Clave(usuario->getTipo())),StringUtil::int2string(usuario->getDni()));
}

bool Indice::agregarServicio(Servicio* servicio){
	try {
		this->indiceServicio->insertarElemento(StringUtil::int2string(servicio->getId()),servicio->serializar());
	} catch (ExceptionElementoKeyYaIngresado e){
		return false;
	}
	
	vector<Categoria*> categorias = servicio->getCategorias();
	
	//Agrego al indice secundario referencias por cada categoria
	for(unsigned int i=0; i < categorias.size();i++){
		Categoria* catActual = categorias.at(i);
		this->indiceServicioPorCategoria->agregarValor(*(new Clave(catActual->getNombre())),StringUtil::int2string(servicio->getId()));
	}
	
	//Agrego las categorias a la lista invertida
	int nuevaPosicion = this->listaCategoriasPorServicio->insertar(StringUtil::int2string(servicio->getId()), servicio->serializarCategorias());
	servicio->setPosicionCategorias(nuevaPosicion);
	
	return true;
}

void Indice::agregarCategoriaServicio(Categoria* categoria, Servicio* servicio){
	servicio->setCategoria(categoria);
	this->indiceServicioPorCategoria->agregarValor(*(new Clave(categoria->getNombre())),StringUtil::int2string(servicio->getId()));
	//Tengo que agregar las categorias a la lista invertida. Se usa lista invertida porque pueden ser infinitas categorias
	int posLista = servicio->getPosicionCategorias();
	int nuevaPosicion = this->listaCategoriasPorServicio->modificar(posLista, servicio->serializarCategorias());
	servicio->setPosicionCategorias(nuevaPosicion);
}

void Indice::eliminarServicio(Servicio* servicio){
	this->indiceServicio->elminarElemento(StringUtil::int2string(servicio->getId()));
	vector<Categoria*> categorias = servicio->getCategorias();
	
	//Elimino al indice secundario referencias de cada categoria
	for(unsigned int i=0; i < categorias.size();i++){
		Categoria* catActual = categorias.at(i);
		this->indiceServicioPorCategoria->borrarValor(*(new Clave(catActual->getNombre())),StringUtil::int2string(servicio->getId()));
	}
	
	//Elimino la lista invertida
	this->listaCategoriasPorServicio->borrar(servicio->getPosicionCategorias());
}

bool Indice::agregarConsulta(Consulta* consulta){
	try {
		this->indiceConsulta->insertarElemento(StringUtil::int2string(consulta->getId()),consulta->serializar());
	} catch (ExceptionElementoKeyYaIngresado e){
		return false;
	}
	
	//La clave se forma con un string con el idServicio y idUsuario -> idServicio+ separador + idUsuario
	string claveString = StringUtil::int2string(consulta->getIdServicio()) + separadorCamposClave + StringUtil::int2string(consulta->getIdUsuario()); 
	Clave* claveArbol = new Clave(claveString);
	this->indiceConsultaPorIdServicioIdUsuario->agregarValor(*claveArbol,StringUtil::int2string(consulta->getId()));
	
	//Esta clave se forma de manera analoga a la anterior
	string claveString2 = StringUtil::int2string(consulta->getIdServicio()) + separadorCamposClave + consulta->getFechaConsulta() + separadorCamposClave + consulta->getHoraConsulta();
	Clave* claveArbol2 = new Clave(claveString2);
	this->indiceConsultaPorIdServicioFechaHora->agregarValor(*claveArbol2, StringUtil::int2string(consulta->getId()));
	return true;
}

void Indice::modificarConsulta(Consulta* consulta){
	
}

vector<string> Indice::parsearConsulta(string consulta){
	Diccionario* dic = new Diccionario();
	vector<string> terminos = StringUtil::split(consulta,' ');
	vector<string> terminosRelevantes;
	
	for(unsigned int i=0; i<terminos.size();i++){
		if(!dic->esStopWord(terminos.at(i))){
			terminosRelevantes.push_back(terminos.at(i));
		}
	}
	
	return terminosRelevantes;
}

Indice::~Indice(){
	
}