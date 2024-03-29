/*
 * Programa.cpp
 *
 *  Created on: 31/10/2013
 *      Author: marian
 */

#include "Programa.h"

Programa::Programa(){

	bool error= true;
	lector = LectorConfig::getLector(rutaConfig);
	indice = new Indice("");

	//si no existe clave del sistema, la genero:
	indice->buscarClaveSistema(error);
	if(error) this->generarClave();

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
	Servicio* resultado;
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
		case RESULTADOS:		estado = emitirResultadoBusqueda(resultados, resultado, usuario);	break;
		case RESULTADO_DET:		estado = detalleResultado(resultado, usuario);			break;
		case PUBLICAR:			estado = publicarServicio(usuario);						break;
		case RESPONDER_PREG:	estado = responderPregunta(usuario);					break;
		case BAJA_PRODUCTO:		estado = bajaProducto(usuario);							break;
		case VER_USUARIOS:		estado = listadoUsuarios();								break;
		case VER_PEDIDOS_COTIZACION: estado = verPedidosCotizacion(usuario);			break;
		case REGISTRO_CAT:		estado = generarNuevasCategorias();						break;
		case LISTAR_CATEGORIAS: estado = listarCategorias();							break;
		case BAJA_CAT:			estado = bajaCategoria();								break;
		case MODERAR_MENSAJES:	estado = moderarMensajes();								break;
		case MODIFICAR_CATEGORIA: estado = modificarCategoria();						break;
		case CAMBIAR_CONTRASENA: estado = cambiarContrasena();							break;
		default:																		break;

		}

		system("clear");
	}
}

estadoPrograma Programa::menuPrincipal(){

	estadoPrograma estado= MENU_PRINCIPAL;
	int cantidadOpciones = 4;
	int posY = 0;
	emitir("MENU PRINCIPAL:", 			0, posY); 	posY = 2;
	emitir("1 - Iniciar sesion.", 		0, posY);	posY++;
	emitir("2 - Registrarse.", 			0, posY);	posY++;
	emitir("3 - Recuperar contraseña.", 0, posY);	posY++;
	emitir("4 - Salir del programa.", 	0, posY);	posY++;

	int opcion = leerOpcion(cantidadOpciones, 5);
	if(opcion == 1) estado = INICIAR_SESION;
	if(opcion == 2) estado = REGISTRO_U;
	if(opcion == 3) estado = RECUPERAR_PASS;
	if(opcion == 4) estado = TERMINAR;

	return estado;
}

estadoPrograma Programa::recuperacion(){
	string nombre="";
	int posY = 0;
	emitir("RECUPERAR CONTRASEÑA:", 0, posY);					posY = 2;
	emitir("Usuario: ", 			0, posY);	leer(nombre);	posY = 4;
	emitir("Se ha enviado un mail con su contraseña a su casilla de correo.", 0, posY); posY++;
	emitir("Presione ENTER para continuar...", 0, posY);
	this->esperarEnter();

	return MENU_PRINCIPAL;
}

estadoPrograma Programa::altaUsuario(string tipo){

	estadoPrograma estado = MENU_PRINCIPAL;
	Usuario* nuevoUsuario = new Usuario();
	bool usuarioAgregado = false;
	string nombre, apellido, DNI, provincia, mail, contrasena;
	string respuestaMail;
	int posY=0;

	emitir("REGISTRO: nuevo " + imprimirTipoDeUsuario(tipo) + "." , 0, posY); posY = 2;
	nuevoUsuario->setTipo(tipo);

	emitir("Nombre:     ", 0, posY); 	leer(nombre); 	nuevoUsuario->setNombre(nombre);		posY++;
	emitir("Apellido:   ", 0, posY);	leer(apellido);	nuevoUsuario->setApellido(apellido);	posY++;
	do{
		emitir("                      ", 0, posY);
		emitir("DNI:        ",			 0, posY); leer(DNI);
	}while((atoi(DNI.c_str()) <= 0) || (tieneEspacios(DNI)));

	nuevoUsuario->setDni(StringUtil::str2int(DNI.c_str()));		posY++;

	emitir("Provincia:  ", 0, posY);	leer(provincia); nuevoUsuario->setProvincia(provincia);	posY++;

	bool otroMail = true;
	//pido hasta 3 mails
	for(int i=1; (i<4)&&(otroMail); i++){
	gotoXY(0, posY);	cout<<"e-Mail   "<<i<<": ";	leer(mail); 	nuevoUsuario->setEmailEnPosicion(mail,i-1);	posY++;
		if(i<3){
			do{
				gotoXY(0, posY+1); cout<<"Desea agregar otra direccion de e-Mail? (s/n) ";
				leer(respuestaMail);
			}while((respuestaMail != "s")&&(respuestaMail != "n"));

			gotoXY(0, posY+1); cout<<"                                                   "; //para borrar la pregunta
			if(respuestaMail == "n") otroMail = false;
			}
	}

	//si quedan mails sin setear, les asigno un "--"
	if(nuevoUsuario->getEmails().size()<3){
		do{
			string mailVacio = "--";
			nuevoUsuario->setEmailEnPosicion(mailVacio, nuevoUsuario->getEmails().size());
		}while(nuevoUsuario->getEmails().size() < 3);
	}

	bool contrasenaValida=false;

	while(!contrasenaValida){
		gotoXY(0, posY);	cout<<"Contraseña (de "<<TAMANIO_CLAVE_SISTEMA<<" caracteres):                       ";
		gotoXY(30, posY);
		leer(contrasena);
		contrasenaValida= Hill::claveValida(contrasena);
		if(!contrasenaValida){
			posY++;
			gotoXY(0, posY);
			cout<<"La contraseña es invalida, por favor reingrese.";
			posY--;
		}
	}
	nuevoUsuario->setContrasena(Hill::encriptar(contrasena,this->obtenerClaveDelSistema()));

	//intento agregar el usuario al indice
	usuarioAgregado = indice->agregarUsuario(nuevoUsuario);

	gotoXY(0, posY+7);
	if(usuarioAgregado)	cout<<"Usuario registrado exitosamente!";
	else cout<<"El usuario ya se encuentra registrado en el sistema.";

	//si se registra un administrador, lo registró otra admin por lo que se va al menu de administracion
	if(tipo == "A")	estado = OPCIONES_USUARIO;

	return estado;
}

