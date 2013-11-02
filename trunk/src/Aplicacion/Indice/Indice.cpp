#include "Indice.h" 

Indice::Indice(){
	LectorConfig* pLector = LectorConfig::getLector(rutaConfig);
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
	this->indiceServicioPorIdProveedor = new ArbolBMas(rutaBaseIndice+"ArbolServicioPorIdProveedor");
	this->indiceConsultaPorIdServicio = new ArbolBMas(rutaBaseIndice+"ArbolConsultaPorIdServicio");
	this->indiceConsultaPorIdUsuario = new ArbolBMas(rutaBaseIndice+"ArbolConsultaPorIdUsuario");
	this->indiceConsultaPorIdServicioFechaHora = new ArbolBMas(rutaBaseIndice+"ArbolConsultaPorIdServicioFechaHora");
	
	//Listas
	this->listaCategoriasPorServicio = new ListaInvertida(rutaBaseIndice+"ListaCategoriasPorServicio");
	this->indiceOcurrenciasTerminos = new ListaInvertida(rutaBaseIndice+"ListaOcurrenciasPorTermino");

	
	this->indiceTerminosId = new ArbolBMas(rutaBaseIndice+"ArbolTerminosId");
	this->indiceTerminos = new Hash(rutaBaseIndice+"Terminos"+rutaTabla,rutaBaseIndice+"Terminos"+rutaNodos);;
}

