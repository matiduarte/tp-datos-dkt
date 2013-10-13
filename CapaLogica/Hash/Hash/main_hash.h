#ifndef MAIN_HASH_H
#define MAIN_HASH_H
# include <getopt.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <iostream>
# include <fstream>
# include <list>
# include "Hash.h"


#define PARAM_ARCHIVO_CONFIG 1
#define PARAM_OPERADOR 2

void cargarConfig(const string pathArchivoConfig, string* pathArchivoTabla, string* pathArchivoNodos,
		int* tamBloqueTabla, int* tamBloqueNodo, string* pathArchivoExport, bool* sobreEscribir);

void comandoBusqueda(const string clave, Hash* hash);
void comandoIngresar(const string clave, const string valor, Hash* hash);
void comandoModificar(const string clave, const string valor, Hash* hash);
void comandoQuitar(const string clave, Hash* hash);
void comandoExportar(const string archivoExportar, Hash* hash);
void comandoHelp(const string nombre_ejecutable);
void uso(const string nombre);

void validarTamBloque(const int tamBloqueNodos,const int tamBloqueTabla);

string parsearLinea(string linea);
int stringToInt(string valor);


#endif
