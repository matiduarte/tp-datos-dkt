/*
 * Programa.cpp
 *
 *  Created on: 31/10/2013
 *      Author: marian
 */

#include "Programa.h"

Programa::Programa(){

	lector = LectorConfig::getLector(rutaConfig);
	indice = new Indice("");
	system("clear");
}

Programa::~Programa(){
	delete indice;
	delete lector;
}

void Programa::ejecutar(){

	estadoPrograma estado = MENU_PRINCIPAL;
	Usuario* usuario = new Usuario();
	vector<Servicio*> resultados;
	emitirInformacion();

	while(estado != TERMINAR){

		switch(estado){

		case MENU_PRINCIPAL:	estado = menuPrincipal();								break;
		case RECUPERAR_PASS:	estado = recuperacion();								break;
		case REGISTRO_U:		estado = altaUsuario("U");								break;
		case REGISTRO_A:		estado = altaUsuario("A");								break;
		case BAJA_ADMIN:		estado = bajaAdmin(usuario);							break;
		case CAMBIAR_DATOS:		estado = modificarDatosUsuario(usuario);				break;
		case INICIAR_SESION:	estado = iniciarSesion(usuario);						break;
		case OPCIONES_USUARIO:	estado = menuOpcionesUsuario(usuario);					break;
		case CONSULTA_SERVICIO: estado = consultarServicio(resultados);					break;
		case RESULTADOS:		estado = emitirResultadoBusqueda(resultados, usuario);	break;
		case PUBLICAR:			estado = publicarServicio(usuario);						break;
		case RESPONDER:			estado = responderPregunta();							break;
		case BAJA_PRODUCTO:		estado = bajaProducto();								break;
		case VER_USUARIOS:		estado = listadoUsuarios();								break;
		case REGISTRO_CAT:		estado = generarNuevasCategorias();						break;
		case BAJA_CAT:			estado = bajaCategoria();								break;
		default:																		break;
		}

		system("clear");
	}
}

estadoPrograma Programa::menuPrincipal(){

	estadoPrograma estado= MENU_PRINCIPAL;
	int cantidadOpciones = 4;

	gotoXY(0, 0);	cout<<"MENU PRINCIPAL:";
	gotoXY(0, 2);	cout<<"1 - Iniciar sesion.";
	gotoXY(0, 3);	cout<<"2 - Registrarse.";
	gotoXY(0, 4);	cout<<"3 - Recuperar contraseña.";
	gotoXY(0, 5);	cout<<"4 - Salir del programa.";

	int opcion = leerOpcion(cantidadOpciones, 5);
	if(opcion == 1) estado = INICIAR_SESION;
	if(opcion == 2) estado = REGISTRO_U;
	if(opcion == 3) estado = RECUPERAR_PASS;
	if(opcion == 4) estado = TERMINAR;

	return estado;
}

estadoPrograma Programa::recuperacion(){
	string nombre="";
	gotoXY(0,0);	cout<<"RECUPERAR CONTRASEÑA:";
	gotoXY(0,2);	cout<<"Usuario: ";	leer(nombre);
	gotoXY(0,9);	cout<<"Se ha enviado un mail con su contraseña a su casilla de correo.";

	return MENU_PRINCIPAL;
}

