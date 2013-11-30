#include "Indice.h" 

Indice::Indice(){
}

Indice::Indice(string ruta){
	this->diccionario = new Diccionario();
	
	LectorConfig* pLector = LectorConfig::getLector(rutaConfig);
	string rutaTabla = "tabla.dat";pLector->getValor("pathArchivoTabla");
	string rutaNodos = "nodos.dat";//pLector->getValor("pathArchivoNodos");
	
	//Indices principales
	this->indiceUsuario = new Hash(rutaBaseIndice+"Usuario"+rutaTabla,rutaBaseIndice+"Usuario"+rutaNodos);
	this->indiceServicio = new Hash(rutaBaseIndice+"Servicio"+rutaTabla,rutaBaseIndice+"Servicio"+rutaNodos);
	this->indiceConsulta = new Hash(rutaBaseIndice+"Consulta"+rutaTabla,rutaBaseIndice+"Consulta"+rutaNodos);
	this->indiceCategorias = new Hash(rutaBaseIndice+"Categoria"+rutaTabla,rutaBaseIndice+"Categoria"+rutaNodos);
	this->indicePedidoCotizacion = new Hash(rutaBaseIndice+"PedidoCotizacion"+rutaTabla,rutaBaseIndice+"PedidoCotizacion"+rutaNodos);
	
	//Indices secundarios
	this->indiceUsuarioPorProvincia = new ArbolBMas(rutaBaseIndice+"ArbolUsuarioPorProvincia");
	this->indiceUsuarioPorTipo = new ArbolBMas(rutaBaseIndice+"ArbolUsuarioPorTipo");
	this->indiceServicioPorCategoria = new ArbolBMas(rutaBaseIndice+"ArbolServicioPorCategoria");
	this->indiceServicioPorIdProveedor = new ArbolBMas(rutaBaseIndice+"ArbolServicioPorIdProveedor");
	this->indiceConsultaPorIdServicio = new ArbolBMas(rutaBaseIndice+"ArbolConsultaPorIdServicio");
	this->indiceConsultaPorIdUsuario = new ArbolBMas(rutaBaseIndice+"ArbolConsultaPorIdUsuario");
	this->indiceConsultaPorIdServicioFechaHora = new ArbolBMas(rutaBaseIndice+"ArbolConsultaPorIdServicioFechaHora");
	this->indiceGeneralEntidades = new ArbolBMas(rutaBaseIndice+"ArbolGeneralEntidades");
	this->indiceCategoriaPorNombre = new ArbolBMas(rutaBaseIndice+"ArbolCategoriasPorNombre");
	this->indicePedidoCotizacionPorIdServicio = new ArbolBMas(rutaBaseIndice+"ArbolPedidoCotizacionPorIdServicio");
	this->indicePedidoCotizacionPorIdUsuario = new ArbolBMas(rutaBaseIndice+"ArbolPedidoCotizacionPorIdUsuario");
	
	
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
	this->indiceGeneralEntidades->agregarValor(*(new Clave(claveIndiceGeneralUsuarios)),StringUtil::int2string(usuario->getDni()));
	
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

bool Indice::elimininarUsuario(Usuario* usuario){
	try{
		this->indiceUsuario->elminarElemento(StringUtil::int2string(usuario->getDni()));
		this->indiceUsuarioPorProvincia->borrarValor(*(new Clave(usuario->getProvincia())),StringUtil::int2string(usuario->getDni()));
		this->indiceUsuarioPorTipo->borrarValor(*(new Clave(usuario->getTipo())),StringUtil::int2string(usuario->getDni()));
		this->indiceGeneralEntidades->borrarValor(*(new Clave(claveIndiceGeneralUsuarios)),StringUtil::int2string(usuario->getDni()));
	}catch(Excepcion& e){
		return false;
	}return true;
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
	} catch (ExceptionElementoNoEncontrado &e){
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
	this->indiceServicio->modificarElemento(StringUtil::int2string(servicio->getId()),servicio->serializar());
	
	
	//Agrego la descripcion a terminos relevantes para busquedas
	this->agregarCadenaATerminosRelevantes(servicio->getDescripcion(),StringUtil::int2string(servicio->getId()));
	
	return true;
}

bool Indice::agregarCategoria(Categoria* categoria){
	string catId = this->indiceCategoriaPorNombre->buscarClave(*(new Clave(categoria->getNombre())));
	if(catId != "NO EXISTE"){
		//Si ya existe una categoria con el mismo nombre, no puedo agregar la categoria
		return false;
	}
	try {
		this->indiceCategorias->insertarElemento(StringUtil::int2string(categoria->getId()),categoria->serializar());
	} catch (ExceptionElementoKeyYaIngresado& e){
		return false;
	}

	this->indiceCategoriaPorNombre->agregarValor(*(new Clave(categoria->getNombre())),StringUtil::int2string(categoria->getId()));
	this->indiceGeneralEntidades->agregarValor(*(new Clave(claveIndiceGeneralCategorias)),StringUtil::int2string(categoria->getId()));
	
	return true;
}

bool Indice::agregarCategoriaServicio(Categoria* categoria, Servicio* servicio){
	//Chequeo si existe la categoria
	try {
		this->indiceCategorias->buscarElemento(StringUtil::int2string(categoria->getId()));
	} catch (ExceptionElementoNoEncontrado e){
		return false;
	}
	
	//Chequeo si ya no se agrego la categoria al servicio
	list<string>* idServiciosDeCategoria = this->indiceServicioPorCategoria->elementosConIgualClave(*(new Clave(categoria->getNombre())));
	for (std::list<string>::iterator it = idServiciosDeCategoria->begin(); it != idServiciosDeCategoria->end(); it++){
		if(*it == StringUtil::int2string(servicio->getId())){
			return false;
		}
	}
	
	servicio->setCategoria(categoria);
	this->indiceServicioPorCategoria->agregarValor(*(new Clave(categoria->getNombre())),StringUtil::int2string(servicio->getId()));
	//Tengo que agregar las categorias a la lista invertida. Se usa lista invertida porque pueden ser infinitas categorias
	int posLista = servicio->getPosicionCategorias();
	int nuevaPosicion = this->listaCategoriasPorServicio->modificar(posLista, servicio->serializarCategorias());
	servicio->setPosicionCategorias(nuevaPosicion);
	this->indiceServicio->modificarElemento(StringUtil::int2string(servicio->getId()),servicio->serializar());
	return true;
}

bool Indice::eliminarCategoria(string nombreCategoria){
	string idCat = this->indiceCategoriaPorNombre->buscarClave(*(new Clave(nombreCategoria)));
	if(idCat == "NO EXISTE"){
		//Si no existe la categoria que se esta bucando devuelvo false
		return false;
	}
	
	string catSerializada = this->indiceCategorias->buscarElemento(idCat);
	Categoria* categoria = new Categoria();
	categoria->desSerializar(catSerializada);
	
	this->indiceCategorias->elminarElemento(StringUtil::int2string(categoria->getId()));
	this->indiceCategoriaPorNombre->borrarValor(*(new Clave(categoria->getNombre())),StringUtil::int2string(categoria->getId()));
	this->indiceGeneralEntidades->borrarValor(*(new Clave(claveIndiceGeneralCategorias)),StringUtil::int2string(categoria->getId()));
	
	//Ahora tengo que borrar todas las referencias de servicios a esta categoria
	list<string>* idsServicio = this->indiceServicioPorCategoria->elementosConIgualClave(*(new Clave(categoria->getNombre())));
	for (std::list<string>::iterator it = idsServicio->begin(); it != idsServicio->end(); it++){
		string idServicioActual = *it;
		string servicioSerializado = this->indiceServicio->buscarElemento(idServicioActual);
		Servicio* servicio = new Servicio();
		servicio->desSerializar(servicioSerializado);
		
		int posLista = servicio->getPosicionCategorias();
		servicio->eliminarCategoria(categoria);
		int nuevaPosicion = this->listaCategoriasPorServicio->modificar(posLista, servicio->serializarCategorias());
		servicio->setPosicionCategorias(nuevaPosicion);
		this->indiceServicio->modificarElemento(StringUtil::int2string(servicio->getId()),servicio->serializar());
	}
	return true;
}
void Indice::modificarCategoria(Categoria* categoria){
	string antiguaCatSerializada = this->indiceCategorias->buscarElemento(StringUtil::int2string(categoria->getId()));
	Categoria* antiguaCat = new Categoria();
	antiguaCat->desSerializar(antiguaCatSerializada);
	
	this->indiceCategorias->modificarElemento(StringUtil::int2string(categoria->getId()),categoria->serializar());
	if(antiguaCat->getNombre() != categoria->getNombre()){
		//Si la categoria cambio su nombre tengo que actualizar el indice por nombre
		this->indiceCategoriaPorNombre->borrarValor(*(new Clave(antiguaCat->getNombre())),StringUtil::int2string(antiguaCat->getId()));
		this->indiceCategoriaPorNombre->agregarValor(*(new Clave(categoria->getNombre())),StringUtil::int2string(categoria->getId()));
	}
}

Categoria* Indice::buscarCategoria(string nombreCategoria, bool &error){
	string idCat = this->indiceCategoriaPorNombre->buscarClave(*(new Clave(nombreCategoria)));
	error = false;
	if(idCat == "NO EXISTE"){
		error = true;
		return new Categoria();
	}
	
	string catSerializada = this->indiceCategorias->buscarElemento(idCat);
	Categoria* cat = new Categoria();
	cat->desSerializar(catSerializada);
	return cat;
}


bool Indice::eliminarServicio(Servicio* servicio){
	try{
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
	}catch(Excepcion& e){
		return false;
	}
	return true;
}

void Indice::hidratarCategoriasDeServicio(Servicio* servicio){
	vector<Categoria*> categorias = servicio->getCategorias();
	vector<Categoria*> categoriasResultado;
	for(unsigned int i=0; i<categorias.size();i++){
		Categoria* catActual = categorias.at(i);
		string catSerializada = this->indiceCategorias->buscarElemento(StringUtil::int2string(catActual->getId()));
		catActual->desSerializar(catSerializada);
		categoriasResultado.push_back(catActual);
	}
	servicio->setCategorias(categoriasResultado);
}

vector<Servicio*> Indice::buscarServiciosPorUsuario(Usuario* usuario){
	//Obtengo todos los ids de servicios dado un usuario proveedor
	list<string>* idsServicios = this->indiceServicioPorIdProveedor->elementosConIgualClave(StringUtil::int2string(usuario->getDni()));

	vector<Servicio*> resultadoServicios;
	 for (std::list<string>::iterator it = idsServicios->begin(); it != idsServicios->end(); it++){
		Servicio* ser = new Servicio();
		string servicioSerializado = this->indiceServicio->buscarElemento(*it);
		ser->desSerializar(servicioSerializado);
		int nuevaPos;
		string catsSerializadas = this->listaCategoriasPorServicio->obtener(ser->getPosicionCategorias(),&nuevaPos);
		ser->deserializarCategorias(catsSerializadas);
		this->hidratarCategoriasDeServicio(ser);
		ser->setPosicionCategorias(nuevaPos);
		this->indiceServicio->modificarElemento(StringUtil::int2string(ser->getId()),ser->serializar());
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
					ser->desSerializar(servicioSerializado);
					int nuevaPos;
					string catsSerializadas = this->listaCategoriasPorServicio->obtener(ser->getPosicionCategorias(),&nuevaPos);
					ser->deserializarCategorias(catsSerializadas);
					this->hidratarCategoriasDeServicio(ser);
					ser->setPosicionCategorias(nuevaPos);
					this->indiceServicio->modificarElemento(StringUtil::int2string(ser->getId()),ser->serializar());
					resultadoServicio.push_back(ser);
				}
			}
		}
	}
	
	return resultadoServicio;
}

