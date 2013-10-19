/*
 * ArchivoRegVariable.cpp
 *
 *  Created on: 12/10/2013
 *      Author: marian
 */

#include "ArchivoRegVariable.h"

ArchivoRegVariable::ArchivoRegVariable(string nombre) {

    archivo.open(nombre.c_str(), fstream::in | fstream::out | fstream::binary);
    if(!archivo){
    	archivo.open(nombre.c_str(), fstream::in | fstream::out | fstream::binary | fstream::trunc);
    	archivo.write(0,sizeof(unsigned int));
    	this->cantidadRegistros = 0;

    }else{ //el archivo ya existe
    	archivo.seekg(0, ios::beg);
    	archivo.read((char*)&cantidadRegistros, sizeof(unsigned int));
    }

    leerEspaciosLibres();
}

ArchivoRegVariable::~ArchivoRegVariable() {

	archivo.close();
	escribirEspaciosLibres();
}

void ArchivoRegVariable::leerEspaciosLibres(){

	string dirEspaciosLibres = nombreArchivo + "ListaEspaciosLibres";
	ifstream espaciosLibres;
	espaciosLibres.open(dirEspaciosLibres.c_str(), fstream::binary);
    if(!espaciosLibres) // si no existe, crear archivo nuevo
    	espaciosLibres.open(dirEspaciosLibres.c_str(), fstream::binary | fstream::app);

    espacioLibre EL;
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

	//se borra para poder escribir el nuevo vector actualizado y que no queden espacios en blanco
	delete(dirEspaciosLibres.c_str());

	ofstream espaciosLibres;
	espaciosLibres.open(dirEspaciosLibres.c_str(), fstream::binary);

	for(unsigned int i=0 ; i< vectorEspaciosLibres.size(); i++){
		espaciosLibres.write((char*)&vectorEspaciosLibres.at(i).inicio, sizeof(vectorEspaciosLibres.at(i).inicio));
		espaciosLibres.write((char*)&vectorEspaciosLibres.at(i).tamanio, sizeof(vectorEspaciosLibres.at(i).tamanio));
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
			archivo.write((char*)&registro, largoCadena);

			//dismunuir el tamaño del espacio libre
			vectorEspaciosLibres.at(i).inicio += tamanioDato;
			vectorEspaciosLibres.at(i).tamanio -= tamanioDato;
		}
	}
	if(!espacioEncontrado){ //si se recorrio el vector de espacios y no se encontro nada, escribir al final

		archivo.seekp(0, ios::end);
		posicionRegistro = archivo.tellp();

		archivo.write((char*)&largoCadena, sizeof(largoCadena));
		archivo.write((char*)&registro, largoCadena);
	}

	return posicionRegistro;
}

char* ArchivoRegVariable::leer(unsigned int numRegistro){
	return new char;
}
void ArchivoRegVariable::borrar(unsigned int numRegistro){

}
unsigned int ArchivoRegVariable::getCantidadRegistros(){
	return 0;
}