string Programa::modificar(string queCosa, string valorActual, int posicionDato){

	string respuesta, datoNuevo;
	bool contrasenaValida;
	do{
		gotoXY(0, 12);
		cout<<"Desea modificar "<<queCosa<<"(s/n)"; leer(respuesta);
	}while((respuesta != "s")&&(respuesta != "n"));

	if(respuesta == "n") datoNuevo = valorActual;
	if(respuesta == "s"){
		gotoXY(12, posicionDato); cout<<"                                    "; //borro lo que haya
		gotoXY(12, posicionDato);
		leer(datoNuevo);

		if(queCosa == "la contraseña? "){
			contrasenaValida = Hill::claveValida(datoNuevo);
			if(contrasenaValida) return Hill::encriptar(datoNuevo, this->obtenerClaveDelSistema());
			else{
				posicionDato +=2;
				gotoXY(12, posicionDato); cout<<"La contraseña es invalida, no se pudo modificar.";
				return valorActual;
			}
		}
	}
	return datoNuevo;
}

estadoPrograma Programa::modificarDatosUsuario(Usuario* &usuario){

	gotoXY(0, 0); cout<<"MODIFICAR:";
	emitirDatosUsuario(usuario);
	//modificar nombre?
	usuario->setNombre(modificar("el nombre? ", usuario->getNombre(), 2));
	//modificar apellido?
	usuario->setApellido(modificar("el apellido? ", usuario->getApellido(), 3));
	//el DNI no se modifica
	//modificar provincia?
	usuario->setProvincia(modificar("la provincia? ", usuario->getProvincia(), 5));

	//modificar emails?
	string nuevoEmail = modificar("el e-mail n. 1?", usuario->getEmails().at(0), 6);
	usuario->setEmailEnPosicion(nuevoEmail, 0);
		   nuevoEmail = modificar("el e-mail n. 2?", usuario->getEmails().at(1), 7);
	usuario->setEmailEnPosicion(nuevoEmail, 1);
	       nuevoEmail = modificar("el e-mail n. 3?", usuario->getEmails().at(2), 8);
	usuario->setEmailEnPosicion(nuevoEmail, 2);

	//modificar contraseña?
	usuario->setContrasena(modificar("la contraseña? ", usuario->getContrasena(), 9));

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

	//encripto la contraseña ingresada para poder compararla con la guardada
	contrasena = Hill::encriptar(contrasena, this->obtenerClaveDelSistema());

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
		bool eliminado = eliminarUsuario(usuario);
		if(eliminado) estado = MENU_PRINCIPAL;
		else estado = OPCIONES_USUARIO;
	}
	return estado;
}

estadoPrograma Programa::opcionesUsuarioProveedor(Usuario* &usuario){

	estadoPrograma estado = MENU_PRINCIPAL;
	int cantidadOpciones = 8;
	gotoXY(0, 2);	cout<<"1 - Modificar datos de usuario.";
	gotoXY(0, 3);	cout<<"2 - Darse de baja del sistema.";
	gotoXY(0, 4);	cout<<"3 - Consultar productos/servicios.";
	gotoXY(0, 5);	cout<<"4 - Publicar producto/servicio.";
	gotoXY(0, 6);	cout<<"5 - Responder preguntas.";
	gotoXY(0, 7);	cout<<"6 - Ver pedidos de cotizacion.";
	gotoXY(0, 8);	cout<<"7 - Dar de baja un producto.";
	gotoXY(0, 9);	cout<<"8 - Cerrar sesion.";

	int opcion = leerOpcion(cantidadOpciones, cantidadOpciones+1);
	if(opcion == 1) estado = CAMBIAR_DATOS;
	if(opcion == 3) estado = CONSULTA_SERVICIO;
	if(opcion == 4) estado = PUBLICAR;
	if(opcion == 5) estado = RESPONDER_PREG;
	if(opcion == 6) estado = VER_PEDIDOS_COTIZACION;
	if(opcion == 7) estado = BAJA_PRODUCTO;

	if (opcion == 2){
		bool eliminado = eliminarUsuario(usuario);
		if(!eliminado) estado = OPCIONES_USUARIO;
	}
	return estado;
}

