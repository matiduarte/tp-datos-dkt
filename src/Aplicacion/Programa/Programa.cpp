/*
 * Programa.cpp
 *
 *  Created on: 31/10/2013
 *      Author: marian
 */

#include "Programa.h"

Programa::Programa() {

	offsetX = 8;
	offsetY = 5;
	system("clear");
}

Programa::~Programa() {
}

void Programa::ejecutarPrograma(){

	estadoPrograma estado = MENU_PRINCIPAL;

	while(estado != TERMINAR){

		switch(estado){

		case MENU_PRINCIPAL:	estado = menuPrincipal();
								break;
		case REGISTRO:			estado = registro();
								break;
		case INICIAR_SESION:	estado = iniciarSesion();
								break;
		case OPCIONES_USUARIO:	estado = menuOpciones();
								break;
		case CONSULTA_SERVICIO: estado = consultarServicio();
								break;
		case BUSQUEDA:			estado = buscarServicio();
								break;
		case RESULTADO:			estado = emitirResultadoBusqueda();
								break;
		case PUBLICAR:			estado = publicarServicio();
								break;
		case RESPONDER:			estado = responderPregunta();
								break;
		case BAJA_PRODUCTO:		estado = bajaProducto();
								break;
		case ADMINISTRACION:	estado = administrar();
								break;
		default:				break;
		}

		system("clear");
	}
}

estadoPrograma Programa::menuPrincipal(){

	estadoPrograma estado= MENU_PRINCIPAL;
	int cantidadOpciones = 3;

	gotoXY(0, 0);	cout<<"MENU PRINCIPAL:";
	gotoXY(0, 2);	cout<<"1 - Iniciar Sesion.";
	gotoXY(0, 3);	cout<<"2 - Registrarse.";
	gotoXY(0, 4);	cout<<"3 - Salir del programa.";

	int opcion = leerOpcion(cantidadOpciones);
	if(opcion == 1) estado = INICIAR_SESION;
	if(opcion == 2) estado = REGISTRO;
	if(opcion == 3) estado = TERMINAR;

	return estado;
}

estadoPrograma Programa::registro(){

	estadoPrograma estado = REGISTRO;
	Usuario* nuevoUsuario = new Usuario();
	bool existeUsuario = false;	//implementar esto!!
	nuevoUsuario->setTipo("U");
	int posY=2;

	string nombre, apellido, DNI, provincia, mail, contrasena;
	string respuestaMail;

	gotoXY(0, 0);		cout<<"REGISTRO:";
	gotoXY(0, posY);	cout<<"Nombre: ";	cin>>nombre;	nuevoUsuario->setNombre(nombre);		posY++;
	gotoXY(0, posY);	cout<<"Apellido: ";	cin>>apellido;	nuevoUsuario->setApellido(apellido);	posY++;

	do{
		gotoXY(0, posY); 	cout<<"                      ";
		gotoXY(0, posY);	cout<<"DNI: ";		cin>>DNI;
	}while(atoi(DNI.c_str()) == 0);							nuevoUsuario->setDni(atoi(DNI.c_str()));posY++;

	gotoXY(0, posY); 	cout<<"Provincia: ";cin>>provincia;	nuevoUsuario->setProvincia(provincia);	posY++;

	bool otroMail = true;
	for(int i=1; (i<4)&&(otroMail); i++){

	gotoXY(0, posY);	cout<<"E-Mail "<<i<<": ";	cin>>mail; 		nuevoUsuario->setEmail(mail);	posY++;
		if(i<3){
			do{
				gotoXY(0, posY+1); cout<<"Desea agregar otra direccion de E-Mail? (s/n) ";
				cin>>respuestaMail;
			}while((respuestaMail != "s")&&(respuestaMail != "n"));

			gotoXY(0, posY+1); cout<<"                                                   "; //para borrar la pregunta
			if(respuestaMail == "n") otroMail = false;
			}
	}

	gotoXY(0, posY);	cout<<"Contraseña: ";	cin>>contrasena;	nuevoUsuario->setContrasena(contrasena);

	if(!existeUsuario){
		gotoXY(0, posY+2); 	cout<<"Usuario ingresado exitosamente!";
		estado = MENU_PRINCIPAL;
	}
	return estado;
}

estadoPrograma Programa::iniciarSesion(){

	estadoPrograma estado = MENU_PRINCIPAL;
	cout<<"estoy en iniciar sesion";
	system("pause");
	return estado;
}

estadoPrograma Programa::menuOpciones(){

	estadoPrograma estado = MENU_PRINCIPAL;
	cout<<"estoy en menu opciones";
	system("pause");
	return estado;
}

estadoPrograma Programa::consultarServicio(){

	estadoPrograma estado = MENU_PRINCIPAL;
	cout<<"estoy en consulta de servicion";
	system("pause");
	return estado;
}

estadoPrograma Programa::buscarServicio(){

	estadoPrograma estado = MENU_PRINCIPAL;
	cout<<"estoy en busqueda de servicios";
	system("pause");
	return estado;
}
estadoPrograma Programa::emitirResultadoBusqueda(){

	estadoPrograma estado = MENU_PRINCIPAL;
	cout<<"estoy en resultados de busqueda";
	system("pause");
	return estado;
}

estadoPrograma Programa::publicarServicio(){

	estadoPrograma estado = MENU_PRINCIPAL;
	cout<<"estoy en publicar";
	system("pause");
	return estado;
}

estadoPrograma Programa::responderPregunta(){

	estadoPrograma estado = MENU_PRINCIPAL;
	cout<<"estoy en responder";
	system("pause");
	return estado;
}

estadoPrograma Programa::bajaProducto(){

	estadoPrograma estado = MENU_PRINCIPAL;
	cout<<"estoy en baja";
	system("pause");
	return estado;
}

estadoPrograma Programa::administrar(){

	estadoPrograma estado = MENU_PRINCIPAL;
	cout<<"estoy en admin";
	system("pause");
	return estado;
}

int Programa::leerOpcion(int cantOpciones){

	string opcion;
	int N;
	do{
		gotoXY(0, cantOpciones +3);
		cout<<"Ingrese opcion: ";
		cin>>opcion;

		N = atoi(opcion.c_str());

	}while((N < 1) && (N > cantOpciones));

	return N;
}

void Programa::gotoXY(int x, int y){
	cout<<"\033["<<offsetY + y<<";"<<offsetX + x<<"H";
}

