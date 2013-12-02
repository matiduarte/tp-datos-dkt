#include <iostream>
#include <algorithm>
#include "Hill.h"
#include <math.h>
#include <stdio.h>

const char* Hill::getAlfabeto(){
	return alfabeto;
}
float** Hill::crearMatriz(string clave){

	float** matriz = new float*[3];
	for (int i = 0; i < 3; ++i)
	    matriz[i] = new float[3];

	//convierto de letra a numeros del 0 al 41
	int claveNumerica[9];
	for(int i=0; i<9; i++){
		claveNumerica[i] = getPosicion(getAlfabeto(), 41, clave[i]);
	}

	//guardo las 9 letras de la clave en la matriz
	for(int i=0; i< 3; i++){
		for(int j=0; j<3; j++){
			matriz[i][j] = claveNumerica[3*i + j];
		}
	}

	return matriz;
}

bool Hill::claveValida(string clave){

	if(clave.size() != 9) return false;

	clave = StringUtil::toUpper(clave);

	float** matriz = crearMatriz(clave);

	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
			if(matriz[i][j] == -1) return false;

	float det = calcularDeterminante(matriz);

	return (det!=0)? true : false;
}

float Hill::calcularDeterminante(float** matriz){
	//DET  =  a11(a33a22-a32a23)-a21(a33a12-a32a13)+a31(a23a12-a22a13)

	float aux1 = matriz[2][2]*matriz[1][1] - matriz[2][1]*matriz[1][2];
	float aux2 = matriz[2][2]*matriz[0][1] - matriz[2][1]*matriz[0][2];
	float aux3 = matriz[1][2]*matriz[0][1] - matriz[1][1]*matriz[0][2];

	float det = matriz[0][0]* aux1 - matriz[1][0]* aux2 + matriz[2][0]* aux3;

	return det;
}

string Hill::encriptar(string mensaje, string clave){

	mensaje = StringUtil::toUpper(mensaje);
	clave = StringUtil::toUpper(clave);
	float** matriz = crearMatriz(clave);

	float* vector3 = new float[3];
	string mensajeEncriptado = "";
	string mensajePlanoValido = "";

	//quito los caracteres invalidos
	for (unsigned int i= 0; i< mensaje.size(); i++){
		int posicion = getPosicion(getAlfabeto(), 41, mensaje[i]);
		if(posicion != -1){ //si la letra es valida la agrego a mensajePlanoValido

			mensajePlanoValido += mensaje[i];
		}
	}

	//agrego letras para que el tamaño sea multiplo de 3
	while(modulo(mensajePlanoValido.size(), 3) != 0)
		mensajePlanoValido += " ";

	int posActual=0;
	for (unsigned int i= 0; i< mensajePlanoValido.size(); i++){

		int posicion = getPosicion(getAlfabeto(), 41, mensajePlanoValido[i]);
		vector3[posActual] = posicion;
		posActual++;

		if(posActual == 3){ //con el vector lleno, multiplico con la matriz
							//y obtengo 3 letras de la clave
			posActual = 0;
			vector3 = multiplicar(matriz, vector3);

			for(int i = 0; i < 3; i++){
				int posicion = modulo(vector3[i], 41);
				mensajeEncriptado += alfabeto[posicion];
			}
		}
	}

	return mensajeEncriptado;
}

string Hill::desencriptar(string mensajeEncriptado, string clave){

	clave = StringUtil::toUpper(clave);
	float** matrizInversa = calcularInversa(crearMatriz(clave));
	float* vector3 = new float[3];
	string mensajePlano = "";

	int posActual=0;
	for (unsigned int i= 0; i< mensajeEncriptado.size(); i++){

		int posicion = getPosicion(getAlfabeto(), 41, mensajeEncriptado[i]);
		vector3[posActual] = posicion;
		posActual++;

		if(posActual == 3){ //con el vector lleno, multiplico con la matriz inversa
							//y obtengo 3 letras del mensaje plano
			posActual = 0;
			vector3 = multiplicar(matrizInversa, vector3);

			for(int i = 0; i < 3; i++){
				while(vector3[i]<0) vector3[i] += 41;
				int posicion = modulo(vector3[i], 41);
				mensajePlano += alfabeto[posicion];
			}
		}
	}

	return mensajePlano;
}

float** Hill::calcularInversa(float** matriz){

	float** matrizInversa = new float*[3];

	float temporary, r;
	int i, j, k, temp;
	int dimension = 3;
	float matrizAuxiliar[dimension][2*dimension];

	//Creo la matriz de 3x3
	for (int i = 0; i < dimension; ++i)
		    matrizInversa[i] = new float[3];

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

		//Copio los elementos a la matriz inversa
		for (i = 0; i < dimension; i++) {
			for (j = dimension; j < 2 * dimension; j++)
				matrizInversa[i][j - dimension] = matrizAuxiliar[i][j];
		}

	    return matrizInversa;
}

float* Hill::multiplicar(float** matriz, float* vector){

	float* vector3 = new float[3];

	vector3[0] = matriz[0][0]*vector[0] + matriz[0][1]*vector[1] + matriz[0][2]*vector[2];
	vector3[1] = matriz[1][0]*vector[0] + matriz[1][1]*vector[1] + matriz[1][2]*vector[2];
	vector3[2] = matriz[2][0]*vector[0] + matriz[2][1]*vector[1] + matriz[2][2]*vector[2];

	return vector3;
}

int Hill::getPosicion(const char* alfabeto, int tamanio, char aBuscar){

	const char* fin = alfabeto + tamanio;
	const char* match = std::find(alfabeto, fin, aBuscar);
	return(fin == match)? -1 : (match-alfabeto);
}

int Hill::modulo(int numero, int mod){

	return(numero%mod);
}

void Hill::mostrarMatriz(float** matriz){
	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++)
			cout<<matriz[i][j]<<" ";

		cout<<endl;
	}
}