estadoPrograma Programa::opcionesAdministrador(Usuario* &usuario){

	estadoPrograma estado = MENU_PRINCIPAL;
	int cantidadOpciones = 10;

	gotoXY(0, 2);	cout<<"1 - Agregar nuevo administrador.";
	gotoXY(0, 3);	cout<<"2 - Eliminar administrador.";
	gotoXY(0, 4);	cout<<"3 - Generar nueva categoria.";
	gotoXY(0, 5);	cout<<"4 - Modificar categoria.";
	gotoXY(0, 6);	cout<<"5 - Eliminar categoria.";
	gotoXY(0, 7);	cout<<"6 - Moderar mensajes.";
	gotoXY(0, 8);	cout<<"7 - Listado de usuarios.";
	gotoXY(0, 9);	cout<<"8 - Listado de categorias.";
	gotoXY(0, 10);	cout<<"9 - Cambiar Contraseñas.";
	gotoXY(0, 11);	cout<<"10 - Cerrar sesion.";

	int opcion = leerOpcion(cantidadOpciones, cantidadOpciones+1);
	if(opcion == 1) estado = REGISTRO_A;
	if(opcion == 2) estado = BAJA_ADMIN;
	if(opcion == 3) estado = REGISTRO_CAT;
	if(opcion == 4) estado = MODIFICAR_CATEGORIA;
	if(opcion == 5) estado = BAJA_CAT;
	if(opcion == 6) estado = MODERAR_MENSAJES;
	if(opcion == 7) estado = VER_USUARIOS;
	if(opcion == 8) estado = LISTAR_CATEGORIAS;
	if(opcion == 9) estado = CAMBIAR_CONTRASENA;

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
		usuario->setDni(StringUtil::str2int(aBuscar.c_str()));
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

estadoPrograma Programa::emitirResultadoBusqueda(vector<Servicio*> &resultados, Servicio* &resultado, Usuario* &usuario){

	estadoPrograma estado = RESULTADOS;
	gotoXY(0, 0);	cout<<"RESULTADOS DE LA BUSQUEDA: ";
	int posY = 2;

	for(unsigned int i = 0; i < resultados.size(); i++){
		emitir("Resultado N." + StringUtil::int2string(i+1) + ":", 0, posY);	posY++;
		emitirResultado(resultados.at(i), posY, false);
	}

	posY++;
	emitir("1 - Ver resultado en detalle.", 	0, posY);	posY++;
	emitir("2 - Volver al menu de opciones.", 	0, posY);

	int opcion = leerOpcion(2, posY);
	unsigned int num;
	string numResultado;
	if(opcion == 1){
		do{
			posY+=4;
			emitir("Ver detalladamente el resultado N.:          ", 0, posY);
			gotoXY(36, posY); leer(numResultado);
			num = atoi(numResultado.c_str());
		}while(!((num >= 1) && (num <= resultados.size())));

		resultado = resultados.at(num-1);
		estado = RESULTADO_DET;
	}
	if(opcion == 2) estado = OPCIONES_USUARIO;

	return estado;
}

estadoPrograma Programa::detalleResultado(Servicio* &resultado, Usuario* &usuario){

	gotoXY(0, 0);	cout<<"DETALLES RESULTADO: ";
	int posY = 2;
	emitirResultado(resultado, posY, true);
	emitirPreguntasRespuestasServicio(resultado, posY);
	posY++;

	emitir("1 - Hacer una pregunta.", 				0, posY);	posY++;
	emitir("2 - Pedir cotizacion.", 				0, posY); 	posY++;
	emitir("3 - Volver al listado de resultados.",	0, posY);
	int opcion = leerOpcion(3, posY);

	if(opcion == 1) hacerPregunta(resultado, usuario, posY + 2);
	if(opcion == 2) pedirCotizacion(resultado, usuario, posY + 2);

	return RESULTADOS;
}

void Programa::emitirPreguntasRespuestasServicio(Servicio* &servicio, int &posY){

	vector<Consulta*> preguntas = indice->buscarConsultasPorServicio(servicio);

	for(unsigned int i = 0 ; i < preguntas.size(); i++){
		if(!preguntas.at(i)->getOculta()){
			emitir("Pregunta " + StringUtil::int2string(i+1) + ": ", 0, posY);	posY++;
			emitir(FechaYHora::getFecha_DD_MM_AAAA(preguntas.at(i)->getFechaConsulta()) +
			", " + FechaYHora::getHoraHH_MM(preguntas.at(i)->getHoraConsulta()) + ".", 5, posY); posY++;

			emitir(preguntas.at(i)->getConsulta(), 5, posY);	posY++;

			if(preguntas.at(i)->getRespuesta() != "--"){
				emitir("Respuesta: ", 0, posY);		posY++;
				emitir(FechaYHora::getFecha_DD_MM_AAAA(preguntas.at(i)->getFechaRespuesta()) +
				", " + FechaYHora::getHoraHH_MM(preguntas.at(i)->getHoraRespuesta()) + ".", 5, posY);	posY++;
				emitir(preguntas.at(i)->getRespuesta(), 5, posY);	posY++;
			}
		}else{
			emitir("Pregunta " + StringUtil::int2string(i+1) + " moderada por el administrador.", 0, posY);
		}
			posY++;
	}
}

void Programa::hacerPregunta(Servicio* &resultado, Usuario* &usuario, int posY){

	bool preguntado=false;
	string pregunta;
	gotoXY(0, posY);
	emitir("Escriba su pregunta: ", 0, posY);	leer(pregunta);

	if(pregunta.size()<= max_caracteres_pregunta){

		// creo una nueva consulta
		Consulta* consulta = new Consulta();
		int IDNuevo = StringUtil::str2int(lector->getValor(idConsulta).c_str()) + 1;
		consulta->setId(IDNuevo);
		consulta->setIdServicio(resultado->getId());
		consulta->setIdUsuario(usuario->getDni());
		consulta->setConsulta(pregunta);
		consulta->setFechaConsulta(FechaYHora::setFechaAAAAMMDD());
		consulta->setHoraConsulta(FechaYHora::setHoraHHMM());
		consulta->setRespuesta("--");
		consulta->setFechaRespuesta("00000000");
		consulta->setHoraRespuesta("0000");
		consulta->setOculta(false);
		preguntado = indice->agregarConsulta(consulta);

		if(!preguntado) emitir("La pregunta no se pudo agregar", 0, posY);
		else{
			emitir("Pregunta agregada satisfactoriamente!", 0, posY);
			//si se agrega bien, sumo en 1 efectivamente el contador de consultas
			string nuevoID = StringUtil::int2string(IDNuevo);
			lector->setValor(idConsulta, nuevoID);
		}

		}else{
			emitir("Su pregunta es demasiado larga.", 0, posY);
	}
}

void Programa::pedirCotizacion(Servicio* &resultado, Usuario* &usuario, int posY){

	bool pedidoEmitido=false;
		string pedido,contrasenaProveedor;
		gotoXY(0, posY);
		emitir("Escriba su pedido de cotizacion: ", 0, posY);	leer(pedido);
		Usuario* proveedor;
		bool error;
		proveedor = indice->buscarUsuario(StringUtil::int2string(resultado->getIdProveedor()), error);

		//desencripto la contrasenia del proveedor:
		contrasenaProveedor = Hill::desencriptar(proveedor->getContrasena(),this->obtenerClaveDelSistema());

		// creo un nuevo pedido de cotizacion
		PedidoCotizacion* pedidoCotizacion = new PedidoCotizacion();
		int IDNuevo = StringUtil::str2int(lector->getValor(idConsulta).c_str()) + 1;
		pedidoCotizacion->setId(IDNuevo);
		pedidoCotizacion->setIdServicio(resultado->getId());
		pedidoCotizacion->setIdUsuario(usuario->getDni());
		pedidoCotizacion->setPedido(Hill::encriptar(pedido,contrasenaProveedor));
		pedidoCotizacion->setFechaPedido(FechaYHora::setFechaAAAAMMDD());
		pedidoCotizacion->setHoraPedido(FechaYHora::setHoraHHMM());

		pedidoEmitido = indice->agregarPedidoCotizacion(pedidoCotizacion);

			if(!pedidoEmitido) emitir("el pedido no se pudo agregar", 0, posY);
			else{
				emitir("Pregunta agregada satisfactoriamente!", 0, posY);
				//si se agrega bien, sumo en 1 efectivamente el contador de consultas
				string nuevoID = StringUtil::int2string(IDNuevo);
				lector->setValor(idConsulta, nuevoID);
			}

}


estadoPrograma Programa::listadoUsuarios(){

	estadoPrograma estado = OPCIONES_USUARIO;

		int posY = 0;

		vector<Usuario*> usuarios = indice->obtenerTodosLosUsuarios();
		Usuario *usuarioAux;

		for(unsigned int i=0; i<usuarios.size(); i++){

			usuarioAux = usuarios.at(i);

			emitir("Usuario nro " + StringUtil::int2string(i+1) + ": ", 0, posY);	posY++;
			emitir("Nombre: " + usuarioAux->getNombre(), 5, posY); 				posY++;
			emitir("Id: " + StringUtil::int2string(usuarioAux->getDni()), 5, posY) ;		posY+=2;
		}


		emitir("Presione ENTER para volver al menu principal... ", 0, posY);
		esperarEnter();

		return estado;
}

void Programa::emitirCategoriasDisponibles(vector<Categoria*> categorias){

		int posY = 1;
		int posX = 50;
		gotoXY(posX, 0);		cout<<"Categorias disponibles:";

		for(unsigned int i=0; i< categorias.size(); i++){

			emitir(categorias.at(i)->getNombre(), posX + 2, posY);	posY++;
		}

}

estadoPrograma Programa::publicarServicio(Usuario* &usuario){

	//si pude agregar alguna categoria sigo, sino se cancela la publicacion
	bool agregado = false;
	bool error = true;
	bool tieneCategorias = true;
	vector<Categoria*> categorias = indice->obtenerTodasLasCategorias(error);

	if(!error){

		string titulo, descr, tipo, cat, respuesta;
		Servicio* servicio = new Servicio();
		int IDNuevo = StringUtil::str2int(lector->getValor(idServicio).c_str()) + 1;
		servicio->setId(IDNuevo);
		servicio->setIdProveedor(usuario->getDni());

		emitirCategoriasDisponibles(categorias);

		gotoXY(0, 0);	cout<<"PUBLICAR:";
		gotoXY(0, 2);	cout<<"Titulo: ";		leer(titulo); 	servicio->setNombre(titulo);
		gotoXY(0, 3);	cout<<"Descripcion: ";	leer(descr);	servicio->setDescripcion(descr);

		do{
			gotoXY(0, 5);	cout<<"Tipo: (GR/PF/SU) "; 	leer(tipo);
		}while((tipo != "GR")&&(tipo != "PF")&&(tipo != "SU"));

		servicio->setTipo(tipo);

		int posY=6;

		//pido categorias
		bool otraCat = true;
		while(otraCat){
			gotoXY(0, posY);	cout<<"Categoria:                    ";
			gotoXY(11, posY);	leer(cat);

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
				gotoXY(0, posY+3);	cout<<"                                      ";
				gotoXY(0, posY+2);	cout<<"                                      ";
			}while((respuesta != "n")&&(respuesta != "s"));
			if(respuesta == "n"){
				if(servicio->getCategorias().size() == 0) tieneCategorias = false;
				otraCat = false;
			}
		}

		gotoXY(0, posY+6);

		if(tieneCategorias) agregado = indice->agregarServicio(servicio);
		if(agregado){
			cout<<"Publicacion exitosa!";
			usuario->setTipo("P"); //si se publica correctamente el usuario se convierte en proveedor
			indice->modificarUsuario(usuario);
			//y se aumenta efectivamente el contador de servicios
			int nuevoID = StringUtil::str2int(lector->getValor(idServicio)) +1;
			string ID = StringUtil::int2string(nuevoID);
			lector->setValor(idServicio, ID);
		}else{
			cout<<"No se pudo publicar su servicio.";
		}
	}else{
		gotoXY(0, 10);
		cout<<"No hay ninguna categoria registrada en el sistema.";
	}
	return OPCIONES_USUARIO;
}