bool Indice::agregarUsuario(Usuario* usuario){
	try {
		this->indiceUsuario->insertarElemento(StringUtil::int2string(usuario->getDni()),usuario->serializar());
	} catch (ExceptionElementoKeyYaIngresado& e){
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


Usuario* Indice::buscarUsuario(string dni, string contrasena, bool &error){
	
	try{
		string usuarioSerializado = this->indiceUsuario->buscarElemento(dni);
		Usuario* usuario = new Usuario();
		usuario->desSerializar(usuarioSerializado);
		if(usuario->getContrasena() == contrasena){
			//Contrasena correcta
			error = false;
			return usuario;
		}
		//Contrasena incorrecta. Devuelvo usuario vacio
		error = true;
		return new Usuario();
	} catch (ExceptionElementoNoEncontrado e){
		error = true;
		return new Usuario();
	}
	
}

bool Indice::agregarServicio(Servicio* servicio){
	try {
		this->indiceServicio->insertarElemento(StringUtil::int2string(servicio->getId()),servicio->serializar());
	} catch (ExceptionElementoKeyYaIngresado e){
		return false;
	}
	
	this->indiceServicioPorIdProveedor->agregarValor(*(new Clave(StringUtil::int2string(servicio->getIdProveedor()))),StringUtil::int2string(servicio->getId()));
	
	vector<Categoria*> categorias = servicio->getCategorias();
	
	//Agrego al indice secundario referencias por cada categoria
	for(unsigned int i=0; i < categorias.size();i++){
		Categoria* catActual = categorias.at(i);
		this->indiceServicioPorCategoria->agregarValor(*(new Clave(catActual->getNombre())),StringUtil::int2string(servicio->getId()));
	}
	
	//Agrego las categorias a la lista invertida
	int nuevaPosicion = this->listaCategoriasPorServicio->insertar(StringUtil::int2string(servicio->getId()), servicio->serializarCategorias());
	servicio->setPosicionCategorias(nuevaPosicion);
	
	//Agrego la descripcion a terminos relevantes para busquedas
	this->agregarCadenaATerminosRelevantes(servicio->getDescripcion(),StringUtil::int2string(servicio->getId()));
	
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
	
	this->indiceServicioPorIdProveedor->borrarValor(*(new Clave(StringUtil::int2string(servicio->getIdProveedor()))),StringUtil::int2string(servicio->getId()));
	
	//Elimino al indice secundario referencias de cada categoria
	for(unsigned int i=0; i < categorias.size();i++){
		Categoria* catActual = categorias.at(i);
		this->indiceServicioPorCategoria->borrarValor(*(new Clave(catActual->getNombre())),StringUtil::int2string(servicio->getId()));
	}
	
	//Elimino la lista invertida
	this->listaCategoriasPorServicio->borrar(servicio->getPosicionCategorias());
}

vector<Servicio*> Indice::buscarServiciosPorUsuario(Usuario* usuario){
	string valor = this->indiceServicioPorIdProveedor->buscarClave(StringUtil::int2string(usuario->getDni()));
	//Obtengo todos los ids de servicios dado un usuario proveedor
	vector<string> idsServicios;
	vector<Servicio*> resultadoServicios;
	for(unsigned int i=0; i<idsServicios.size();i++){
		Servicio* ser = new Servicio();
		string servicioSerializado = this->indiceServicio->buscarElemento(idsServicios.at(i));
		ser->desSerializar(servicioSerializado);
		resultadoServicios.push_back(ser);
	}
	return resultadoServicios;
}

vector<Servicio*> Indice::buscarServiciosPorPalabrasClave(string query){
	vector<string> terminosRelevantes = this->parsearConsulta(query);
	vector<Servicio*> resultadoServicio;
	for(unsigned int i=0;i<terminosRelevantes.size();i++){
		string terminoActual = terminosRelevantes.at(i);
		string idTermino = this->indiceTerminosId->buscarClave(*(new Clave(terminoActual)));
		if(idTermino != "NO EXISTE"){
			//Encontre el termino en el arbol, quiere decir que fue indexado
			string posLista = this->indiceTerminos->buscarElemento(idTermino);
			int nuevaPosicion;
			//Con la posicion de la lista busco en la lista invertida para obtener los servicios asociados a la palabra
			string listaSerializada = this->indiceOcurrenciasTerminos->obtener(StringUtil::str2int(posLista),&nuevaPosicion);
			//Como cada vez que busco en la listaInvertida se modifica la posicion tengo que actualizar
			this->indiceTerminos->modificarElemento(idTermino,StringUtil::int2string(nuevaPosicion));
			vector<string> idsServicio = StringUtil::split(listaSerializada,separadorCamposEntidades);
			
			//Por cada termino agrego los servicios en los que aparece
			for(unsigned int j=0;j<idsServicio.size();j++){
				if(StringUtil::str2int(idsServicio.at(i)) > 0){
					Servicio* ser = new Servicio();
					string servicioSerializado = this->indiceServicio->buscarElemento(idsServicio.at(j));
					ser->deserializarCategorias(servicioSerializado);
					resultadoServicio.push_back(ser);
				}
			}
		}
	}
	
	return resultadoServicio;
}


bool Indice::agregarConsulta(Consulta* consulta){
	try {
		this->indiceConsulta->insertarElemento(StringUtil::int2string(consulta->getId()),consulta->serializar());
	} catch (ExceptionElementoKeyYaIngresado e){
		return false;
	}
	
	this->indiceConsultaPorIdServicio->agregarValor(*(new Clave(StringUtil::int2string(consulta->getIdServicio()))),StringUtil::int2string(consulta->getId()));
	this->indiceConsultaPorIdUsuario->agregarValor(StringUtil::int2string(consulta->getIdUsuario()),StringUtil::int2string(consulta->getId()));
	
	//Esta clave se forma de manera analoga a la anterior
	string claveString2 = StringUtil::int2string(consulta->getIdServicio()) + separadorCamposClave + consulta->getFechaConsulta() + separadorCamposClave + consulta->getHoraConsulta();
	Clave* claveArbol2 = new Clave(claveString2);
	this->indiceConsultaPorIdServicioFechaHora->agregarValor(*claveArbol2, StringUtil::int2string(consulta->getId()));
	
	//Agrego la consulta a terminos relevantes para busquedas
	this->agregarCadenaATerminosRelevantes(consulta->getConsulta(),StringUtil::int2string(consulta->getIdServicio()));
	
	
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

vector<Consulta*> Indice::buscarConsultasHechasAUsuario(Usuario* usuario){
	string valor = this->indiceServicioPorIdProveedor->buscarClave(StringUtil::int2string(usuario->getDni()));
	//Aca tengo que tener todos los ids de los servicios del provvedor. Asi busco las preguntas de cada servicio
	vector<string> idsServicios;
	vector<string> idsConsulta;
	for(unsigned int i=0; i< idsServicios.size();i++){
		string valor2 = this->indiceConsultaPorIdServicio->buscarClave(idsServicios.at(i));
		//Obtengo los ids de consultas dado un servicio. Y lo agrego al vector idsConsulta
	}
	
	vector<Consulta*> resultadoConsultas;
	for(unsigned int j=0; j < idsConsulta.size();j++){
		Consulta* cons = new Consulta();
		string consultaSerializada = this->indiceConsulta->buscarElemento(idsServicios.at(j));
		cons->desSerializar(consultaSerializada);
		resultadoConsultas.push_back(cons);
	}
	return resultadoConsultas;

}

void Indice::agregarCadenaATerminosRelevantes(string cadena, string idServicio){
	vector<string> terminosRelevantes = this->parsearConsulta(cadena);
	int posicionLista;
	for(unsigned int i=0;i<terminosRelevantes.size();i++){
		string terminoActual = terminosRelevantes.at(i);
		//El valor representa el id del termino para buscar luego en el hash
		string idTermino = this->indiceTerminosId->buscarClave(*(new Clave(terminoActual)));
		string lista = "";
		if(idTermino == "NO EXISTE"){
			//Tengo que agregar la palabra porque no esta en el indice
			string idTermino = this->obtenerNuevoId("idTerminoActual");
			this->indiceTerminosId->agregarValor(*(new Clave(terminoActual)),idTermino);
			this->indiceTerminos->insertarElemento(idTermino,"");
			lista += idServicio + separadorCamposEntidades;
			posicionLista = this->indiceOcurrenciasTerminos->insertar(terminoActual,lista);
		}else{
			string posLista = this->indiceTerminos->buscarElemento(idTermino);
			int nuevaPosicion;
			//El valor devuelto por el hash representa la posicion de la lista
			lista = this->indiceOcurrenciasTerminos->obtener(StringUtil::str2int(posLista),&nuevaPosicion);
			
			//Tengo que agregar la nueva referencia al servicio a la lista si ya no esta el servicio
			vector<string> idsServicioParaTermino = StringUtil::split(lista,separadorCamposEntidades);
			if(!(std::find(idsServicioParaTermino.begin(), idsServicioParaTermino.end(), idServicio) != idsServicioParaTermino.end())) {
				//Solo lo agrego si no estaba antes
				lista += idServicio + separadorCamposEntidades;
				posicionLista = this->indiceOcurrenciasTerminos->modificar(nuevaPosicion,lista);
			}else{
				posicionLista = nuevaPosicion;
			}
		}
		this->indiceTerminos->modificarElemento(idTermino,StringUtil::int2string(posicionLista));
	}
}

string Indice::obtenerNuevoId(string tipoId){
	LectorConfig* pLector = LectorConfig::getLector(rutaConfig);
	string actualValor = pLector->getValor(tipoId);
	int nuevoId = StringUtil::str2int(actualValor) + 1;
	string nuevoIdString = StringUtil::int2string(nuevoId);
	pLector->setValor(tipoId,nuevoIdString);
	return nuevoIdString;
}

Indice::~Indice(){
	
}
