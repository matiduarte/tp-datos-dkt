/*
 * Programa.h
 *
 *  Created on: 31/10/2013
 *      Author: marian
 */

#ifndef PROGRAMA_H_
#define PROGRAMA_H_

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <iostream>
#include <termios.h>
#include "../constantes.h"
#include "../Indice/Indice.h"
#include "../Utiles/FechaYHora.h"

using namespace std;

typedef enum{
	MENU_PRINCIPAL,
	REGISTRO_U,
	REGISTRO_A,
	REGISTRO_CAT,
	BAJA_ADMIN,
	BAJA_CAT,
	CAMBIAR_DATOS,
	INICIAR_SESION,
	OPCIONES_USUARIO,
	CONSULTA_SERVICIO,
	VER_MENSAJES,
	VER_USUARIOS,
	RESULTADOS,
	PUBLICAR,
	RESPONDER,
	BAJA_PRODUCTO,
	RECUPERAR_PASS,
	TERMINAR,
}estadoPrograma;

class Programa {
public:
	Programa();
	virtual ~Programa();
	void ejecutar();

private:
	Indice* indice;
	LectorConfig* lector;

	/////FALTA MODIFICAR MAI

	estadoPrograma menuPrincipal();
	estadoPrograma registrarNuevoUsuario(string tipoUsuario);
	estadoPrograma modificarDatosUsuario(Usuario* &usuario);
	estadoPrograma iniciarSesion(Usuario* &usuario);
	estadoPrograma menuOpcionesUsuario(Usuario* &usuario);
	estadoPrograma consultarServicio(vector<Servicio*> &resultados);
	estadoPrograma emitirResultadoBusqueda(vector<Servicio*> &resultados, Usuario* &usuario);
	estadoPrograma publicarServicio();
	estadoPrograma responderPregunta();
	estadoPrograma bajaProducto();
	estadoPrograma bajaAdmin(Usuario* &usuario);
	estadoPrograma opcionesUsuarioNormal(Usuario* &usuario);
	estadoPrograma opcionesUsuarioProveedor(Usuario* &usuario);
	estadoPrograma opcionesAdministrador(Usuario* &usuario);
	estadoPrograma generarNuevasCategorias();
	estadoPrograma recuperacion();
	estadoPrograma bajaCategoria();
	estadoPrograma listadoUsuarios();

	vector<Servicio*> buscarServicio(int opcion);

	bool eliminarUsuario(Usuario* usuario, int posY);

	string imprimirTipoDeUsuario(string tipo);
	void emitirDatosUsuario(Usuario* &usuario);
	string modificar(string queCosa, string valorActual, int posicionDato);

	void emitirResultado(Servicio* resultado,int &posY, bool enDetalle);
	void detalleResultado(vector<Servicio*> &resultados, Usuario* &usuario, int posY);
	void emitirPreguntasRespuestasServicio(Servicio* servicio, int &posY);
	void hacerPregunta(Servicio* &resultado, Usuario* &usuario, int posY);
	void pedirCotizacion(Servicio* &resultado, int posY);

	void cargaManualCategoria();
	void cargaMasivaCategoria();
	vector<Categoria*> leerCategoriasDeArchivo(ifstream &archivo); //falta comprobar errores
	bool tieneEspacios(string dato);
	void emitirInformacion();
	//para ingresar la contraseña sin mostrarla
	void desactivarEcho();
	void activarEcho();

	void gotoXY(int x, int y);
	int leerOpcion(int cantidadDeOpciones, int posY);
	void leer(string& nombre);
};

#endif /* PROGRAMA_H_ */
