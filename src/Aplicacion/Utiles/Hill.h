
#ifndef HILL_H_
#define HILL_H_
#include <string.h>
#include "StringUtil.h"

using namespace std;

class Hill {

public:
	static string encriptar(string mensaje, string clave);
	static string desencriptar(string mensaje, string clave);
	static bool chequearClave(string clave);

private:
	static int** crearMatriz(string clave);
	static int getPosicion(char alfabeto, int tamanio, char aBuscar);

	static char alfabeto = {'_', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
	'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', '?', ',', '-'};
};

#endif /* HILL_H_ */
