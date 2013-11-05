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

		case MENU_PRINCIPAL:	estado = menuPrincipal();
								break;
		case RECUPERAR_PASS:	estado = recuperacion();
								break;
		case REGISTRO_U:		estado = registrarNuevoUsuario("U");
								break;
		case REGISTRO_A:		estado = registrarNuevoUsuario("A");
								break;
		case CAMBIAR_DATOS:		estado = modificarDatosUsuario(usuario);
								break;
		case INICIAR_SESION:	estado = iniciarSesion(usuario);
								break;
		case OPCIONES_USUARIO:	estado = menuOpcionesUsuario(usuario);
								break;
		case CONSULTA_SERVICIO: estado = consultarServicio(resultados);
								break;
		case RESULTADOS:		estado = emitirResultadoBusqueda(resultados);
								break;
		case PUBLICAR:			estado = publicarServicio();
								break;
		case RESPONDER:			estado = responderPregunta();
								break;
		case BAJA_PRODUCTO:		estado = bajaProducto();
								break;
		case ADMINISTRACION:	estado = opcionesAdministrador();
								break;
		default:				break;
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
	//se puede llegar a hacer busqueda de usuario, pero este metodo es de juguete, no vale la pena
	string nombre="";
	gotoXY(0,0);	cout<<"RECUPERAR CONTRASEÑA:";
	gotoXY(0,2);	cout<<"Usuario: ";	cin>>nombre;
	gotoXY(0,9);	cout<<"Se ha enviado un mail con su contraseña a su casilla de correo, ah re que no hacia nada.";

	return MENU_PRINCIPAL;
}

estadoPrograma Programa::registrarNuevoUsuario(string tipo){

	estadoPrograma estado = MENU_PRINCIPAL;
	Usuario* nuevoUsuario = new Usuario();
	bool usuarioAgregado = false;
	string nombre, apellido, DNI, provincia, mail, contrasena;
	string respuestaMail;
	int posY=2;

	gotoXY(0, 0);		cout<<"REGISTRO:";
	nuevoUsuario->setTipo(tipo);
	gotoXY(0, posY);	cout<<"Nombre:     ";	cin>>nombre;	nuevoUsuario->setNombre(nombre);		posY++;
	gotoXY(0, posY);	cout<<"Apellido:   ";	cin>>apellido;	nuevoUsuario->setApellido(apellido);	posY++;

	do{
		gotoXY(0, posY); 	cout<<"                      ";
		gotoXY(0, posY);	cout<<"DNI:        ";		cin>>DNI;
	}while(atoi(DNI.c_str()) == 0);

	nuevoUsuario->setDni(atoi(DNI.c_str()));posY++;

	gotoXY(0, posY); 	cout<<"Provincia:  ";cin>>provincia;	nuevoUsuario->setProvincia(provincia);	posY++;

	bool otroMail = true;
	//pido hasta 3 mails
	for(int i=1; (i<4)&&(otroMail); i++){

	gotoXY(0, posY);	cout<<"e-Mail   "<<i<<": ";	cin>>mail; 		nuevoUsuario->setEmail(mail);	posY++;
		if(i<3){
			do{
				gotoXY(0, posY+1); cout<<"Desea agregar otra direccion de e-Mail? (s/n) ";
				cin>>respuestaMail;
			}while((respuestaMail != "s")&&(respuestaMail != "n"));

			gotoXY(0, posY+1); cout<<"                                                   "; //para borrar la pregunta
			if(respuestaMail == "n") otroMail = false;
			}
	}

	gotoXY(0, posY);	cout<<"Contraseña: ";	cin>>contrasena;	nuevoUsuario->setContrasena(contrasena);

	//intento agregar el usuario al indice
	usuarioAgregado = indice->agregarUsuario(nuevoUsuario);

	gotoXY(0, posY+2);
	if(usuarioAgregado)	cout<<"Usuario ingresado exitosamente!";
	else cout<<"El usuario ya se encuentra registrado en el sistema.";

	if(tipo == "A") estado = ADMINISTRACION;
	return estado;
}

