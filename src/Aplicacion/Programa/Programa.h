/*
 * Programa.h
 *
 *  Created on: 31/10/2013
 *      Author: marian
 */

#ifndef PROGRAMA_H_
#define PROGRAMA_H_

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <termios.h>
#include <iostream>
#include "../constantes.h"
#include "../Entidades/Usuario.h"
#include "../Indice/Indice.h"
using namespace std;

typedef enum{
	MENU_PRINCIPAL,
	REGISTRO_U,
	REGISTRO_A,
	CAMBIAR_DATOS,
	INICIAR_SESION,
	OPCIONES_USUARIO,
	CONSULTA_SERVICIO,
	BUSQUEDA,
	RESULTADO,
	PUBLICAR,
	RESPONDER,
	BAJA_PRODUCTO,
	ADMINISTRACION,
	RECUPERAR_PASS,
	TERMINAR,
}estadoPrograma;

class Programa {
public:
	Programa();
	virtual ~Programa();
	void ejecutarPrograma();

private:
	Indice* indice;

	estadoPrograma menuPrincipal();
	estadoPrograma registrarNuevoUsuario(string tipoUsuario);
	estadoPrograma modificarDatosUsuario(Usuario* &usuario);
	estadoPrograma iniciarSesion(Usuario* &usuario);
	estadoPrograma menuOpcionesUsuario(Usuario* &usuario);
	estadoPrograma consultarServicio();
	estadoPrograma buscarServicio();
	estadoPrograma emitirResultadoBusqueda();
	estadoPrograma publicarServicio();
	estadoPrograma responderPregunta();
	estadoPrograma bajaProducto();
	estadoPrograma administrar();
	estadoPrograma opcionesUsuarioNormal(Usuario* &usuario);
	estadoPrograma opcionesUsuarioProveedor(Usuario* &usuario);
	estadoPrograma opcionesAdministrador();
	estadoPrograma recuperacion();

	bool eliminarUsuario(Usuario* usuario);
	string imprimirTipoDeUsuario(string tipo);
	void emitirDatosUsuario(Usuario* usuario);
	string modificar(string queCosa, string valorActual, int posicionDato);
	void emitirInformacion();
	//para ingresar la contraseña sin mostrarla
	void desactivarEcho();
	void activarEcho();

	void gotoXY(int x, int y);
	int leerOpcion(int cantidadDeOpciones);
};

#endif /* PROGRAMA_H_ */
