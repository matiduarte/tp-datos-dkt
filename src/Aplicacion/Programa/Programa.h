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
#include "../Utiles/Hill.h"
#include "time.h"


using namespace std;

typedef enum{
	MENU_PRINCIPAL,
	REGISTRO_U,
	REGISTRO_A,
	REGISTRO_CAT,
	BAJA_ADMIN,
	BAJA_CAT,
	MODIFICAR_CATEGORIA,
	LISTAR_CATEGORIAS,
	CAMBIAR_DATOS,
	CAMBIAR_CONTRASENA,
	INICIAR_SESION,
	OPCIONES_USUARIO,
	CONSULTA_SERVICIO,
	VER_MENSAJES,
	VER_USUARIOS,
	VER_PEDIDOS_COTIZACION,
	RESULTADOS,
	RESULTADO_DET,
	PUBLICAR,
	RESPONDER,
	BAJA_PRODUCTO,
	RECUPERAR_PASS,
	TERMINAR,
	MODERAR_MENSAJES,
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
	estadoPrograma emitirResultadoBusqueda(vector<Servicio*> &resultados, Servicio* &resultado, Usuario* &usuario);
	estadoPrograma responderPregunta(Usuario* &usuario);
	estadoPrograma opcionesUsuarioNormal(Usuario* &usuario);
	estadoPrograma opcionesUsuarioProveedor(Usuario* &usuario);
	estadoPrograma opcionesAdministrador(Usuario* &usuario);
	estadoPrograma generarNuevasCategorias();
	estadoPrograma recuperacion();
	estadoPrograma bajaCategoria();
	estadoPrograma bajaProducto(Usuario* &usuario);
	estadoPrograma bajaAdmin(Usuario* &usuario);
	estadoPrograma listadoUsuarios();
	estadoPrograma listarCategorias();
	estadoPrograma detalleResultado(Servicio* &resultado, Usuario* &usuario);
	estadoPrograma moderarMensajes();
	estadoPrograma modificarCategoria();
	estadoPrograma verPedidosCotizacion(Usuario* &usuario);
	estadoPrograma cambiarContrasena();


	vector<Servicio*> buscarServicio(int opcion);

	bool eliminarUsuario(Usuario* usuario);
	bool existeCategoria(string categoria);
	string imprimirTipoDeUsuario(string tipo);
	void emitirDatosUsuario(Usuario* &usuario);
	string modificar(string queCosa, string valorActual, int posicionDato);

	void emitirResultado(Servicio* &resultado, int &posY, bool enDetalle);
	void emitirPreguntasRespuestasServicio(Servicio* &servicio, int &posY);
	void hacerPregunta(Servicio* &resultado, Usuario* &usuario, int posY);
	void pedirCotizacion(Servicio* &resultado, Usuario* &usuario, int posY);

	void cargaManualCategoria();
	void cargaMasivaCategoria();
	Categoria* buscarCategoria(string categoria);
	vector<Categoria*> leerCategoriasDeArchivo(ifstream &archivo);
	void emitirInformacion();
	bool tieneEspacios(string dato);

	void emitirCategoriasDisponibles(vector<Categoria*> categorias);
	//para ingresar la contraseña sin mostrarla
	void desactivarEcho();
	void activarEcho();

	void gotoXY(int x, int y);
	int leerOpcion(int cantidadDeOpciones, int posY);
	void leer(string& nombre);
	void leerNombreCategoria(string& nombre);
	void leerDescripcionCategoria(string& descripcion);
	void emitir(string texto, int posX, int &posY);
	void esperarEnter();
	string obtenerClaveDelSistema();
	void generarClave();


};

#endif /* PROGRAMA_H_ */
