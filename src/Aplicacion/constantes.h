/*
 * constantes.h
 *
 *  Created on: 26/10/2013
 *      Author: marian
 */

#ifndef CONSTANTES_H_
#define CONSTANTES_H_

#include <string.h>
using namespace std;

const unsigned int TAMANIOBLOQUE_DEFAULT = 1024;
const unsigned int TAMANIOCLAVE_DEFAULT = 50;
const unsigned int MAX_EMAILS = 3;
const unsigned int TAMANIO_CLAVE_SISTEMA = 9;

const string rutaConfig = "\Aplicacion/config";
const string rutaConfig_Dic = "\Aplicacion/Diccionario/stop-words-espanol";
const string rutaBaseIndice = "\Aplicacion/Indice/Archivos/";

const char separadorCamposClave = '#';
const char separadorCamposEntidades = '#';

const int offsetX = 6;
const int offsetY = 5;
const int MAX_Y = 18;
const unsigned int max_caracteres_pregunta = 140;
const unsigned int max_nombre_categoria = 100;
const unsigned int max_descr_categoria = 300;
const char separadorCategoria = ',';
const string idCategoria = "idUltimaCategoria";
const string idServicio  = "idUltimoServicio";
const string idConsulta = "idUltimaConsulta";
const string claveIndiceGeneralUsuarios = "Usuarios";
const string claveIndiceGeneralCategorias = "Categorias";

#endif /* CONSTANTES_H_ */