estadoPrograma Programa::responderPregunta(Usuario* &usuario){

	estadoPrograma estado = OPCIONES_USUARIO;
	vector<Servicio*> productos = indice->buscarServiciosPorUsuario(usuario);

	vector<Consulta*> consultas;
	unsigned int contProductos=0;
	int posY=0;
	string respuesta;

	for(unsigned int i=0; i< productos.size();i++){
		consultas = indice->buscarConsultasPorServicio(productos.at(i));
		contProductos++;
		for(unsigned int j=0; j< consultas.size(); j++){

			//si no esta respondida ni oculta, la emito
			if((consultas.at(j)->getRespuesta() == "--")&&(consultas.at(j)->getOculta() == false)){
				posY = 0;
				system("clear");
				emitir("Pruducto " + StringUtil::int2string(contProductos) + ": "			 ,  0, posY);	posY++;
				emitir(productos.at(i)->getNombre()											 ,  3, posY);	posY++;
				emitir("Pregunta: " + consultas.at(j)->getConsulta()    		 			 ,  0, posY);	posY+=3;
				emitir(		"1 - Responder pregunta."			    						 ,  0, posY);	posY++;
				emitir(		"2 - Continuar con la siguiente pregunta."					     ,  0, posY);	posY++;
				emitir(		"3 - Dejar de responder."		    							 ,  0, posY);	posY++;
				int opcion = leerOpcion(3,posY);
				if(opcion == 1) {
					posY=3;
					emitir("Respuesta: ", 0, posY); leer(respuesta);
					consultas.at(j)->setRespuesta(respuesta);
					consultas.at(j)->setFechaRespuesta(FechaYHora::setFechaAAAAMMDD());
					consultas.at(j)->setHoraRespuesta(FechaYHora::setHoraHHMM());
					indice->modificarConsulta(consultas.at(j));
				}
				if(opcion == 3) return estado;
			}
		}
	}
	system("clear");
	emitir(		"Se han respondido todos los mensajes."		    							 ,  0, posY);	posY++;
	emitir(		"presione ENTER para volver al menu..."			    						 ,  0, posY);	posY++;
	esperarEnter();

return estado;


}

