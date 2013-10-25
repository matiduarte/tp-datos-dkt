/*
 * ArchivoRegVariable.cpp
 *
 *  Created on: 12/10/2013
 *      Author: marian
 */

#include "ArchivoRegVariable.h"

ArchivoRegVariable::ArchivoRegVariable(string nombre) {

    archivo.open(nombre.c_str(), fstream::in | fstream::out | fstream::binary);
    if(!archivo){ // si no existe, crear archivo nuevo
    	archivo.open(nombre.c_str(), fstream::in | fstream::out | fstream::binary | fstream::trunc);
    	cantidadRegistros = 0;
    	archivo.seekp(0, ios::beg);
    	archivo.write((char*)&cantidadRegistros, sizeof(cantidadRegistros));

    }else{ //el archivo ya existe
    	archivo.seekg(0, ios::beg);
    	archivo.read((char*)&cantidadRegistros, sizeof(cantidadRegistros));
    }

    nombreArchivo= nombre;
    leerEspaciosLibres();
}

ArchivoRegVariable::~ArchivoRegVariable() {
	archivo.seekp(0, ios::beg);
	archivo.write((char*)&cantidadRegistros, sizeof(cantidadRegistros));
	archivo.close();

	escribirEspaciosLibres();
}

void ArchivoRegVariable::leerEspaciosLibres(){

	string dirEspaciosLibres = nombreArchivo + "ListaEspaciosLibres";
	fstream espaciosLibres;
	espaciosLibres.open(dirEspaciosLibres.c_str(), fstream::in | fstream::out |  fstream::binary);
    if(!espaciosLibres) // si no existe, crear archivo nuevo
    	espaciosLibres.open(dirEspaciosLibres.c_str(), fstream::in | fstream::out |  fstream::binary | fstream::trunc);

    espacioLibre EL;
    espaciosLibres.seekg(0, ios::beg);
    espaciosLibres.read((char*)&EL.inicio, sizeof(EL.inicio));

    while(!espaciosLibres.eof()){
        espaciosLibres.read((char*)&EL.tamanio, sizeof(EL.tamanio));
    	vectorEspaciosLibres.push_back(EL);
        espaciosLibres.read((char*)&EL.inicio, sizeof(EL.inicio));
    }

    espaciosLibres.close();
}

void ArchivoRegVariable::escribirEspaciosLibres(){

	string dirEspaciosLibres = nombreArchivo + "ListaEspaciosLibres";
	fstream espaciosLibres;
	espaciosLibres.open(dirEspaciosLibres.c_str(), fstream::in | fstream::out |  fstream::binary);

	for(unsigned int i=0 ; i< vectorEspaciosLibres.size(); i++){
		if(vectorEspaciosLibres.at(i).tamanio > 0){
			espaciosLibres.write((char*)&vectorEspaciosLibres.at(i).inicio, sizeof(vectorEspaciosLibres.at(i).inicio));
			espaciosLibres.write((char*)&vectorEspaciosLibres.at(i).tamanio, sizeof(vectorEspaciosLibres.at(i).tamanio));
		}
	}
	espaciosLibres.close();
}

unsigned int ArchivoRegVariable::escribir(char* registro){

	bool espacioEncontrado = false;
	unsigned int posicionRegistro;
	unsigned int largoCadena = strlen(registro);
	unsigned int tamanioDato = sizeof(largoCadena) + largoCadena;

	for(unsigned int i=0; ((i< vectorEspaciosLibres.size()) && (!espacioEncontrado)) ; i++){
		if(vectorEspaciosLibres.at(i).tamanio >= tamanioDato){ //si el dato entra en uno de los espacios libres

			espacioEncontrado = true;
			posicionRegistro = vectorEspaciosLibres.at(i).inicio;

			//escribir el dato
			archivo.seekp(vectorEspaciosLibres.at(i).inicio, ios::beg);
			archivo.write((char*)&largoCadena, sizeof(largoCadena));
			archivo.write(registro, largoCadena);

			//dismunuir el tamaño del espacio libre
			vectorEspaciosLibres.at(i).inicio += tamanioDato;
			vectorEspaciosLibres.at(i).tamanio -= tamanioDato;
		}
	}
	if(!espacioEncontrado){ //si se recorrio el vector de espacios y no se encontro nada, escribir al final

		archivo.seekp(0, ios::end);
		posicionRegistro = archivo.tellp();

		archivo.write((char*)&largoCadena, sizeof(largoCadena));
		archivo.write(registro, largoCadena);
	}

	cantidadRegistros++;
	return posicionRegistro;
}

char* ArchivoRegVariable::sacar(unsigned int posicionBytes){

	unsigned int largoCadena;
	archivo.seekg(posicionBytes, ios::beg);
	archivo.read((char*)&largoCadena, sizeof(largoCadena));

	char* dato = new char[largoCadena];

	archivo.read(dato, largoCadena);

	borrar(posicionBytes);

	return dato;
}

void ArchivoRegVariable::borrar(unsigned int posRegistroBytes){

	for(unsigned int i=0; i< vectorEspaciosLibres.size(); i++){
		espacioLibre EL = vectorEspaciosLibres.at(i);
		if((EL.inicio <= posRegistroBytes) && (EL.inicio + EL.tamanio > posRegistroBytes))
			throw new ExcepcionPosicionEnEspacioLibre();
	}

	unsigned int largoCadena;
	archivo.seekg(posRegistroBytes, ios::beg);
	archivo.read((char*)&largoCadena, sizeof(largoCadena));

	espacioLibre espacio;

	espacio.inicio = posRegistroBytes;
	espacio.tamanio = sizeof(largoCadena) + largoCadena;

	vectorEspaciosLibres.push_back(espacio);
	cantidadRegistros--;
}

unsigned int ArchivoRegVariable::getCantidadRegistros(){

	return cantidadRegistros;
}