vector<Servicio*> Indice::buscarServiciosCategoria(Categoria* categoria){
	list<string>* idsServicios = this->indiceServicioPorCategoria->elementosConIgualClave(*(new Clave(categoria->getNombre())));
	vector<Servicio*> resultadoServicios;
	for (std::list<string>::iterator it = idsServicios->begin(); it != idsServicios->end(); it++){
		//Obtengo los ids de servicios
		Servicio* ser = new Servicio();
		string servicioSerializado = this->indiceServicio->buscarElemento(*it);
		ser->desSerializar(servicioSerializado);
		int nuevaPos;
		string catsSerializadas = this->listaCategoriasPorServicio->obtener(ser->getPosicionCategorias(),&nuevaPos);
		ser->deserializarCategorias(catsSerializadas);
		this->hidratarCategoriasDeServicio(ser);
		ser->setPosicionCategorias(nuevaPos);
		this->indiceServicio->modificarElemento(StringUtil::int2string(ser->getId()),ser->serializar());
		resultadoServicios.push_back(ser);
	}
	
	return resultadoServicios;
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

	string antiguaConsultaSerializada = this->indiceConsulta->buscarElemento(StringUtil::int2string(consulta->getId()));
	Consulta* antiguaConsulta = new Consulta();
	antiguaConsulta->desSerializar(antiguaConsultaSerializada);
	//Actualizo el indice de fecha y hora
	string claveString = StringUtil::int2string(antiguaConsulta->getIdServicio()) + separadorCamposClave + antiguaConsulta->getFechaConsulta() + separadorCamposClave + antiguaConsulta->getHoraConsulta();
	Clave* claveArbol = new Clave(claveString);
	this->indiceConsultaPorIdServicioFechaHora->borrarValor(*claveArbol, StringUtil::int2string(antiguaConsulta->getId()));
	
	claveString = StringUtil::int2string(consulta->getIdServicio()) + separadorCamposClave + consulta->getFechaConsulta() + separadorCamposClave + consulta->getHoraConsulta();
	claveArbol = new Clave(claveString);
	this->indiceConsultaPorIdServicioFechaHora->agregarValor(*claveArbol, StringUtil::int2string(consulta->getId()));
	
	//Modifico la consulta
	this->indiceConsulta->modificarElemento(StringUtil::int2string(consulta->getId()),consulta->serializar());
}