estadoPrograma Programa::verPedidosCotizacion(Usuario* &usuario){

		estadoPrograma estado = OPCIONES_USUARIO;
		vector<PedidoCotizacion*> pedidos = indice->buscarPedidosCotizacionHechasAUsuario(usuario);
		int posY =0;
		string contrasenaUsuario, pedido;

		contrasenaUsuario = Hill::desencriptar(usuario->getContrasena(),this->obtenerClaveDelSistema());

		for(unsigned int i=0; i< pedidos.size();i++){

					posY++;
					emitir("Pedido " + StringUtil::int2string(i+1) + ": "			 , 							0, posY);	posY++;
					emitir("ID Usuario: " + StringUtil::int2string(pedidos.at(i)->getIdUsuario()),				0, posY); 	posY++;
					emitir("ID Servicio: "+ StringUtil::int2string(pedidos.at(i)->getIdServicio()),				0, posY);	posY++;
					emitir("Fecha y hora: " + FechaYHora::getFecha_DD_MM_AAAA(pedidos.at(i)->getFechaPedido()) + ", " + FechaYHora::getHoraHH_MM(pedidos.at(i)->getHoraPedido()), 0, posY); posY++;
					emitir(Hill::desencriptar(pedidos.at(i)->getPedido(),contrasenaUsuario)	 ,  3, posY);	posY++;

		}

		posY++;
		emitir(		"Se han mostrado todos los mensajes."		    							 ,  0, posY);	posY++;
		emitir(		"presione ENTER para volver al menu..."			    						 ,  0, posY);	posY++;
		esperarEnter();


	return estado;
}


estadoPrograma Programa::bajaProducto(Usuario* &usuario){

	estadoPrograma estado = OPCIONES_USUARIO;
	vector <Servicio*> productos = indice->buscarServiciosPorUsuario(usuario);

		int posY = 2;
	if(productos.size() > 0){

		gotoXY(0 ,0); cout<<"BAJA DE SERVICIO:";
		//si llego a esta pantalla es porque todavia tengo cosas publicadas
		for(unsigned int i=0; i<productos.size(); i++){
			emitir("Producto N. " + StringUtil::int2string(i+1) + ": ", 0, posY); posY++;
			emitirResultado(productos.at(i), posY, false); posY++;
		}
		posY++;

		string numPublicacion;
		unsigned int numero;
		do{
			emitir("Borrar la publicacion N. : ", 0, posY);	leer(numPublicacion);
			numero = atoi(numPublicacion.c_str());
		}while(!((numero >= 1)&&(numero <= productos.size())));

		posY++;
		string confirmacion;
		do{
			emitir("Seguro que desea borrar la publicacion N. "+ StringUtil::int2string(numero) + "? (s/n) ", 0, posY);	leer(confirmacion);
		}while((confirmacion != "s")&&(confirmacion != "n"));

		posY+=2;
		if(confirmacion == "s"){
			numero--;
			bool eliminado = indice->eliminarServicio(productos.at(numero));

			if(eliminado){
				emitir("Se ha borrado su producto.", 0, posY);
				if(productos.size() == 1){
					//si solo tenia un producto y lo borre, paso a ser usuario normal
					usuario->setTipo("U");
					indice->modificarUsuario(usuario);
				}
			}
			else emitir("No se pudo borrar su producto.", 0, posY);


		}else{
			emitir("No se ha borrado su producto.", 0, posY);
		}

	}else{
		gotoXY(0, 0); cout<<"No hay ningun producto registrado con sus datos.";
//		char c;
//		cin.get(c);
	}

	return estado;
}

