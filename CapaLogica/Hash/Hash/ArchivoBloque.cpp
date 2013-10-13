#include "ArchivoBloque.h"

#include <iostream>
#include <fstream>

using namespace std;

/**
 * (Constructor ArchivoBloque)
 *
 * @params:
 * string r: ruta/nombre del archivo.
 * int t: tama�o de los bloques.
 * bool sobreEscribir: true si se desea pisar un archivo existente, false caso contrario.
 */
ArchivoBloque::ArchivoBloque(const string& r,const short& t,const bool& sobreEscribir )
:Archivo(r,sobreEscribir){
	tamBloque = t;
	buffer = new char[tamBloque+1];

}

/**
 * Se encarga de escribir un bloque en el ArchivoBloque.
 *
 * @params:
 * string bloque: bloque a escribir.
 * int numBloque: numero de bloque del archivo en donde se va a realizar la escritura.
 */
void ArchivoBloque::EscribirBloque(const string& bloque,const int& numBloque){
        //Calculo el tama�o de lo que voy a escribir
        if ((int)bloque.size() > tamBloque){
                throw ExceptionOverflowTamBloque();
                return;
        }
        archivo.seekp(numBloque*tamBloque);
        archivo.write(bloque.c_str(), tamBloque);

        archivo.flush();
}

/**
 * Se encarga de leer un bloque del ArchivoBloque.
 *
 * ACLARACION: no realiza ningun tipo de validaci�n sobre el rango del archivo y el numero de bloque a leer.
 *
 * @params:
 * int numBloque: numero de bloque a leer.
 */
const char* ArchivoBloque::LeerBloque(const int& numBloque){
        archivo.seekg(numBloque*tamBloque);

        int cantLeida=archivo.readsome(buffer, tamBloque);
        buffer[cantLeida+1] = '\0';
        return buffer;
}

/**
 * Obtiene la cantidad de bloques que hay en el archivo.
 */
const unsigned int ArchivoBloque::getCantidadDeBloques (){
	archivo.seekg (0, ios_base::end);
	return ( archivo.tellg () / tamBloque );
}

/**
 * (Destructor ArchivoBloque)
 *
 * Cierra el archivo utilizado.
 */
ArchivoBloque::~ArchivoBloque(){

	if (buffer) {
		delete[] buffer;
	}
}