vector<string> Indice::parsearConsulta(string consulta){
	vector<string> terminos = StringUtil::split(consulta,' ');
	vector<string> terminosRelevantes;
	
	for(unsigned int i=0; i<terminos.size();i++){
		string terminoActual = terminos.at(i);
		std::transform(terminoActual.begin(), terminoActual.end(), terminoActual.begin(), ::tolower);
		if(!this->diccionario->esStopWord(terminoActual)){
			terminosRelevantes.push_back(terminoActual);
		}
	}
	
	return terminosRelevantes;
}

vector<Consulta*> Indice::buscarConsultasHechasAUsuario(Usuario* usuario){
	//Aca tengo que tener todos los ids de los servicios del provvedor. Asi busco las preguntas de cada servicio
	list<string>* idsServicios  = this->indiceServicioPorIdProveedor->elementosConIgualClave(StringUtil::int2string(usuario->getDni()));

	list<string>* idsConsulta = new list<string>;
	for (std::list<string>::iterator it = idsServicios->begin(); it != idsServicios->end(); it++){
		//Obtengo los ids de consultas dado un servicio
		list<string>* idsActual = this->indiceConsultaPorIdServicio->elementosConIgualClave(*it);
		idsConsulta->insert(idsConsulta->end(), idsActual->begin(), idsActual->end());
	}
	
	vector<Consulta*> resultadoConsultas;
	for (std::list<string>::iterator itCons = idsConsulta->begin(); itCons != idsConsulta->end(); itCons++){
		Consulta* cons = new Consulta();
		string consultaSerializada = this->indiceConsulta->buscarElemento(*itCons);
		cons->desSerializar(consultaSerializada);
		resultadoConsultas.push_back(cons);
	}
	return resultadoConsultas;

}