estadoPrograma Programa::altaUsuario(string tipo){

	estadoPrograma estado = MENU_PRINCIPAL;
	Usuario* nuevoUsuario = new Usuario();
	bool usuarioAgregado = false;
	string nombre, apellido, DNI, provincia, mail, contrasena;
	string respuestaMail;
	int posY=2;

	gotoXY(0, 0);		cout<<"REGISTRO: nuevo "<<imprimirTipoDeUsuario(tipo)<<".";
	nuevoUsuario->setTipo(tipo);
	gotoXY(0, posY);	cout<<"Nombre:     "; 	leer(nombre); 	nuevoUsuario->setNombre(nombre);		posY++;
	gotoXY(0, posY);	cout<<"Apellido:   ";	leer(apellido);	nuevoUsuario->setApellido(apellido);	posY++;
	do{
		gotoXY(0, posY); 	cout<<"                      ";
		gotoXY(0, posY);	cout<<"DNI:        "; leer(DNI);
	}while((atoi(DNI.c_str()) <= 0) || (tieneEspacios(DNI)));

	nuevoUsuario->setDni(atoi(DNI.c_str()));posY++;

	gotoXY(0, posY); 	cout<<"Provincia:  ";	leer(provincia);	nuevoUsuario->setProvincia(provincia);	posY++;

	bool otroMail = true;
	//pido hasta 3 mails
	for(int i=1; (i<4)&&(otroMail); i++){
	gotoXY(0, posY);	cout<<"e-Mail   "<<i<<": ";	leer(mail); 		nuevoUsuario->setEmailEnPosicion(mail,i-1);	posY++;
		if(i<3){
			do{
				gotoXY(0, posY+1); cout<<"Desea agregar otra direccion de e-Mail? (s/n) ";
				leer(respuestaMail);
			}while((respuestaMail != "s")&&(respuestaMail != "n"));

			gotoXY(0, posY+1); cout<<"                                                   "; //para borrar la pregunta
			if(respuestaMail == "n") otroMail = false;
			}
	}

	gotoXY(0, posY);	cout<<"Contraseña: ";	leer(contrasena);	nuevoUsuario->setContrasena(contrasena);

	//intento agregar el usuario al indice
	usuarioAgregado = indice->agregarUsuario(nuevoUsuario);

	gotoXY(0, posY+2);
	if(usuarioAgregado)	cout<<"Usuario registrado exitosamente!";
	else cout<<"El usuario ya se encuentra registrado en el sistema.";

	//si se registra un administrador, lo registró otra admin por lo que se va al menu de administracion
	if(tipo == "A") estado = OPCIONES_USUARIO;
	return estado;
}

string Programa::modificar(string queCosa, string valorActual, int posicionDato){

	string respuesta, datoNuevo;
	do{
		gotoXY(0, 12);
		cout<<"Desea modificar "<<queCosa<<"(s/n)"; leer(respuesta);
	}while((respuesta != "s")&&(respuesta != "n"));

	if(respuesta == "n") datoNuevo = valorActual;
	if(respuesta == "s"){
		gotoXY(12, posicionDato); cout<<"                                    "; //borro lo que haya
		gotoXY(12, posicionDato);
		leer(datoNuevo);
	}
	return datoNuevo;
}

estadoPrograma Programa::modificarDatosUsuario(Usuario* &usuario){

	gotoXY(0, 0); cout<<"REGISTRO:";
	emitirDatosUsuario(usuario);
	//modificar nombre?
	usuario->setNombre(modificar("el nombre? ", usuario->getNombre(), 2));
	//modificar apellido?
	usuario->setApellido(modificar("el apellido? ", usuario->getApellido(), 3));
	//el DNI no se modifica
	//modificar provincia?
	usuario->setProvincia(modificar("la provincia? ", usuario->getProvincia(), 5));
	//modificar emails?
	for(unsigned int i = 0; i < usuario->getEmails().size(); i++){
		string aModificar= "el e-mail num " + StringUtil::int2string(i+1) + "? ";
		string nuevoEmail = modificar(aModificar, usuario->getEmails().at(i), 6 + i);
		usuario->setEmailEnPosicion(nuevoEmail, i);
	}
	//modificar contraseña?
	usuario->setContrasena(modificar("la contraseña? ", usuario->getContrasena(), 6 + usuario->getEmails().size()));

	indice->modificarUsuario(usuario);

	return OPCIONES_USUARIO;
}

estadoPrograma Programa::iniciarSesion(Usuario* &usuario){

	estadoPrograma estado = MENU_PRINCIPAL;

	string dni = "";
	string contrasena = "";
	gotoXY(0, 0); cout<<"INICIAR SESION:";

	do{
		gotoXY(0, 2);	cout<<"DNI:                 ";
		gotoXY(5, 2);  leer(dni);
	}while(atoi(dni.c_str()) == 0);

	gotoXY(0, 3); cout<<"Contrasena: ";
	desactivarEcho();	leer(contrasena);	activarEcho();

	bool error=true;
	usuario = indice->buscarUsuario(dni, contrasena, error);

	if(!error) estado = OPCIONES_USUARIO;

	if(error){
		gotoXY(0, 9); cout<<"Datos de usuario invalidos!";
	}

	return estado;
}

