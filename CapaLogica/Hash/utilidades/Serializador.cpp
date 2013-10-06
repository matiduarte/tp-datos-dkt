/*
 * Serializador.cpp
 *
 *  Created on: 08/05/2010
 *      Author: mike
 */

#include "Serializador.h"

void Serializador::serializarInt(const int& valor, void* aSerializar){
	((int*)aSerializar)[0] = valor;
}

int Serializador::desSerializarInt(const void* aDesSerializar){
	return ((int*)aDesSerializar)[0];
}

void Serializador::serializarString(const string& valor, void* aSerializar){
	memcpy(aSerializar, valor.c_str(), valor.length());
}

string Serializador::desSerializarString(const void* aDesSerializar){
	string retorno = (char*)aDesSerializar;
	return retorno;
}