string Programa::modificar(string queCosa, string valorActual, int posicionDato){

	string respuesta, datoNuevo;
	do{
		gotoXY(0, 12);
		cout<<"Desea modificar "<<queCosa<<"(s/n)"; cin>>respuesta;
	}while((respuesta != "s")&&(respuesta != "n"));

	if(respuesta == "n") datoNuevo = valorActual;
	if(respuesta == "s"){
		gotoXY(12, posicionDato); cout<<"                                    "; //borro lo que haya
		gotoXY(12, posicionDato);
		cin>>datoNuevo;
	}
	return datoNuevo;
}

estadoPrograma Programa::modificarDatosUsuario(Usuario* &usuario){

	gotoXY(0, 0); cout<<"REGISTRO:";
	emitirDatosUsuario(usuario);

	usuario->setNombre(modificar("el nombre? ", usuario->getNombre(), 2));
	usuario->setApellido(modificar("el apellido? ", usuario->getApellido(), 3));
	usuario->setProvincia(modificar("la provincia? ", usuario->getProvincia(), 5));
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
		gotoXY(5, 2);  cin>>dni;
	}while(atoi(dni.c_str()) == 0);

	gotoXY(0, 3); cout<<"Contrasena: ";
	desactivarEcho();	cin>>contrasena;	activarEcho();

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
	gotoXY(0, 6);	cout << "5 - Volver al menu principal.";

	int opcion = leerOpcion(cantidadOpciones, 6);
	if (opcion == 1) estado = CAMBIAR_DATOS;
	if (opcion == 3) estado = CONSULTA_SERVICIO;
	if (opcion == 4) estado = PUBLICAR;
	if (opcion == 5) estado = MENU_PRINCIPAL;

	if (opcion == 2){
		bool eliminado = eliminarUsuario(usuario);
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
	gotoXY(0, 5);	cout<<"5 - Responder preguntas/cotizaciones.";
	gotoXY(0, 6);	cout<<"6 - Dar de baja un producto.";
	gotoXY(0, 7);	cout<<"7 - Volver al menu principal.";

	int opcion = leerOpcion(cantidadOpciones, 7);
	if(opcion == 1) estado = CAMBIAR_DATOS;
	if(opcion == 3) estado = CONSULTA_SERVICIO;
	if(opcion == 4) estado = PUBLICAR;
	if(opcion == 5) estado = RESPONDER;
	if(opcion == 6) estado = BAJA_PRODUCTO;
	if(opcion == 7) estado = MENU_PRINCIPAL;

	if (opcion == 2){
		bool eliminado = eliminarUsuario(usuario);
		if(eliminado) estado = MENU_PRINCIPAL;
		else estado = OPCIONES_USUARIO;
	}
	return estado;
}

estadoPrograma Programa::opcionesAdministrador(){ //FALta

	estadoPrograma estado = MENU_PRINCIPAL;
	int cantidadOpciones = 6;

	gotoXY(0, 2);	cout<<"1 - Agregar nuevo administrador.";
	gotoXY(0, 3);	cout<<"2 - Eliminar administrador.";
	gotoXY(0, 4);	cout<<"3 - Generar nueva categoria.";
	gotoXY(0, 5);	cout<<"4 - Eliminar categoria.";
	gotoXY(0, 6);	cout<<"5 - Moderar mensajes.";
	gotoXY(0, 7);	cout<<"6 - Listado de usuarios.";

	int opcion = leerOpcion(cantidadOpciones, 7);
	if(opcion == 1) estado = REGISTRO_A;
	if(opcion == 2) estado = REGISTRO_A;
	if(opcion == 3) estado = TERMINAR;
	return estado;
}

estadoPrograma Programa::menuOpcionesUsuario(Usuario* &usuario){

	estadoPrograma estado= MENU_PRINCIPAL;
	string tipoUsuario = usuario->getTipo();
	gotoXY(0, 0);	cout<<"OPCIONES:                             "<<imprimirTipoDeUsuario(tipoUsuario)<<": "<<usuario->getNombre();

	if(tipoUsuario == "U") estado = opcionesUsuarioNormal(usuario);
	if(tipoUsuario == "P") estado = opcionesUsuarioProveedor(usuario);
	if(tipoUsuario == "A") estado = opcionesAdministrador();

	return estado;
}