estadoPrograma Programa::opcionesUsuarioNormal(Usuario* &usuario){

	estadoPrograma estado = MENU_PRINCIPAL;
	int cantidadOpciones = 5;

	gotoXY(0, 2);	cout << "1 - Modificar datos de usuario.";
	gotoXY(0, 3);	cout << "2 - Darse de baja del sistema.";
	gotoXY(0, 4);	cout << "3 - Consultar productos/servicios.";
	gotoXY(0, 5);	cout << "4 - Publicar producto/servicio.";
	gotoXY(0, 6);	cout << "5 - Cerrar sesion.";

	int opcion = leerOpcion(cantidadOpciones, cantidadOpciones+1);
	if (opcion == 1) estado = CAMBIAR_DATOS;
	if (opcion == 3) estado = CONSULTA_SERVICIO;
	if (opcion == 4) estado = PUBLICAR;

	if (opcion == 2){
		bool eliminado = eliminarUsuario(usuario, 10);
		if(eliminado) estado = MENU_PRINCIPAL;
		else estado = OPCIONES_USUARIO;
	}
	return estado;
}

estadoPrograma Programa::opcionesUsuarioProveedor(Usuario* &usuario){

	estadoPrograma estado = MENU_PRINCIPAL;
	int cantidadOpciones = 7;
	gotoXY(0, 2);	cout<<"1 - Modificar datos de usuario.";
	gotoXY(0, 3);	cout<<"2 - Darse de baja del sistema.";
	gotoXY(0, 4);	cout<<"3 - Consultar productos/servicios.";
	gotoXY(0, 5);	cout<<"4 - Publicar producto/servicio.";
	gotoXY(0, 6);	cout<<"5 - Responder preguntas/cotizaciones.";
	gotoXY(0, 7);	cout<<"6 - Dar de baja un producto.";
	gotoXY(0, 8);	cout<<"7 - Cerrar sesion.";

	int opcion = leerOpcion(cantidadOpciones, cantidadOpciones+1);
	if(opcion == 1) estado = CAMBIAR_DATOS;
	if(opcion == 3) estado = CONSULTA_SERVICIO;
	if(opcion == 4) estado = PUBLICAR;
	if(opcion == 5) estado = RESPONDER;
	if(opcion == 6) estado = BAJA_PRODUCTO;

	if (opcion == 2){
		bool eliminado = eliminarUsuario(usuario, 13);
		if(!eliminado) estado = OPCIONES_USUARIO;
	}
	return estado;
}

estadoPrograma Programa::opcionesAdministrador(Usuario* &usuario){

	estadoPrograma estado = MENU_PRINCIPAL;
	int cantidadOpciones = 8;

	gotoXY(0, 2);	cout<<"1 - Agregar nuevo administrador.";
	gotoXY(0, 3);	cout<<"2 - Eliminar administrador.";
	gotoXY(0, 4);	cout<<"3 - Generar nueva categoria.";
	gotoXY(0, 5);	cout<<"4 - Modificar categoria.";
	gotoXY(0, 6);	cout<<"5 - Eliminar categoria.";
	gotoXY(0, 7);	cout<<"6 - Moderar mensajes.";
	gotoXY(0, 8);	cout<<"7 - Listado de usuarios.";
	gotoXY(0, 9);	cout<<"8 - Cerrar sesion.";

	int opcion = leerOpcion(cantidadOpciones, cantidadOpciones+1);
	if(opcion == 1) estado = REGISTRO_A;
	if(opcion == 2) estado = BAJA_ADMIN;
	if(opcion == 3) estado = REGISTRO_CAT;
	if(opcion == 4) estado = MOD_CAT;
	if(opcion == 5) estado = BAJA_CAT;
	if(opcion == 6) estado = VER_MENSAJES;
	if(opcion == 7) estado = VER_USUARIOS;

	return estado;
}

estadoPrograma Programa::menuOpcionesUsuario(Usuario* &usuario){

	estadoPrograma estado= MENU_PRINCIPAL;
	string tipoUsuario = usuario->getTipo();
	gotoXY(0, 0);	cout<<"OPCIONES:                             "<<imprimirTipoDeUsuario(tipoUsuario)<<": "<<usuario->getNombre()<<" "<<usuario->getApellido();

	if(tipoUsuario == "U") estado = opcionesUsuarioNormal(usuario);
	if(tipoUsuario == "P") estado = opcionesUsuarioProveedor(usuario);
	if(tipoUsuario == "A") estado = opcionesAdministrador(usuario);

	return estado;
}

