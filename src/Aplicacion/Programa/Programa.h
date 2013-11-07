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
#include <unistd.h>

using namespace std;

typedef enum{
	MENU_PRINCIPAL,
	REGISTRO_U,
	REGISTRO_A,
	REGISTRO_CAT,
	BAJA_ADMIN,
	BAJA_CAT,
	MOD_CAT,
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

	estadoPrograma menuPrincipal();
	estadoPrograma altaUsuario(string tipoUsuario);
	estadoPrograma modificarDatosUsuario(Usuario* &usuario);
	estadoPrograma iniciarSesion(Usuario* &usuario);
	estadoPrograma menuOpcionesUsuario(Usuario* &usuario);
	estadoPrograma consultarServicio(vector<Servicio*> &resultados);
	estadoPrograma publicarServicio(Usuario* &usuario);
	estadoPrograma emitirResultadoBusqueda(vector<Servicio*> &resultados, Usuario* &usuario);
	estadoPrograma responderPregunta();
	estadoPrograma opcionesUsuarioNormal(Usuario* &usuario);
	estadoPrograma opcionesUsuarioProveedor(Usuario* &usuario);
	estadoPrograma opcionesAdministrador(Usuario* &usuario);
	estadoPrograma generarNuevasCategorias();
	estadoPrograma recuperacion();
	estadoPrograma bajaCategoria();
	estadoPrograma bajaProducto();
	estadoPrograma bajaAdmin(Usuario* &usuario);
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
	vector<Categoria*> leerCategoriasDeArchivo(ifstream &archivo);
	void emitirInformacion();
	bool tieneEspacios(string dato);

	void emitirCategoriasDisponibles();
	//para ingresar la contraseña sin mostrarla
	void desactivarEcho();
	void activarEcho();

	void gotoXY(int x, int y);
	int leerOpcion(int cantidadDeOpciones, int posY);
	void leer(string& nombre);

	/* NECESITO:
	 * modificar al indica para que el obtenerID devuelva un int?
	 * poder modificar los emails
	 * listado de usuarios y de categorias, las necesito si o si !
	 * que todas las excepciones deriven de Excepcion
	 * agregarCategoria deberia chequear sobre los Nombres, no los IDs
	 * faltan metodos:
	 * bool eliminarCategoria(string nombreCategoria);
	 * void modificarCategoria(Categoria categoria);
	 * Categoria* buscarCategoria(string nombreCategoria);
	 *
	 * USUARIO: 	falta Publicar
	 * PROVEEDOR:	falta la opcion publicar falta responder y baja producto
	 * ADMIN:		falta modificarCategoria, moderarConsultas, listadoUsuarios
	 */
};

#endif /* PROGRAMA_H_ */
