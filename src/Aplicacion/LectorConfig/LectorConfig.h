/*
 * LectorConfig.h
 *
 *  Created on: 25/10/2013
 *      Author: marian
 */

#ifndef LECTORCONFIG_H_
#define LECTORCONFIG_H_

#include "Excepciones/ExcepcionArchivoInexistente.h"
#include "Excepciones/ExcepcionVariableInexistente.h"

#include <vector>
#include <string.h>
#include <fstream>
#include <sstream>

using namespace std;

typedef struct{
	string nombreVariable;
	string valorVariable;
}Dato;

class LectorConfig {

public:
	//se instancia un nuevo lector con el metodo getLector (asi se asegura el Singleton)
	static LectorConfig* getLector(string rutaArchivo);

	//le pasas como parametro el nombre de una variable dentro del archivo y te devuelve un string con su valor
	string getValor(string nombreVariable);
	void setValor(string nobreVariable, string valorVariable);
	//para convertir de string a int
	int stringToInt(const string& valor);
	virtual ~LectorConfig();

private:
	static LectorConfig* pLector;
	LectorConfig(string rutaArchivoConfig);
	string nombreArchivo;
	int tamanioLinea;
	vector<Dato> vectorDatos;
	void leerArchivoConfig(ifstream &archivo);
	Dato parsearLinea(char linea[]);
};

/*EJEMPLO DE ARCHIVO CONFIG:
 *
 * #si una linea empieza con numeral, es un comentario (no se parsea)
 *
 * #ejemplo de variable: nombreVariable=valorVariable
 *
 * tamanioBloque=2048
 * textoRandom=estaCadenaSeVaAParsearComoString
 * numeroDeNodos=10
 * mensajeError=No se pudo realizar el pedido
 *
 *
 * #comentario
 * #las variables string van sin comillas
 *
 * se carga el lector haciendo:
 *
 * LectorConfig* pLector = LectorConfig::getLector("rutaYNombreArchivoConfig");
 *
 * si quiero levantar la variable tamanioBloque hago:
 *
 * string tamanio = pLector->getValor("tamanioBloque");
 * int tamanioBloque = pLector->stringToInt(tamanio);
 *
 * string texto = pLector->getValor("mensajeError");
 *
 * cout<<texto; -----> No se pudo realizar el pedido
 */

#endif /* LECTORCONFIG */
