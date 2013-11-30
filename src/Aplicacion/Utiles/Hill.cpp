#include <iostream>
#include <algorithm>
#include "Hill.h"

int** Hill::crearMatriz(string clave){

	int filas = 3, cols = 3;
	int** matriz = new int*[filas];
	for (int i = 0; i < filas; ++i)
	    matriz[i] = new int[cols];

	return matriz;
}

string Hill::encriptar(string mensaje, string clave){

	mensaje = StringUtil::toUpper(mensaje);
	int** matriz = crearMatriz(clave);
	string tresLetras = "";

	for (int i= 0; i< mensaje.size(); i++){

		int posicion = getPosicion(alfabeto, 41, mensaje[i]);
	}



}

int Hill::getPosicion(char* alfabeto, int tamanio, char aBuscar){

	char* fin = alfabeto + tamanio;
	char* match = std::find(alfabeto, fin, *aBuscar);
	return(fin == match)? -1 : (match-alfabeto);
}


