
#ifndef HILL_H_
#define HILL_H_
#include <string.h>
#include "../constantes.h"
#include "StringUtil.h"

using namespace std;

class Hill {

public:
	static string encriptar(string mensaje, string clave);
	static string desencriptar(string mensaje, string clave);
	static char* alfabeto;
	//chequea que la clave sea de 9 letras, que sean todas validas y que el det y su inversa modular sean != 0
	static bool claveValida(string clave);
	static char* getAlfabeto();

private:
	static int modulo(int num, int modulo);
	static int** crearMatriz(string clave);
	static int* multiplicar(int** matriz, int* vector);
	static int calcularDeterminante(int** matriz);
	static int** calcularInversaModular(int** matriz, int modulo);
	static int getPosicion(const char* alfabeto, int tamanio, char aBuscar);

	static int invmod(int m,int n);
	static int pow(int a, int b, int mod);

	static void mostrarMatriz(int** matriz);
};

#endif /* HILL_H_ */
