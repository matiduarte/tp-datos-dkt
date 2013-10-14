/*
 * ArchivoBloque.cpp
 *
 *  Created on: 12/10/2013
 *      Author: marian
 */

#include "ArchivoBloque.h"

ArchivoBloque::ArchivoBloque(string nombre) {

    archivo.open(nombre.c_str(), fstream::in | fstream::out | fstream::binary);
    if(!archivo) // si no existe, crear archivo nuevo
    	archivo.open(nombre.c_str(), fstream::in | fstream::out | fstream::binary | fstream::trunc);

    	nombreArchivo = (string)nombre;

    	leerMapaDeBits();
}

ArchivoBloque::~ArchivoBloque() {

	escribirMapaBits();
	archivo.close();
}

void ArchivoBloque::leerMapaDeBits(){

	string dirMapaBits = nombreArchivo + "MapaBits";
	ifstream archivoMapaBits;
	archivoMapaBits.open(dirMapaBits.c_str(), fstream::binary);
    if(!archivoMapaBits) // si no existe, crear archivo nuevo
    	archivoMapaBits.open(dirMapaBits.c_str(), fstream::binary | fstream::app);

    char bit;
    archivoMapaBits.read((char*)&bit, sizeof(char));
    while(!archivoMapaBits.eof()){
    	vectorMapaBits.push_back(bit);
    	archivoMapaBits.read((char*)&bit, sizeof(char));
    }

    archivoMapaBits.close();
}

void ArchivoBloque::escribirMapaBits(){

	string dirMapaBits = nombreArchivo + "MapaBits";
	ofstream archivoMapaBits;
	archivoMapaBits.open(dirMapaBits.c_str(), fstream::binary);
	archivoMapaBits.seekp(0);
	for(unsigned int i=0; i< vectorMapaBits.size(); i++)
		archivoMapaBits.write((char*)&vectorMapaBits[i], sizeof(char));

	archivoMapaBits.close();
}

void ArchivoBloque::subir(char* bloque, int tamanioBloque){

	int posicion = this->siguientePosicionLibre();
	archivo.seekp(posicion*tamanioBloque);
	archivo.write(bloque, tamanioBloque);
}

int ArchivoBloque::siguientePosicionLibre(){

	unsigned int pos;
	for(pos= 0; pos<vectorMapaBits.size() && vectorMapaBits.at(pos)=='1'; pos++){};
	if(pos== vectorMapaBits.size())
	     vectorMapaBits.push_back('1');
	else vectorMapaBits.at(pos) = '1';

	return pos;
}

void ArchivoBloque::leer(char* &dato, int tamanioDato, int numBloque){
	archivo.seekg(tamanioDato * numBloque);
	archivo.read(dato, tamanioDato);
}

void ArchivoBloque::borrarBloque(int numeroBloque){
	vectorMapaBits.at(numeroBloque) = '0';
}

