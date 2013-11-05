/*
 * LectorConfig.cpp
 *
 *  Created on: 25/10/2013
 *      Author: marian
 */

#include "LectorConfig.h"

LectorConfig* LectorConfig::pLector = 0;

LectorConfig* LectorConfig::getLector(string ruta){

	if(pLector == 0){ //si nunca se llamó lo creo
		 pLector = new LectorConfig(ruta);
	}
	return pLector;
}

LectorConfig::LectorConfig(string rutaArchivo) {

	tamanioLinea = 100;
	ifstream archivo;

	archivo.open(rutaArchivo.c_str());
	if(!archivo)
		throw ExcepcionArchivoInexistente();

	this->leerArchivoConfig(archivo);

	this->nombreArchivo = rutaArchivo;
	archivo.close();
}

LectorConfig::~LectorConfig() {

	FILE *archivo;
	archivo = fopen(nombreArchivo.c_str(),"w");

	for(unsigned int i=0; i< vectorDatos.size(); i++){

		string linea = vectorDatos.at(i).nombreVariable + '=' + vectorDatos.at(i).valorVariable;
		fprintf(archivo,"%s\n",linea.c_str());
	}

	fclose(archivo);
}

string LectorConfig::getValor(string nombre){

	unsigned int i;
	for(i = 0; (i< vectorDatos.size())&&(vectorDatos[i].nombreVariable != nombre); i++){};

	//si llego al final del vector, la variable buscada no existe
	if(i == vectorDatos.size()) throw ExcepcionVariableInexistente();

	return vectorDatos[i].valorVariable;
}

void LectorConfig::setValor(string nombreVariable, string nuevoValorVariable){

	bool variableEncontrada= false;

	for(unsigned int i=0; i<vectorDatos.size(); i++){
		if(this->vectorDatos.at(i).nombreVariable == nombreVariable){
			this->vectorDatos.at(i).valorVariable = nuevoValorVariable;
			variableEncontrada = true;
		}
	}
	if( !variableEncontrada ) throw ExcepcionVariableInexistente();
}

void LectorConfig::leerArchivoConfig(ifstream &archivo){

	Dato dato;
	char linea[tamanioLinea];

	archivo.getline(linea, tamanioLinea, '\n');

	while(!archivo.eof()){

		if(linea[0] != '#'){
			dato = parsearLinea(linea);
			if(dato.nombreVariable != "#ERROR#") vectorDatos.push_back(dato);
		}
		archivo.getline(linea, tamanioLinea, '\n');

	};
}

int LectorConfig::stringToInt(const string& valor){
	int retorno;
	istringstream stream(valor);
	return stream>>retorno? retorno:-1;
}

string LectorConfig::intToString(int value){
	std::string s;
	std::stringstream out;
	out << value;
	s = out.str();

	return s;
}

Dato LectorConfig::parsearLinea(char linea[]){

	string nombreVariable= "";
	string valorVariable= "";
	Dato dato;
	int i;

	for(i=0; ((linea[i] != '=')&& (i< tamanioLinea)) ; i++)
		nombreVariable += linea[i];

	if( i== tamanioLinea) nombreVariable = "#ERROR#"; //no se encontro el signo de separacion

	for(int j=i+1 ; j < tamanioLinea; j++)
		valorVariable += linea[j];

	dato.nombreVariable = nombreVariable;
	dato.valorVariable = valorVariable;
	return dato;
}
