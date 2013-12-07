#include <iostream>
#include <algorithm>
#include "Hill.h"
#include <math.h>
#include <stdio.h>

char* Hill::alfabeto = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.?,-";

char* Hill::getAlfabeto(){
	return alfabeto;
}
int** Hill::crearMatriz(string clave){

	int** matriz = new int*[TAMANIO_MATRIZ_HILL];
	for (unsigned int i = 0; i < TAMANIO_MATRIZ_HILL; ++i)
	    matriz[i] = new int[TAMANIO_MATRIZ_HILL];

	//convierto de letra a numeros del 0 al 41
	int claveNumerica[TAMANIO_CLAVE_SISTEMA];
	for(unsigned int i=0; i<TAMANIO_CLAVE_SISTEMA; i++){
		claveNumerica[i] = getPosicion(getAlfabeto(), strlen(alfabeto), clave[i]);
	}

	//guardo las 9 letras de la clave en la matriz
	for(unsigned int i=0; i< TAMANIO_MATRIZ_HILL; i++){
		for(unsigned int j=0; j<TAMANIO_MATRIZ_HILL; j++){
			matriz[i][j] = claveNumerica[TAMANIO_MATRIZ_HILL*i + j];
		}
	}

	return matriz;
}

bool Hill::claveValida(string clave){

	clave= StringUtil::toUpper(clave);

	if(clave.size() != TAMANIO_CLAVE_SISTEMA) return false;

	int** matriz = crearMatriz(clave);

	for(unsigned int i=0; i<TAMANIO_MATRIZ_HILL; i++)
		for(unsigned int j=0; j<TAMANIO_MATRIZ_HILL; j++)
			if(matriz[i][j] == -1) return false;

	int det = calcularDeterminante(matriz);
	int invDet = invmod(det, strlen(alfabeto));

	if(invDet==0) return false;
	if(det != 0) return true;

	return false;
}

int Hill::calcularDeterminante(int** matriz){
	//DET  =  a11(a33a22-a32a23)-a21(a33a12-a32a13)+a31(a23a12-a22a13)

	int aux1 = matriz[2][2]*matriz[1][1] - matriz[2][1]*matriz[1][2];
	int aux2 = matriz[2][2]*matriz[0][1] - matriz[2][1]*matriz[0][2];
	int aux3 = matriz[1][2]*matriz[0][1] - matriz[1][1]*matriz[0][2];

	int det = matriz[0][0]* aux1 - matriz[1][0]* aux2 + matriz[2][0]* aux3;

	return det;
}

string Hill::encriptar(string mensaje, string clave){

	mensaje = StringUtil::toUpper(mensaje);
	clave = StringUtil::toUpper(clave);
	int** matriz = crearMatriz(clave);
	int* vector3 = new int[TAMANIO_MATRIZ_HILL];
	string mensajeEncriptado = "";
	string mensajePlanoValido = "";

	//quito los caracteres invalidos
	for (unsigned int i= 0; i< mensaje.size(); i++){
		int posicion = getPosicion(getAlfabeto(), strlen(alfabeto), mensaje[i]);
		if(posicion != -1){ //si la letra es valida la agrego a mensajePlanoValido

			mensajePlanoValido += mensaje[i];
		}
	}

	//agrego letras para que el tamaño sea multiplo de 3
	while(modulo(mensajePlanoValido.size(), TAMANIO_MATRIZ_HILL) != 0)
		mensajePlanoValido += " ";

	int posActual=0;
	for (unsigned int i= 0; i< mensajePlanoValido.size(); i++){

		int posicion = getPosicion(getAlfabeto(), strlen(alfabeto), mensajePlanoValido[i]);
		vector3[posActual] = posicion;
		posActual++;

		if(posActual == TAMANIO_MATRIZ_HILL){ //con el vector lleno, multiplico con la matriz
							//y obtengo 3 letras de la clave
			posActual = 0;
			vector3 = multiplicar(matriz, vector3);

			for(unsigned int i = 0; i < TAMANIO_MATRIZ_HILL; i++){
				int posicion = modulo(vector3[i], strlen(alfabeto));
				mensajeEncriptado += alfabeto[posicion];
			}
		}
	}

	return mensajeEncriptado;
}