estadoPrograma Programa::consultarServicio(vector<Servicio*> &resultados){

	estadoPrograma estado = OPCIONES_USUARIO;
	int cantidadOpciones = 4;

	gotoXY(0, 0);	cout<<"CONSULTAR... ";
	gotoXY(0, 2);	cout<<"1 - Por proveedor.";
	gotoXY(0, 3);	cout<<"2 - Por categoria.";
	gotoXY(0, 4);	cout<<"3 - Por palabra clave.";
	gotoXY(0, 5); 	cout<<"4 - Volver al menu de opciones.";
	int opcion = leerOpcion(cantidadOpciones, 5);

	if(opcion !=4){
		resultados = buscarServicio(opcion);
		if(resultados.size() > 0) estado = RESULTADOS;
		else{
			gotoXY(0, 11);
			cout<<"No se encontro ninguna coincidencia con la busqueda dada.";
		}
	}

	return estado;
}

vector<Servicio*> Programa::buscarServicio(int opcion){

	string por;
	string aBuscar;
	vector <Servicio*> resultado;

	if(opcion == 1) por = "proveedor: ";
	if(opcion == 2) por = "categoria: ";
	if(opcion == 3) por = "palabra clave: ";

	gotoXY(0, 9); cout<<"Busqueda por "<<por; leer(aBuscar);

	if(opcion == 1){
		Usuario* usuario = new Usuario();
		usuario->setDni(atoi(aBuscar.c_str()));
		resultado = indice->buscarServiciosPorUsuario(usuario);
	}

	if(opcion == 2){
		Categoria* categoria = new Categoria();
		categoria->setNombre(aBuscar);
		resultado = indice->buscarServiciosCategoria(categoria);
	}

	if(opcion == 3){
		resultado = indice->buscarServiciosPorPalabrasClave(aBuscar);
	}

	return resultado;
}

estadoPrograma Programa::emitirResultadoBusqueda(vector<Servicio*> &resultados, Usuario* &usuario){

	estadoPrograma estado = RESULTADOS;
	gotoXY(0, 0);	cout<<"RESULTADOS DE LA BUSQUEDA: ";
	int posY = 2;

	for(unsigned int i = 0; i < resultados.size(); i++){
		gotoXY(0, posY); cout<<"Resultado N."<<i+1<<":";	posY++;
		emitirResultado(resultados.at(i), posY, false);
	}

	posY++;
	gotoXY(0,posY); cout<<"1 - Ver resultado en detalle.";	posY++;
	gotoXY(0,posY); cout<<"2 - Volver al menu de opciones.";

	int opcion = leerOpcion(2, posY);
	if(opcion == 1) detalleResultado(resultados, usuario, posY + 4);
	if(opcion == 2) estado = OPCIONES_USUARIO;

	return estado;
}

void Programa::detalleResultado(vector<Servicio*> &resultados, Usuario* &usuario, int posY){

	system("clear");
	gotoXY(0 , 0);
	string numResultado;
	unsigned int resultado;
	do{
		gotoXY(0, posY); cout<<"Ver detalladamente el resultado N.:          ";
		gotoXY(36, posY); leer(numResultado);
		resultado = atoi(numResultado.c_str());
	}while(!((resultado >= 1) && (resultado <= resultados.size())));
	resultado--;
	posY = posY +2;
	emitirResultado(resultados.at(resultado), posY, true);
	posY++;
	emitirPreguntasRespuestasServicio(resultados.at(resultado), posY);
	posY++;

	gotoXY(0, posY); cout<<"1 - Hacer una pregunta.";	posY++;
	gotoXY(0, posY); cout<<"2 - Pedir cotizacion."; posY++;
	gotoXY(0, posY); cout<<"3 - Volver al listado de resultados.";
	int opcion = leerOpcion(3, posY);

	if(opcion == 1) hacerPregunta(resultados.at(resultado), usuario, posY + 4);
	if(opcion == 2) pedirCotizacion(resultados.at(resultado), posY + 4);
}