vector<Consulta*> Indice::buscarConsultasPorServicio(Servicio* servicio){
	list<string>* idsConsulta = this->indiceConsultaPorIdServicio->elementosConIgualClave(StringUtil::int2string(servicio->getId()));
		
	vector<Consulta*> resultadoConsultas;
	for (std::list<string>::iterator itCons = idsConsulta->begin(); itCons != idsConsulta->end(); itCons++){
		Consulta* cons = new Consulta();
		string consultaSerializada = this->indiceConsulta->buscarElemento(*itCons);
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
			idTermino = this->obtenerNuevoId("idTerminoActual");
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
	int nuevoId = StringUtil::str2int(actualValor) +1;
	string nuevoIdString = StringUtil::int2string(nuevoId);
	pLector->setValor(tipoId, nuevoIdString);
	return nuevoIdString;
}

vector<Usuario*> Indice::obtenerTodosLosUsuarios(){
	list<string>* idsUsuarios = this->indiceGeneralEntidades->elementosConIgualClave(*(new Clave(claveIndiceGeneralUsuarios)));
	
	vector<Usuario*> resultadoUsuarios;
	for (std::list<string>::iterator it = idsUsuarios->begin(); it != idsUsuarios->end(); it++){
		Usuario* usuario = new Usuario();
		string usuarioSerializado = this->indiceUsuario->buscarElemento(*it);
		usuario->desSerializar(usuarioSerializado);
		resultadoUsuarios.push_back(usuario);
	}
	
	return resultadoUsuarios;
}


vector<Categoria*> Indice::obtenerTodasLasCategorias(bool &error){
	list<string>* idsCategorias = this->indiceGeneralEntidades->elementosConIgualClave(*(new Clave(claveIndiceGeneralCategorias)));
		
	vector<Categoria*> resultadoCategorias;
	for (std::list<string>::iterator it = idsCategorias->begin(); it != idsCategorias->end(); it++){
		Categoria* cat = new Categoria();
		string categoriaSerializada = this->indiceCategorias->buscarElemento(*it);
		cat->desSerializar(categoriaSerializada);
		resultadoCategorias.push_back(cat);
		error=false;
	}
	
	return resultadoCategorias;
}


bool Indice::agregarPedidoCotizacion(PedidoCotizacion* pedido){
	try {
		this->indicePedidoCotizacion->insertarElemento(StringUtil::int2string(pedido->getId()),pedido->serializar());

	} catch (ExceptionElementoKeyYaIngresado e){
		return false;
	}
	
	this->indicePedidoCotizacionPorIdServicio->agregarValor(*(new Clave(StringUtil::int2string(pedido->getIdServicio()))),StringUtil::int2string(pedido->getId()));
	this->indicePedidoCotizacionPorIdUsuario->agregarValor(StringUtil::int2string(pedido->getIdUsuario()),StringUtil::int2string(pedido->getId()));
	
	return true;
}

vector<PedidoCotizacion*> Indice::buscarPedidosCotizacionHechasAUsuario(Usuario* usuario){
	//Aca tengo que tener todos los ids de los servicios del provvedor. Asi busco las preguntas de cada servicio
	list<string>* idsServicios  = this->indiceServicioPorIdProveedor->elementosConIgualClave(StringUtil::int2string(usuario->getDni()));

	list<string>* idsPedido = new list<string>;
	for (std::list<string>::iterator it = idsServicios->begin(); it != idsServicios->end(); it++){
		//Obtengo los ids de pedidos dado un servicio
		list<string>* idsActual = this->indicePedidoCotizacionPorIdServicio->elementosConIgualClave(*it);
		idsPedido->insert(idsPedido->end(), idsActual->begin(), idsActual->end());
	}
	
	vector<PedidoCotizacion*> resultadoPedidos;
	for (std::list<string>::iterator itPed = idsPedido->begin(); itPed != idsPedido->end(); itPed++){
		PedidoCotizacion* pedido = new PedidoCotizacion();
		string pedidoSerializada = this->indicePedidoCotizacion->buscarElemento(*itPed);
		pedido->desSerializar(pedidoSerializada);
		resultadoPedidos.push_back(pedido);
	}
	return resultadoPedidos;

}

vector<PedidoCotizacion*> Indice::buscarPedidosCotizacionPorServicio(Servicio* servicio){
	list<string>* idsPedido = this->indicePedidoCotizacionPorIdServicio->elementosConIgualClave(StringUtil::int2string(servicio->getId()));
		
	vector<PedidoCotizacion*> resultadoPedidos;
	for (std::list<string>::iterator itPed = idsPedido->begin(); itPed != idsPedido->end(); itPed++){
		PedidoCotizacion* ped = new PedidoCotizacion();
		string pedidoSerializada = this->indicePedidoCotizacion->buscarElemento(*itPed);
		ped->desSerializar(pedidoSerializada);
		resultadoPedidos.push_back(ped);
	}
	return resultadoPedidos;
}


Indice::~Indice(){
	delete(indiceUsuario);
	delete(indiceServicio);
	delete(indiceConsulta);
	delete(indiceCategorias);
	delete(listaCategoriasPorServicio);
	delete(indiceUsuarioPorProvincia);
	delete(indiceUsuarioPorTipo);
	delete(indiceServicioPorCategoria);
	delete(indiceServicioPorIdProveedor);
	delete(indiceConsultaPorIdServicio);
	delete(indiceConsultaPorIdUsuario);
	delete(indiceConsultaPorIdServicioFechaHora);
	delete(indiceGeneralEntidades);
	delete(indiceCategoriaPorNombre);
	delete(indiceOcurrenciasTerminos);
	delete(indiceTerminosId);
	delete(indiceTerminos);
	delete(indicePedidoCotizacion);
	delete(indicePedidoCotizacionPorIdServicio);
	delete(indicePedidoCotizacionPorIdUsuario);
	delete(diccionario);
}
