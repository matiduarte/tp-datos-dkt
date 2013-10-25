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
	static LectorConfig* getLector(string rutaArchivo);
	string getValor(string nombreVariable);
	int stringToInt(const string& valor);

private:
	static LectorConfig* pLector;
	LectorConfig(string rutaArchivoConfig);
	virtual ~LectorConfig();

	int tamanioLinea;
	vector<Dato> vectorDatos;
	void leerArchivoConfig(ifstream &archivo);
	Dato parsearLinea(char linea[]);
};

#endif /* LECTORCONFIG */