void Programa::emitirPreguntasRespuestasServicio(Servicio* servicio, int &posY){

	vector<Consulta*> preguntas = indice->buscarConsultasPorServicio(servicio);

	for(unsigned int i = 0 ; i < preguntas.size(); i++){
		if(!preguntas.at(i)->getOculta()){
			gotoXY(0, posY);	cout<<"Pregunta "<<i+1<<": ";	posY++;
			gotoXY(5, posY);	cout<<FechaYHora::getFecha_DD_MM_AAAA(preguntas.at(i)->getFechaConsulta());
			cout<<", "<<FechaYHora::getHoraHH_MM(preguntas.at(i)->getHoraConsulta())<<".";	posY++;
			gotoXY(5, posY);	cout<<preguntas.at(i)->getConsulta();	posY++;
			gotoXY(5, posY);	cout<<"Respuesta: ";	posY++;
			gotoXY(5, posY);	cout<<FechaYHora::getFecha_DD_MM_AAAA(preguntas.at(i)->getFechaRespuesta());
			cout<<", "<<FechaYHora::getHoraHH_MM(preguntas.at(i)->getHoraRespuesta())<<".";	posY++;
			gotoXY(5, posY);	cout<<preguntas.at(i)->getRespuesta();	posY++;
		}else{
			gotoXY(0, posY);	cout<<"Pregunta "<<i+1<<" moderada por el administrador";
		}
			posY++;
	}
}

void Programa::hacerPregunta(Servicio* &resultado, Usuario* &usuario, int posY){

	bool preguntado=false;
	string pregunta;
	gotoXY(0, posY);
	cout<<"Escriba su pregunta en "<<max_caracteres_pregunta<<" caracteres: ";

	leer(pregunta);

	gotoXY(0, posY+4);
	if(pregunta.size()<= max_caracteres_pregunta){

		// creo una nueva consulta
		Consulta* consulta = new Consulta();
		consulta->setId(atoi(indice->obtenerIdActual(idConsulta).c_str()));
		consulta->setConsulta(pregunta);
		consulta->setIdServicio(resultado->getId());
		consulta->setIdUsuario(usuario->getDni());
		consulta->setOculta(false);
		preguntado = indice->agregarConsulta(consulta);

		if(!preguntado) cout<<"La pregunta no se pudo agregar";
		else{
			cout<<"Pregunta agregada satisfactoriamente!";
			indice->incrementarId(idConsulta);
		}

		}else{
			cout<<"Su pregunta es demasiado larga.";
	}
}

void Programa::pedirCotizacion(Servicio* &resultado, int posY){

	gotoXY(0, posY); cout<<"Funcionalidad no implementada.";
}

estadoPrograma Programa::listadoUsuarios(){

	cout<<"Funcionalidad no implementada.";
	return OPCIONES_USUARIO;
}

void Programa::emitirCategoriasDisponibles(){

	vector<Categoria*> categorias = indice->obtenerTodasLasCategorias();
	int posY = 1;
	int posX = 40;
	gotoXY(posX, 0);		cout<<"Categorias disponibles:";

	for(unsigned int i=0; i< categorias.size(); i++){

		gotoXY(posX + 5, posY);	cout<<categorias.at(i)->getNombre();	posY++;
	}
}

estadoPrograma Programa::publicarServicio(Usuario* &usuario){

	string titulo, descr, tipo, cat, respuesta;
	Servicio* servicio = new Servicio();

	servicio->setId(atoi(indice->obtenerIdActual(idServicio).c_str()));
	servicio->setIdProveedor(usuario->getDni());

	emitirCategoriasDisponibles();

	gotoXY(0, 0);	cout<<"PUBLICAR:";
	gotoXY(0, 2);	cout<<"Titulo: ";		leer(titulo); 	servicio->setNombre(titulo);
	gotoXY(0, 3);	cout<<"Descripcion: ";	leer(descr);	servicio->setDescripcion(descr);

	do{
		gotoXY(0, 5);	cout<<"Tipo: (GR = Gratuito, PF = Precio Fijo, SU = Subasta) "; 	leer(tipo);
	}while((tipo != "GR")&&(tipo != "PF")&&(tipo != "SU"));

	servicio->setTipo(tipo);

	int posY=6;

	//pido categorias
	bool otraCat = true;
	while(otraCat){
		gotoXY(0, posY);	cout<<"Categoria: ";	leer(cat);

		bool error= false;
		do{
			//si la categoria pedida existe, se la seteo al servicio
			Categoria* categoria = indice->buscarCategoria(cat, error);
			if(!error) {
				posY++;
				servicio->setCategoria(categoria);
			}
			else{
				gotoXY(0, posY+2); cout<<"No existe la categoria deseada.";
			}
			//agregar otra categoria?
			gotoXY(0, posY+3);	cout<<"Agregar otra categoria? (s/n) "; leer(respuesta);
		}while((respuesta != "n")&&(respuesta != "s"));
		if(respuesta == "n") otraCat = false;
		gotoXY(0, posY+2);	cout<<"                                           ";
	}

	bool agregado = indice->agregarServicio(servicio);
	gotoXY(0, posY+4);

	if(agregado){
		cout<<"Publicacion exitosa!";
		usuario->setTipo("P"); //si se publica correctamente el usuario se convierte en proveedor
		indice->incrementarId(idServicio);
		indice->modificarUsuario(usuario);
	}else{
		cout<<"No se pudo publicar su servicio.";
	}
	return OPCIONES_USUARIO;
}

