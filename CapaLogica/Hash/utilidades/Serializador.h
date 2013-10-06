/*
 * Serializador.h
 *
 *  Created on: 08/05/2010
 *      Author: mike
 */

#ifndef SERIALIZADOR_H_
#define SERIALIZADOR_H_

#include <string>
#include <string.h>


using namespace std;

class Serializador {

public:
	//INT
	static void serializarInt(const int& valor, void* aSerializar);
	static int desSerializarInt(const void* aDesSerializar);

	//CHAR*
	/*
	static void serializarChar(const char* valor, void* aSerializar);
	static void desSerializarChar(const void* aDesSerializar, char* aSerializar);
	*/

	//STRING
	static void serializarString(const string& valor, void* aSerializar);
	static string desSerializarString(const void* aDesSerializar);

};

#endif /* SERIALIZADOR_H_ */
