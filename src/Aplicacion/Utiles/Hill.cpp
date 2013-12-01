#include <iostream>
#include <algorithm>
#include "Hill.h"
#include <math.h>

char alfabeto[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.?,-";

float** Hill::crearMatriz(string clave){

	float** matriz = new float*[3];
	for (int i = 0; i < 3; ++i)
	    matriz[i] = new float[3];

	//convierto de letra a numeros del 0 al 41
	int claveNumerica[9];
	for(int i=0; i<9; i++){
		claveNumerica[i] = getPosicion(alfabeto, 41, clave[i]);
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
		int posicion = getPosicion(alfabeto, 41, mensaje[i]);
		if(posicion != -1){ //si la letra es valida la agrego a mensajePlanoValido

			mensajePlanoValido += mensaje[i];
		}
	}

	//agrego letras para que el tamaño sea multiplo de 3
	while(modulo(mensajePlanoValido.size(), 3) != 0)
		mensajePlanoValido += " ";

	int posActual=0;
	for (unsigned int i= 0; i< mensajePlanoValido.size(); i++){

		int posicion = getPosicion(alfabeto, 41, mensajePlanoValido[i]);
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

		int posicion = getPosicion(alfabeto, 41, mensajeEncriptado[i]);
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

	////////MODIFICAR POR ELIMINACION GAUSSIANA

	float** matrizInversa = new float*[3];
	for (int i = 0; i < 3; ++i)
	    matrizInversa[i] = new float[3];

	int determinante = calcularDeterminante(matriz);

	matrizInversa[0][0] = (matriz[2][2]*matriz[1][1] - matriz[2][1]*matriz[1][2]);
	matrizInversa[0][1] = (matriz[2][1]*matriz[0][2] - matriz[2][2]*matriz[0][1]);
	matrizInversa[0][2] = (matriz[1][2]*matriz[0][1] - matriz[1][1]*matriz[0][2]);
	matrizInversa[1][0] = (matriz[2][0]*matriz[1][2] - matriz[2][2]*matriz[1][0]);
	matrizInversa[1][1] = (matriz[2][2]*matriz[0][0] - matriz[2][0]*matriz[0][2]);
	matrizInversa[1][2] = (matriz[1][0]*matriz[0][2] - matriz[1][2]*matriz[0][0]);
	matrizInversa[2][0] = (matriz[2][1]*matriz[1][0] - matriz[2][0]*matriz[1][1]);
	matrizInversa[2][1] = (matriz[2][0]*matriz[0][1] - matriz[2][1]*matriz[0][0]);
	matrizInversa[2][2] = (matriz[1][1]*matriz[0][0] - matriz[1][0]*matriz[0][1]);

	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++){
			matrizInversa[i][j] /= determinante;
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

int Hill::getPosicion(char* alfabeto, int tamanio, char aBuscar){

	char* fin = alfabeto + tamanio;
	char* match = std::find(alfabeto, fin, aBuscar);
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