estadoPrograma Programa::responderPregunta(){

	estadoPrograma estado = MENU_PRINCIPAL;
	cout<<"estoy en responder";
	return estado;
}

estadoPrograma Programa::bajaProducto(){

	estadoPrograma estado = MENU_PRINCIPAL;
	cout<<"estoy en baja";
	return estado;
}

estadoPrograma Programa::bajaAdmin(Usuario* &adminActual){

	gotoXY(0, 0);	cout<<"ELIMINAR ADMINISTRADOR:";
	string id;
	int posY=4,contAdministradores=1;
	vector<Usuario*> usuarios = indice->obtenerTodosLosUsuarios();
	Usuario *usuarioAux;

	for(unsigned int i=0; i<usuarios.size(); i++){

		usuarioAux = usuarios.at(i);

		if(usuarioAux->getTipo() == "A" && (usuarioAux != adminActual) ){
		gotoXY(0, posY); cout<<"Administrador nro " <<usuarios.size()<<": ";	posY++;
		gotoXY(0, posY); cout<<"Nombre: "<< usuarioAux->getNombre(); posY++;
		gotoXY(0, posY); cout<<"ID: "<< usuarioAux->getDni() ; posY++;
		contAdministradores++;
		}
	}

	do{
		gotoXY(0, posY);	cout<<"Ingrese el ID del administrador a eliminar:                 ";
		gotoXY(44, posY);	leer(id); posY++;
	}while(atoi(id.c_str()) <= 0);

	//busco que exista el ID ingresado:
	bool encontrado = false, IDvalido=false;
	unsigned int indice=0;
	while( (indice<usuarios.size()) && (!encontrado) ){
		if( atoi(id.c_str()) == usuarios.at(indice)->getDni()) encontrado = true;
		indice++;
	}

	if( encontrado && (atoi(id.c_str())!=adminActual->getDni()) ) IDvalido = true;

	if(IDvalido){
		Usuario* usuario = new Usuario();
		usuario->setDni(atoi(id.c_str()));
		usuario->setTipo("A");
		eliminarUsuario(usuario, posY);
	}else{
		system("clear");
		gotoXY(0, 10);
		cout<<"No se puede eliminar el administrador pedido.";

	}

	return OPCIONES_USUARIO;
}

estadoPrograma Programa::generarNuevasCategorias(){

	gotoXY(0, 0); cout<<"CARGA DE CATEGORIAS: ";

	gotoXY(0, 2);	cout<<"1 - Carga manual de categorias.";
	gotoXY(0, 3); 	cout<<"2 - Carga masiva de categorias.";
	gotoXY(0, 4);	cout<<"3 - Volver al menu de opciones.";

	int opcion = leerOpcion(3, 4);

	if(opcion == 1) cargaManualCategoria();
	if(opcion == 2) cargaMasivaCategoria();

	return OPCIONES_USUARIO;
}

