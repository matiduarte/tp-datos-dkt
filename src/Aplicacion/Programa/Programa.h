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
#include <iostream>
#include "../constantes.h"
#include "../Entidades/Usuario.h"
using namespace std;

typedef enum{
	MENU_PRINCIPAL,
	REGISTRO,
	INICIAR_SESION,
	OPCIONES_USUARIO,
	CONSULTA_SERVICIO,
	BUSQUEDA,
	RESULTADO,
	PUBLICAR,
	RESPONDER,
	BAJA_PRODUCTO,
	ADMINISTRACION,
	TERMINAR,
}estadoPrograma;

class Programa {
public:
	Programa();
	virtual ~Programa();
	void ejecutarPrograma();

private:
	unsigned int offsetY;
	unsigned int offsetX;
	void gotoXY(int x, int y);

	int leerOpcion(int cantidadDeOpciones);

	estadoPrograma menuPrincipal();
	estadoPrograma registro();
	estadoPrograma iniciarSesion();
	estadoPrograma menuOpciones();
	estadoPrograma consultarServicio();
	estadoPrograma buscarServicio();
	estadoPrograma emitirResultadoBusqueda();
	estadoPrograma publicarServicio();
	estadoPrograma responderPregunta();
	estadoPrograma bajaProducto();
	estadoPrograma administrar();
};

#endif /* PROGRAMA_H_ */