estadoPrograma Programa::bajaAdmin(Usuario* &adminActual){

	gotoXY(0, 0);	cout<<"ELIMINAR ADMINISTRADOR:";
	string id;
	int posY=2,contAdministradores=1;
	vector<Usuario*> usuarios = indice->obtenerTodosLosUsuarios();
	Usuario *usuarioAux;

	for(unsigned int i=0; i<usuarios.size(); i++){

		usuarioAux = usuarios.at(i);

		if( (usuarioAux->getTipo() == "A") && (usuarioAux->getDni() != adminActual->getDni()) ){
		emitir("Administrador nro " +StringUtil::int2string(contAdministradores)+ ": ",0,posY);	 		 posY++;
		emitir("Nombre: " + usuarioAux->getNombre(), 5, posY);					 posY++;
		emitir("ID: " + StringUtil::int2string(usuarioAux->getDni()), 5, posY) ; posY++;
		posY++;

		contAdministradores++;
		}
	}


	if(contAdministradores == 1){
		system("clear");
		gotoXY(0, 14);
		emitir("Usted es el unico administrador hasta el momento.", 0, posY);		  posY++;
		emitir("Presione ENTER para volver al menu...", 0, posY);					  posY++;
		esperarEnter();
		return OPCIONES_USUARIO;
	}

	posY++;

	do{
		emitir("Ingrese el ID del administrador a eliminar:                 ", 0, posY);
		gotoXY(44, posY);	leer(id);
	}while(atoi(id.c_str()) <= 0);

	//busco que exista el ID ingresado:
	bool encontrado = false, IDvalido=false;
	unsigned int indice=0;
	while( (indice<usuarios.size()) && (!encontrado) ){
		if( atoi(id.c_str()) == usuarios.at(indice)->getDni()) encontrado = true;
		indice++;
	}

	//verifico que el ID ingresado no sea uno mismo.
	if( encontrado && ( atoi(id.c_str()) != adminActual->getDni() ) ) IDvalido = true;

	posY+= 4;
	if(IDvalido){
		Usuario* usuario = new Usuario();
		usuario->setDni(atoi(id.c_str()));
		usuario->setTipo("A");
		eliminarUsuario(usuario);
	}else{

		system("clear");
		gotoXY(0, 0);
		emitir("No se puede eliminar el administrador pedido.", 0, posY); posY++;
		emitir("Presione ENTER para volver al menu...", 0, posY);
		esperarEnter();


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
	if(opcion == 3) return OPCIONES_USUARIO;
	char c;
	cin.get(c);
	return OPCIONES_USUARIO;

}

void Programa::cargaManualCategoria(){

	gotoXY(0, 8);
	string nombre="", descripcion="";
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
	int IDNuevo = StringUtil::str2int(lector->getValor(idCategoria).c_str()) + 1;
	nuevaCategoria->setId(IDNuevo);
	nuevaCategoria->setNombre(nombre);
	if(descripcion=="")	nuevaCategoria->setDescripcion("-");
	else nuevaCategoria->setDescripcion(descripcion);

	bool agregada = indice->agregarCategoria(nuevaCategoria);

	int posY = 14;
	if(agregada){
		emitir("Categoria '" + nombre + "' agregada con exito!", 0, posY);
		//se incrementa efectivamente el contador de categorias
		string nuevoID = StringUtil::int2string(IDNuevo);
		lector->setValor(idCategoria, nuevoID);
	}else{
		emitir("Ya existe la categoria '" + nombre + "'.", 0, posY);
	}
}

void Programa::cargaMasivaCategoria(){ //EL AGREGARCATEGORIA DEBERIA CHEQUEAR LOS NOMBRES

	gotoXY(0, 8);
	string rutaArchivoCategorias;
	ifstream archivo;
	bool agregada;
	int posY = 14;

	cout<<"Ingrese la ruta del archivo de categorias: "; leer(rutaArchivoCategorias);

	archivo.open(rutaArchivoCategorias.c_str());
	if(!archivo){
		gotoXY(0, 14); 	cout<<"No se pudo encontrar el archivo de categorias indicado.";
	}else{
		vector<Categoria*> categorias = leerCategoriasDeArchivo(archivo);

		for(unsigned int i = 0; i < categorias.size(); i++){

			agregada = indice->agregarCategoria(categorias.at(i));
			 posY++;
			if(agregada){
				emitir("Categoria '" + categorias.at(i)->getNombre() + "' agregada con exito!", 0, posY);
			}else{
				emitir("Ya existe la categoria '" + categorias.at(i)->getNombre() + "'.", 0, posY);
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

		if((i > max_nombre_categoria) && linea[i-1] != separadorCategoria){
			//si entra es porque es una categoria invalida, por lo tanto no la agrego.
		}else{
		Categoria* categoria =new Categoria();
		int IDNuevo = StringUtil::str2int(lector->getValor(idCategoria).c_str()) + 1;
		categoria->setId(IDNuevo);
		string nuevoID = StringUtil::int2string(IDNuevo);
		lector->setValor(idCategoria, nuevoID);
		categoria->setNombre(nombre);
		categoria->setDescripcion(descripcion);
		categorias.push_back(categoria);
		}

		archivo.getline(linea, max_nombre_categoria + max_descr_categoria +3);
	};

	return categorias;
}

estadoPrograma Programa::bajaCategoria(){	////FALTAN METODOS


	string categoriaIngresada;
	string respuesta="s";

	int posY=0;

	system("clear");

	do{
			system("clear");
			emitir(	"ingrese el nombre de la categoria que desea eliminar: "	 ,  0, posY);
			leer(categoriaIngresada);
			if( !existeCategoria(categoriaIngresada) ){
				do{
					system("clear");
					emitir(	"La categoria ingresada no existe, reintentar? (s/n) " ,  0, posY);
					leer(respuesta);
				}while((respuesta != "s")&&(respuesta != "n"));
			}
		}while((!existeCategoria(categoriaIngresada))&&(respuesta == "s"));

	if(existeCategoria(categoriaIngresada)){
		indice->eliminarCategoria(categoriaIngresada);
		system("clear");
		emitir(	"la categoria ha sido elimina exitosamente. "	 ,  0, posY);	posY++;
		emitir(	"presione ENTER para continuar... " ,  0, posY);
		esperarEnter();
	}


	return OPCIONES_USUARIO;
}

int Programa::leerOpcion(int cantOpciones, int posY){

	string opcion;
	int N;
	do{
		int Y = posY + 2;
		emitir("Ingrese opcion:                                          ", 0, Y);
		gotoXY(16, Y);
		leer(opcion);

		N = atoi(opcion.c_str());

	}while((N < 1) || (N > cantOpciones));

	return N;
}

void Programa::emitirResultado(Servicio* &resultado, int &posY, bool enDetalle){

	if(enDetalle){
		emitir("ID: " + StringUtil::int2string(resultado->getId()), 5, posY);					posY++;
		emitir("Proveedor: " + StringUtil::int2string(resultado->getIdProveedor()), 5, posY);	posY++;
	};

	emitir("Nombre: " + resultado->getNombre(), 5, posY);				posY++;
	emitir("Tipo: " + resultado->getTipo(), 5, posY);					posY++;

	if(enDetalle){
		emitir("Descripcion: " + resultado->getDescripcion(), 5, posY);	posY++;
		emitir("Categorias: ", 5, posY);
		for(unsigned int j=0; j<resultado->getCategorias().size(); j++){
			emitir(resultado->getCategorias().at(j)->getNombre() + ".", 17, posY);	posY++;
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
	gotoXY(0, posY);	cout<<"e-Mail 1:   "<<usuario->getEmails().at(0);	posY++;
	gotoXY(0, posY);	cout<<"e-Mail 2:   "<<usuario->getEmails().at(1);	posY++;
	gotoXY(0, posY);	cout<<"e-Mail 3:   "<<usuario->getEmails().at(2);	posY++;
	gotoXY(0, posY);	cout<<"Contraseña: "<<Hill::desencriptar(usuario->getContrasena(), this->obtenerClaveDelSistema()); posY++;

}

string Programa::imprimirTipoDeUsuario(string tipo){

	string respuesta;
	if(tipo == "U") respuesta = "Usuario";
	if(tipo == "P") respuesta = "Proveedor";
	if(tipo == "A") respuesta = "Administrador";

	return respuesta;
}

bool Programa::eliminarUsuario(Usuario* usuario){

	system("clear");
	bool usuarioEliminado= false;
	string respuesta;
	do {
		gotoXY(0, 0);
		cout<<"Esta seguro que desea eliminar al " + imprimirTipoDeUsuario(usuario->getTipo()) + "? (s/n) ";
		if(usuario->getTipo() != "A")
			cout<<"(Las publicaciones, preguntas y respuestas no serán borradas)";
		leer(respuesta);
	} while ((respuesta != "s") && (respuesta != "n"));

	if (respuesta == "s"){
		usuarioEliminado = indice->elimininarUsuario(usuario);
		gotoXY(0, 2);
		if(usuarioEliminado){
			cout << imprimirTipoDeUsuario(usuario->getTipo())<<" eliminado correctamente!                                                                                   ";

		}else{
			cout <<"No se pudo eliminar al "<<imprimirTipoDeUsuario(usuario->getTipo())<<"                                                                               ";

		}
	}
	gotoXY(0, 4); cout<<"Presione ENTER para continuar...";
	esperarEnter();

	return usuarioEliminado;
}

void Programa::leer(string& dato) {
	getline(cin, dato);
}

void Programa::esperarEnter(){
	string enter;
	this->leer(enter);
}

void Programa::emitir(string texto, int posX, int &posY){

	if(posY > MAX_Y){
		cout<<endl;	//si llego abajo de la pantalla, hago scrolls
	}

	gotoXY(posX, posY); cout<<texto;
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

estadoPrograma Programa::listarCategorias(){

	estadoPrograma estado = OPCIONES_USUARIO;

	int posY = 0;
	bool error=true;
	vector<Categoria*> categorias = indice->obtenerTodasLasCategorias(error);
	Categoria *categoriaAux;

	if(!error){
		for(unsigned int i=0; i<categorias.size(); i++){

			categoriaAux = categorias.at(i);

			emitir("Categoria nro " + StringUtil::int2string(i+1) + ": ", 0, posY);	posY++;
			emitir("Nombre: " + categoriaAux->getNombre(), 5, posY); 				posY++;
			emitir("Descripcion: " + categoriaAux->getDescripcion(), 5, posY) ;		posY+=2;


		}
	}else{
		emitir("No hay ninguna categoria registrada en el sistema.", 0, posY); posY++;
	}

	emitir("Presione ENTER para volver al menu principal... ", 0, posY);
	esperarEnter();

	return estado;
}

estadoPrograma Programa::moderarMensajes(){

	estadoPrograma estado = OPCIONES_USUARIO;
	vector<Usuario*> usuarios = indice->obtenerTodosLosUsuarios();

	vector<Consulta*> consultas;
	unsigned int contConsultas=0;
	int posY=0;

	for(unsigned int i=0; i< usuarios.size();i++){

		consultas = indice->buscarConsultasHechasAUsuario(usuarios.at(i));

		for(unsigned int j=0; j< consultas.size(); j++){

			if(consultas.at(j)->getOculta() == false){
				system("clear");
				posY=0;
				contConsultas++;
				emitir("Mensaje numero " + StringUtil::int2string(contConsultas) + ": ",  0, posY);	posY++;
				emitir("Pregunta: " + consultas.at(j)->getConsulta()     			   ,  0, posY);	posY++;
				emitir("Respuesta: " +  consultas.at(j)->getRespuesta()                ,  0, posY); posY+=2;
				emitir(		"1 - Ocultar Mensaje."			    							 ,  0, posY);	posY++;
				emitir(		"2 - Continuar con el siguiente mensaje."					     ,  0, posY);	posY++;
				emitir(		"3 - Dejar de moderar."			    							 ,  0, posY);	posY++;
				int opcion = leerOpcion(3,posY);
				if(opcion == 1) {
					consultas.at(j)->setOculta(true);
					indice->modificarConsulta(consultas.at(j));
				}
				if(opcion == 3) return estado;
			}
		}
	}
	system("clear");
	emitir(		"Se han moderado todos los mensajes."		    							 ,  0, posY);	posY++;
	emitir(		"presione ENTER para volver al menu..."			    						 ,  0, posY);	posY++;
	esperarEnter();

return estado;
}

estadoPrograma Programa::modificarCategoria(){
	system("clear");
	estadoPrograma estado = OPCIONES_USUARIO;
	int posY = 0;
	string categoriaIngresada,nombre,descripcion, respuesta;
	Categoria *categoriaAux;
	bool error=true;

	vector<Categoria*> categorias = indice->obtenerTodasLasCategorias(error);

	if(!error){
		do{
			system("clear");
			emitir(	"Ingrese la categoria que desea modificar: ", 0, posY);
			leer(categoriaIngresada);
			if( !existeCategoria(categoriaIngresada) ){
				do{
					system("clear");
					emitir(	"La categoria ingresada no existe, reintentar? (s/n) " ,  0, posY);
					leer(respuesta);
				}while((respuesta != "s")&&(respuesta != "n"));
			}
		}while((!existeCategoria(categoriaIngresada))&&(respuesta == "s"));

		//la busco para conservar el mismo ID.(el indice usa el ID)
		if(existeCategoria(categoriaIngresada)){
			categoriaAux = this->buscarCategoria(categoriaIngresada);
			leerNombreCategoria(nombre); posY++;
			leerDescripcionCategoria(descripcion); posY++;
			categoriaAux->setNombre(nombre);
			categoriaAux->setDescripcion(descripcion);
			indice->modificarCategoria(categoriaAux);

			system("clear");
			emitir(	"La categoria ha sido modificada satisfactoriamente. " ,  0, posY); posY++;
			emitir(	"presione ENTER para continuar... " ,  0, posY);
			esperarEnter();
		}
	}else{
		emitir("No hay ninguna categoria registrada en el sistema.", 0, posY); posY++;

	}
		return estado;
}

bool Programa::existeCategoria(string categoria){

		Categoria *cat = this->buscarCategoria(categoria);

		if(cat == NULL) return false;
		else return true;


}

Categoria* Programa::buscarCategoria(string categoria){

			bool error=true;
			vector<Categoria*> categorias = indice->obtenerTodasLasCategorias(error);

			if(!error){
					for(unsigned int i=0; i<categorias.size(); i++){

						if(categorias.at(i)->getNombre() == categoria) return categorias.at(i);

					}
				}

			return NULL;
}

void Programa::leerNombreCategoria(string& nombre){
	int posY=0;

	do{
			system("clear");

			emitir("Ingrese el nuevo nombre: ", 0, posY);
			leer(nombre);

			if(nombre.size() > max_nombre_categoria){
				system("clear");
				emitir("Nombre demasiado largo... :S ", 0, posY); posY++;
				emitir("presione ENTER para volver a intentar... ", 0, posY);
				esperarEnter();
			}
		}while(nombre.size() > max_nombre_categoria);

}


void Programa::leerDescripcionCategoria(string& descripcion){
	int posY=0;

	system("clear");
	do{
			system("clear");
			emitir("Ingrese la nueva descripcion: ", 0, posY);
			leer(descripcion);

			if(descripcion.size() > max_descr_categoria){
				system("clear");
				emitir("descripcion demasiado largo... :S ", 0, posY); posY++;
				emitir("presione ENTER para volver a intentar... ", 0, posY);
				esperarEnter();
			}
		}while(descripcion.size() > max_nombre_categoria);

}


string Programa::obtenerClaveDelSistema(){

	bool e;
	return indice->buscarClaveSistema(e);
}


estadoPrograma Programa::cambiarContrasena(){
	estadoPrograma estado = OPCIONES_USUARIO;
	string id,contrasena;
	int posY=0;
	vector<Usuario*> usuarios = indice->obtenerTodosLosUsuarios();
	Usuario* usuarioAux;

	system("clear");
	emitir("Estos son los usuarios registrados en el sistema: ", 0, posY); posY++;
	//listo todos los usuarios:

	for(unsigned int i=0; i<usuarios.size(); i++){

		usuarioAux = usuarios.at(i);

		emitir("Usuario nro " + StringUtil::int2string(i+1) + ": ", 0, posY);	posY++;
		emitir("Nombre: " + usuarioAux->getNombre(), 5, posY); 				posY++;
		emitir("Id: " + StringUtil::int2string(usuarioAux->getDni()), 5, posY) ;		posY+=2;

	}

	emitir("Presione ENTER para continuar... ", 0, posY);
	esperarEnter();

	do{
			system("clear");
			posY=0;
			emitir("Ingrese el ID del usuario que desea cambiarle la contrasena:                 ", 0, posY); posY++;
			leer(id); posY++;
		}while(atoi(id.c_str()) <= 0);


	//busco que exista el ID ingresado:
	bool encontrado = false;
	unsigned int i=0;
	while( (i<usuarios.size()) && (!encontrado) ){
		if( atoi(id.c_str()) == usuarios.at(i)->getDni()) encontrado = true;
		i++;
	}

	//si existe el usuario, le cambio la contrasenia:

	if(encontrado){
		usuarioAux = usuarios.at(i-1);
		gotoXY(0, posY);	cout<<"ingrese la nueva contraseña: ";	leer(contrasena);

		if(Hill::claveValida(contrasena)){
			usuarioAux->setContrasena(Hill::encriptar(contrasena,this->obtenerClaveDelSistema()));
			indice->modificarUsuario(usuarioAux);

		}else{
			posY +=2;
			gotoXY(12, posY); cout<<"La contraseña es invalida, no se pudo modificar."; posY++;
			emitir("Presione ENTER para volver al menu...", 0, posY);
			esperarEnter();
		}
	}else{

		system("clear");
		gotoXY(0, 0);
		emitir("No existe el usuario ingresado", 0, posY); posY++;
		emitir("Presione ENTER para volver al menu...", 0, posY);
		esperarEnter();


		}

	return estado;
}


void Programa::generarClave(){

	srand(time(NULL));

	 string clave="";
	 bool claveCorrecta = false;
	 char* alfabetoClaves = Hill::getAlfabeto();

	 while(!claveCorrecta){

		 for(int i=0;i<9;i++){
		 	clave += alfabetoClaves[rand() % 41];
		 }
		 claveCorrecta = Hill::claveValida(clave);
		 if(!claveCorrecta) clave = "";
	 }

	 indice->agregarClaveSistema(clave);

}
