#include "Convertidor.h"

/**
 * Convierte un string a int.
 *
 * @params:
 * string* valor del string a convertir.
 * @return:
 * int con el valor de la cadena (-1 si no pudo convertirlo)
 */
int Convertidor::stringToInt(const string& valor){
	int retorno;
	istringstream stream(valor);
	return stream>>retorno? retorno:-1;
}

/**
 * Convierte un int a un string.
 *
 * @params:
 * int valor del entero a convertir.
 * @return:
 * string con el valor del entero.
 */
string Convertidor::intToString(const int& valor){
	string s;
	stringstream retorno;
	retorno << valor;
	s = retorno.str();
	return s;
}

