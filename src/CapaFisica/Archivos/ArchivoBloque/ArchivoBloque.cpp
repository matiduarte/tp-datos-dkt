/*
 * ArchivoBloque.cpp
 *
 *  Created on: 12/10/2013
 *      Author: marian
 */

#include "ArchivoBloque.h"

ArchivoBloque::ArchivoBloque(string nombre, int tamanioBloque) {

	if (!esMultiplo(tamanioBloque)) throw new ExcepcionBloqueIncorrecto();

    archivo.open(nombre.c_str(), fstream::in | fstream::out | fstream::binary);
    if(!archivo) // si no existe, crear archivo nuevo
    	archivo.open(nombre.c_str(), fstream::in | fstream::out | fstream::binary | fstream::trunc);

    	nombreArchivo = (string)nombre;

    	this->tamanioBloque = tamanioBloque;

    	leerMapaDeBits();
}

ArchivoBloque::~ArchivoBloque() {

	escribirMapaBits();
	archivo.close();
}

bool ArchivoBloque::esMultiplo(int tamanio){

	double n1= tamanio / 512;
	double n2= log2(n1);

	return((n2 == static_cast<int>(n2)) && (tamanio>=512));
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
	archivoMapaBits.seekp(0, ios::beg);
	for(unsigned int i=0; i< vectorMapaBits.size(); i++)
		archivoMapaBits.write((char*)&vectorMapaBits[i], sizeof(char));

	archivoMapaBits.close();
}

int ArchivoBloque::escribir(char* bloque){

	int posicion = this->siguientePosicionLibre();
	archivo.seekp(posicion*tamanioBloque, ios::beg);
	archivo.write(bloque, tamanioBloque);

	return posicion;
}

int ArchivoBloque::siguientePosicionLibre(){

	unsigned int pos;
	for(pos= 0; pos<vectorMapaBits.size() && vectorMapaBits.at(pos)=='1'; pos++){};
	if(pos== vectorMapaBits.size())
	     vectorMapaBits.push_back('1');
	else vectorMapaBits.at(pos) = '1';

	return pos;
}

char* ArchivoBloque::leer(int numBloque){

	char* dato = new char[tamanioBloque];

	archivo.seekg(tamanioBloque * numBloque, ios::beg);
	archivo.read(dato, tamanioBloque);

	return dato;
}

int ArchivoBloque::getCantidadBloques(){

	int posicion = archivo.tellp();

	archivo.seekp(0, ios::end);
	int numBloques= (archivo.tellp() / tamanioBloque);

	archivo.seekp(posicion, ios::beg);

	return numBloques;
}

void ArchivoBloque::borrar(unsigned int numBloque){

	if(numBloque >= vectorMapaBits.size()) throw new ExcepcionBloqueInexistente();

	vectorMapaBits.at(numBloque) = '0' ;
}