void Programa::cargaManualCategoria(){

	gotoXY(0, 8);
	string nombre, descripcion;
	do{
		cout<<"Nombre de la categoria: "; leer(nombre);
		if(nombre.size() > max_nombre_categoria){
			gotoXY(0, 12); cout<<"Nombre demasiado largo.";
		}
	}while(nombre.size() > max_nombre_categoria);

	do{
		gotoXY(0, 9);
		cout<<"Descripcion: "; leer(descripcion);
		if(descripcion.size() > max_descr_categoria){
			gotoXY(0, 12); cout<<"Descripcion demasiado larga.";
		}
	}while(descripcion.size() > max_descr_categoria);

	Categoria* nuevaCategoria = new Categoria();
	nuevaCategoria->setId(atoi(indice->obtenerIdActual(idCategoria).c_str()));
	nuevaCategoria->setNombre(nombre);
	nuevaCategoria->setDescripcion(descripcion);

	bool agregada = indice->agregarCategoria(nuevaCategoria);
	gotoXY(0, 14);
	if(agregada){
		cout<<"Categoria '"<<nombre<<"' agregada con exito!";
	}else{
		cout<<"Ya existe la categoria '"<<nombre<<"'.";
	}
}

void Programa::cargaMasivaCategoria(){ //EL AGREGARCATEGORIA DEBERIA CHEQUEAR LOS NOMBRES

	gotoXY(0, 8);
	string rutaArchivoCategorias;
	ifstream archivo;
	bool agregada;

	cout<<"Ingrese la ruta del archivo de categorias: "; leer(rutaArchivoCategorias);

	archivo.open(rutaArchivoCategorias.c_str());
	if(!archivo){
		gotoXY(0, 14); 	cout<<"No se pudo encontrar el archivo de categorias indicado.";
	}else{
		vector<Categoria*> categorias = leerCategoriasDeArchivo(archivo);

		for(unsigned int i = 0; i < categorias.size(); i++){

			agregada = indice->agregarCategoria(categorias.at(i));
			gotoXY(0, 12 + i);
			if(agregada){
				cout<<"Categoria '"<<categorias.at(i)->getNombre()<<"' agregada con exito!";
			}else{
				cout<<"Ya existe la categoria '"<<categorias.at(i)->getNombre()<<"'.";
			}
		}

		archivo.close();
	}
}
/*
 * antes de agregar la categoria se cheque que tenga el "separador de categoria",
 * y que el nombre no supere el tamaño maximo permitido.
 * La descripcion se copia hasta el tamaño maximo permitido.
 * Por lo tanto, si una categoria no tiene el "separador de categoria" o el nombre supera
 * el maximo permitido, se la considera invalida y no se agrega.
 */
vector<Categoria*> Programa::leerCategoriasDeArchivo(ifstream &archivo){

	vector<Categoria*> categorias;
	char* linea= new char[max_nombre_categoria + max_descr_categoria +3];
	string nombre = "", descripcion = "";

	archivo.getline(linea, max_nombre_categoria + max_descr_categoria +3);

	while(!archivo.eof()){
		unsigned int i=0, j=0;
		nombre ="";
		while( (linea[i] != separadorCategoria)  && (i <= max_nombre_categoria) ){
			nombre += linea[i];
			i++;
		}

		descripcion="";
		j = i+1;
		while(j <= max_descr_categoria ){
			descripcion += linea[j];
			j++;
		}

		if((i = max_nombre_categoria) && linea[i] != separadorCategoria){
			//si entra es porque es una categoria invalida, por lo tanto no la agrego.
		}else{
		Categoria* categoria =new Categoria();
		categoria->setId(atoi(indice->obtenerIdActual(idCategoria).c_str()));
		categoria->setNombre(nombre);
		categoria->setDescripcion(descripcion);
		categorias.push_back(categoria);
		}

		archivo.getline(linea, max_nombre_categoria + max_descr_categoria +3);
	};

	return categorias;
}

estadoPrograma Programa::bajaCategoria(){	////FALTAN METODOS

	gotoXY(0, 0);	cout<<"ELIMINAR CATEGORIA:";
	string id;
	bool eliminada=false;
	//falta listado de todos las categorias
	do{
		gotoXY(0, 2);	cout<<"Ingrese el ID de la categoria a eliminar:                 ";
		gotoXY(42, 2);	leer(id);
	}while(atoi(id.c_str()) <= 0);


		Categoria* categoria= new Categoria();
		categoria->setId(atoi(id.c_str()));
		//eliminada = indice->eliminarCategoria(categoria);
		gotoXY(0, 4);
		if(eliminada)
			cout<<"Se ha eliminado la categoria indicada.";
		else
			cout<<"No se pudo eliminar la categoria indicada.";

	return OPCIONES_USUARIO;
}