string Hill::desencriptar(string mensajeEncriptado, string clave){

	clave = StringUtil::toUpper(clave);
	int** matrizInversa = calcularInversaModular(crearMatriz(clave), strlen(alfabeto));
	int* vector3 = new int[TAMANIO_MATRIZ_HILL];
	string mensajePlano = "";

	int posActual=0;
	for (unsigned int i= 0; i< mensajeEncriptado.size(); i++){

		int posicion = getPosicion(getAlfabeto(), strlen(alfabeto), mensajeEncriptado[i]);
		vector3[posActual] = posicion;
		posActual++;

		if(posActual == TAMANIO_MATRIZ_HILL){ //con el vector lleno, multiplico con la matriz inversa
							//y obtengo 3 letras del mensaje plano
			posActual = 0;
			vector3 = multiplicar(matrizInversa, vector3);

			for(unsigned int i = 0; i < TAMANIO_MATRIZ_HILL; i++){
				int posicion = modulo(vector3[i], strlen(alfabeto));
				mensajePlano += alfabeto[posicion];
			}
		}
	}

	return mensajePlano;
}

int** Hill::calcularInversaModular(int** matriz, int mod){

	int** matrizInversa = new int*[TAMANIO_MATRIZ_HILL];

	float temporary, r;
	int i, j, k, temp;
	int dimension = TAMANIO_MATRIZ_HILL;
	float matrizAuxiliar[dimension][2*dimension];

	//Creo la matriz de 3x3
	for (int i = 0; i < dimension; ++i)
		    matrizInversa[i] = new int[TAMANIO_MATRIZ_HILL];

	//Agrego los elementos de matriz a la matriz auxiliar
	for (i = 0; i < dimension; i++)
	        for (j = 0; j < dimension; j++)
	            matrizAuxiliar[i][j] = matriz[i][j];


	//Armo la identidad para luego triangular
    for (i = 0; i < dimension; i++)
        for (j = dimension; j < 2 * dimension; j++)
            if (i == j % dimension)
                matrizAuxiliar[i][j] = 1;
            else
                matrizAuxiliar[i][j] = 0;

	//Eliminacion de gauss
	for (j = 0; j < dimension; j++) {
		temp = j;
		//busco el máximo elemento de la columna j-ésima
		for (i = j + 1; i < dimension; i++)
			if (matriz[i][j] > matriz[temp][j])
				temp = i;

		//intercambio de fila que tiene el máximo elemento de la columna j-ésima
		if (temp != j)
			for (k = 0; k < 2 * dimension; k++) {
				temporary = matrizAuxiliar[j][k];
				matrizAuxiliar[j][k] = matrizAuxiliar[temp][k];
				matrizAuxiliar[temp][k] = temporary;
			}

		for (i = 0; i < dimension; i++)
			if (i != j) {
				r = matrizAuxiliar[i][j];
				for (k = 0; k < 2 * dimension; k++)
					matrizAuxiliar[i][k] -= matrizAuxiliar[j][k] * r / matrizAuxiliar[j][j];
			} else {
				r = matrizAuxiliar[i][j];
				for (k = 0; k < 2 * dimension; k++)
					matrizAuxiliar[i][k] /= r;
			}
	    }

		int det = calcularDeterminante(matriz);
		int invdet = modulo(invmod(det, mod), mod);
		//Copio los elementos a la matriz inversa
		for (i = 0; i < dimension; i++) {
			for (j = dimension; j < 2 * dimension; j++)
				matrizInversa[i][j - dimension] = modulo(round(matrizAuxiliar[i][j]*det*invdet), mod);
		}
	    return matrizInversa;
}

int* Hill::multiplicar(int** matriz, int* vector){

	int* vector3 = new int[TAMANIO_MATRIZ_HILL];

	for(unsigned int i=0; i < TAMANIO_MATRIZ_HILL; i++){
		vector3[i] = 0 ;

		for(unsigned int j=0; j < TAMANIO_MATRIZ_HILL; j++){
			vector3[i] += matriz[i][j]*vector[j];
		}
	}

	return vector3;
}

int Hill::getPosicion(const char* alfabeto, int tamanio, char aBuscar){

	const char* fin = alfabeto + tamanio;
	const char* match = std::find(alfabeto, fin, aBuscar);
	return(fin == match)? -1 : (match-alfabeto);
}

int Hill::modulo(int numero, int mod){

	while(numero<0) numero +=mod;
	int resultado = (numero%mod);

	return resultado;
}

//calcula la inversa modular de un numero mediante el teorema de Fermat

int Hill::invmod(int a, int m) {
    return pow(a,m-2,m);
}

int Hill::pow(int a, int b, int MOD) {
int x = 1, y = a;
    while(b > 0) {
        if(b%2 == 1) {
            x=(x*y);
            if(x>MOD) x%=MOD;
        }
        y = (y*y);
        if(y>MOD) y%=MOD;
        b /= 2;
    }
    return x;
}
