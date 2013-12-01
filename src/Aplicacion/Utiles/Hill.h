
#ifndef HILL_H_
#define HILL_H_
#include <string.h>
#include "StringUtil.h"

using namespace std;

class Hill {

public:
	static string encriptar(string mensaje, string clave);
	static string desencriptar(string mensaje, string clave);

	//chequea que la clave sea de 9 letras, que sean todas validas y que el det != 0
	static bool claveValida(string clave);

private:
	static void mostrarMatriz(float** mat);
	static float** crearMatriz(string clave);
	static float* multiplicar(float** matriz, float* vector);
	static int modulo(int num, int modulo);
	static float calcularDeterminante(float** matriz);
	static float** calcularInversa(float** matriz);
	static int getPosicion(char* alfabeto, int tamanio, char aBuscar);

};

#endif /* HILL_H_ */