int Programa::leerOpcion(int cantOpciones, int posY){

	string opcion;
	int N;
	do{
		gotoXY(0, posY +2);
		cout<<"Ingrese opcion:                                          ";
		gotoXY(16, posY + 2);
		leer(opcion);

		N = atoi(opcion.c_str());

	}while((N < 1) || (N > cantOpciones));

	return N;
}

void Programa::emitirResultado(Servicio* resultado, int &posY, bool enDetalle){

	if(enDetalle){
		gotoXY(5, posY); cout<<"ID: "<<resultado->getId();					posY++;
		gotoXY(5, posY); cout<<"Proveedor: "<<resultado->getIdProveedor(); 	posY++;
	};

	gotoXY(5, posY); cout<<"Nombre: "<<resultado->getNombre();				posY++;
	gotoXY(5, posY); cout<<"Tipo: "<<resultado->getTipo();					posY++;

	if(enDetalle){
		gotoXY(5, posY); cout<<"Descripcion: "<<resultado->getDescripcion();posY++;
		gotoXY(5, posY); cout<<"Categoria/s: ";
		for(unsigned int j=0; j<resultado->getCategorias().size(); j++){
			gotoXY(13, posY); cout<<resultado->getCategorias().at(j)<<".";	posY++;
		}
	}
	posY++;
}

void Programa::emitirDatosUsuario(Usuario* &usuario){

	int posY=2;

	gotoXY(0, posY);	cout<<"Nombre:     "<<usuario->getNombre();		posY++;
	gotoXY(0, posY);	cout<<"Apellido:   "<<usuario->getApellido();	posY++;
	gotoXY(0, posY);	cout<<"DNI:        "<<usuario->getDni();		posY++;
	gotoXY(0, posY); 	cout<<"Provincia:  "<<usuario->getProvincia();	posY++;

	for(unsigned int i=0; i< usuario->getEmails().size(); i++){
	gotoXY(0, posY);	cout<<"e-Mail "<<i+1<<":   "<<usuario->getEmails().at(i);	posY++;
	}

	gotoXY(0, posY);	cout<<"Contraseña: "<<usuario->getContrasena(); posY++;

}

string Programa::imprimirTipoDeUsuario(string tipo){

	string respuesta;
	if(tipo == "U") respuesta = "Usuario";
	if(tipo == "P") respuesta = "Proveedor";
	if(tipo == "A") respuesta = "Administrador";

	return respuesta;
}

bool Programa::eliminarUsuario(Usuario* usuario, int posY){

	bool usuarioEliminado= false;
	string respuesta;
	do {
		gotoXY(0, posY);
		cout<< "Esta seguro que desea eliminar al ";
		cout<<imprimirTipoDeUsuario(usuario->getTipo())<<"? (s/n) ";
		if(usuario->getTipo() != "A")
			cout<<"(Las publicaciones, preguntas y respuestas no serán borradas)";
		leer(respuesta);
	} while ((respuesta != "s") && (respuesta != "n"));

	if (respuesta == "s"){
		usuarioEliminado = indice->elimininarUsuario(usuario);
		gotoXY(0, posY);
		if(usuarioEliminado){
			cout << imprimirTipoDeUsuario(usuario->getTipo())<<" eliminado correctamente!                                                                                   ";

		}else{
			cout <<"No se pudo eliminar al "<<imprimirTipoDeUsuario(usuario->getTipo())<<"                                                                               ";

		}
	}

	return usuarioEliminado;
}

void Programa::leer(string& nombre) {
	getline(cin, nombre);
}

bool Programa::tieneEspacios(string cadena){

	for(unsigned int i=0; i<cadena.size(); i++){
		if(cadena[i] == ' ') return true;
	}
	return false;
}

void Programa::emitirInformacion(){

	gotoXY(0, -5); 	cout<<"Organizacion de Datos - Catedra Servetto - 2do C. 2013";
	gotoXY(0,-3);	cout<<" PAGINAS DORADAS - Drink Team.";
}

void Programa::gotoXY(int x, int y){
	cout<<"\033["<<offsetY + y<<";"<<offsetX + x<<"H";
}

void Programa::activarEcho(){

	termios tty;
	tcgetattr(0, &tty);
	tty.c_lflag |= ECHO;
	tcsetattr(0, TCSANOW, &tty);
}

void Programa::desactivarEcho(){

	termios tty;
	tcgetattr(0, &tty);
	tty.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &tty);
}
