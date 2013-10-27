/*
 * ArchivoBloque.cpp
 *
 *  Created on: 12/10/2013
 *      Author: marian
 */

#include "ArchivoBloque.h"

ArchivoBloque::ArchivoBloque(string nombre) {

	try{
		LectorConfig* lector = LectorConfig::getLector(rutaConfig_ArchivoBloque);
    	this->tamanioBloque = lector->stringToInt(lector->getValor("tamanioBloque"));
	}
	catch(Excepcion& e){

		this->tamanioBloque = TAMANIOBLOQUE_DEFAULT;
	}


	if (!esMultiplo(tamanioBloque)) throw new ExcepcionBloqueIncorrecto();

    archivo.open(nombre.c_str(), fstream::in | fstream::out | fstream::binary);
    if(!archivo){ // si no existe, crear archivo nuevo
    	archivo.open(nombre.c_str(), fstream::in | fstream::out | fstream::binary | fstream::trunc);
    	cantidadBloques=0;
    }else{

    	archivo.seekg(0, ios::beg);
    	archivo.read((char*)&cantidadBloques, sizeof(cantidadBloques));
    }

    	nombreArchivo = (string)nombre;

    	leerEspaciosLibres();
}

ArchivoBloque::~ArchivoBloque() {

	escribirEspaciosLibres();
	archivo.seekp(0, ios::beg);
	archivo.write((char*)&cantidadBloques, sizeof(cantidadBloques));
	archivo.close();
}

bool ArchivoBloque::esMultiplo(int tamanio){

	double n1= tamanio / 512;
	double n2= log2(n1);

	return((n2 == static_cast<int>(n2)) && (tamanio>=512));
}

void ArchivoBloque::leerEspaciosLibres(){

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

void ArchivoBloque::escribirEspaciosLibres(){

	string dirMapaBits = nombreArchivo + "MapaBits";
	ofstream archivoMapaBits;
	archivoMapaBits.open(dirMapaBits.c_str(), fstream::binary);
	archivoMapaBits.seekp(0, ios::beg);
	for(unsigned int i=0; i< vectorMapaBits.size(); i++)
		archivoMapaBits.write((char*)&vectorMapaBits[i], sizeof(char));

	archivoMapaBits.close();
}

unsigned int ArchivoBloque::escribir(char* bloque){

	if(strlen(bloque) > tamanioBloque) throw ExcepcionOverflowTamBloque();

	unsigned int posicion = this->siguientePosicionLibre();
	archivo.seekp(sizeof(cantidadBloques) + posicion*tamanioBloque, ios::beg);
	archivo.write(bloque, tamanioBloque);
	cantidadBloques++;

	return posicion;
}

void ArchivoBloque::reescribir(char* bloque, unsigned int posicion){

	if(strlen(bloque) > tamanioBloque) throw ExcepcionOverflowTamBloque();
	if(posicion >= vectorMapaBits.size()) throw ExcepcionBloqueInexistente();

	archivo.seekp(sizeof(cantidadBloques) + posicion*tamanioBloque, ios::beg);
	archivo.write(bloque, tamanioBloque);
}

unsigned int ArchivoBloque::siguientePosicionLibre(){

	unsigned int pos;
	for(pos= 0; pos<vectorMapaBits.size() && vectorMapaBits.at(pos)=='1'; pos++){};
	if(pos== vectorMapaBits.size())
	     vectorMapaBits.push_back('1');
	else vectorMapaBits.at(pos) = '1';

	return (pos + sizeof(unsigned int));
}

void ArchivoBloque::leer(char* &bloque, unsigned int numBloque){

	archivo.seekg(sizeof(cantidadBloques) + numBloque*tamanioBloque, ios::beg);
	archivo.read(bloque, tamanioBloque);
}

unsigned int ArchivoBloque::getCantidadBloques(){
	return cantidadBloques;
}

unsigned int ArchivoBloque::getTamanoBloque(){
	return tamanioBloque;
}

void ArchivoBloque::borrar(unsigned int numBloque){

	if(numBloque >= vectorMapaBits.size()) throw ExcepcionBloqueInexistente();

	vectorMapaBits.at(numBloque) = '0' ;
	cantidadBloques--;
}