estadoPrograma Programa::consultarServicio(vector<Servicio*> &resultados){

	estadoPrograma estado = CONSULTA_SERVICIO;
	int cantidadOpciones = 4;

	gotoXY(0, 0);	cout<<"CONSULTAR... ";
	gotoXY(0, 2);	cout<<"1 - Por proveedor.";
	gotoXY(0, 3);	cout<<"2 - Por categoria.";
	gotoXY(0, 4);	cout<<"3 - Por palabra clave.";
	gotoXY(0, 5); 	cout<<"4 - Volver al menu de opciones.";
	int opcion = leerOpcion(cantidadOpciones, 5);

	if(opcion == 4) estado = OPCIONES_USUARIO;
	if(opcion !=4){
		resultados = buscarServicio(opcion);
		if(resultados.size()>0) estado = RESULTADOS;
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

	gotoXY(0, 9); cout<<"Busqueda por "<<por; cin>>aBuscar;

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

estadoPrograma Programa::emitirResultadoBusqueda(vector<Servicio*> &resultados){

	estadoPrograma estado = RESULTADOS;
	gotoXY(0, 0);	cout<<"RESULTADOS DE LA BUSQUEDA: ";
	int posY = 2;

	for(unsigned int i = 0; i< resultados.size(); i++){
		gotoXY(0, posY); cout<<"Resultado N."<<i+1<<":";	posY++;
		emitirResultado(resultados.at(i), posY, false);
	}

	posY++;
	gotoXY(0,posY); cout<<"1 - Ver resultado en detalle.";	posY++;
	gotoXY(0,posY); cout<<"2 - Volver al menu de opciones.";

	int opcion = leerOpcion(2, posY);
	if(opcion == 1) detalleResultado(resultados, posY + 4);
	if(opcion == 2) estado = OPCIONES_USUARIO;

	return estado;
}

void Programa::detalleResultado(vector<Servicio*> &resultados, int posY){

	string numResultado;
	unsigned int resultado;
	do{
		gotoXY(0, posY); cout<<"Ver detalladamente el resultado N.: ";cin>>numResultado;
		resultado = atoi(numResultado.c_str());
	}while((resultado < 1) || (resultado > resultados.size()));
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

	if(opcion == 1) hacerPregunta(resultados.at(resultado), posY + 4);
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

void Programa::hacerPregunta(Servicio* &resultado, int posY){

}

void Programa::pedirCotizacion(Servicio* &resultado, int posY){

}

estadoPrograma Programa::publicarServicio(){

	estadoPrograma estado = MENU_PRINCIPAL;
	cout<<"estoy en publicar";
	return estado;
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

int Programa::leerOpcion(int cantOpciones, int posY){

	string opcion;
	int N;
	do{
		gotoXY(0, posY +2);
		cout<<"Ingrese opcion:            ";
		gotoXY(16, posY + 2);
		cin>>opcion;

		N = atoi(opcion.c_str());

	}while((N < 1) || (N > cantOpciones));

	return N;
}

void Programa::emitirResultado(Servicio* resultado, int &posY, bool enDetalle){

	if(enDetalle){
		gotoXY(5, posY); cout<<"ID: "<<resultado->getId();					posY++;
		gotoXY(5, posY); cout<<"Proveedor: "<<resultado->getIdProveedor(); 	posY++;
	};

	gotoXY(5, posY); cout<<"Tipo: "<<resultado->getTipo();					posY++;
	gotoXY(5, posY); cout<<"Nombre: "<<resultado->getNombre();				posY++;

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

bool Programa::eliminarUsuario(Usuario* usuario){

	bool usuarioEliminado= false;
	string respuesta;
	do {
		gotoXY(0, 8);
		cout<< "Esta seguro que desea eliminar al usuario? (s/n) (Las publicaciones, preguntas y respuestas no serán borradas)";
		cin >> respuesta;
	} while ((respuesta != "s") && (respuesta != "n"));

	if (respuesta == "s"){
		usuarioEliminado = indice->elimininarUsuario(usuario);
		gotoXY(0, 9);
		if(usuarioEliminado){
			cout << "Usuario eliminado correctamente!                                                                                   ";
		}else{
			cout <<"No se pudo eliminar al usuario.                                                                                     ";
		}
	}

	return usuarioEliminado;
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
