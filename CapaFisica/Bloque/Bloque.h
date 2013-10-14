#ifndef BUFFER_H_
#define BUFFER_H_

#include "../Excepciones/Buffer_Incorrecto/ExcepcionBufferIncorrecto.h"
#include "../../Constantes/Constantes.h"
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>

using namespace std;

class Bloque {

public:
	Bloque();
	virtual ~Bloque();

	bool vacio();
	void vaciar();
	void subirAArchivo(Archivo& archivo);
	bool hayEspacio(registro* reg);
	void cargar(registro* reg);
	void cargarDesdeArchivo(Archivo& archivo);
	registro* sacarSiguienteRegistro();
	registro* buscarClave(char* codigo, bool& encontrado);

private:
	char* buffer;
	int tamanioBuffer;
	int posicionActual;
	int bloquesLeidos;
	unsigned short int cantidadRegistros;
	unsigned int leerTamanioBuffer();
	bool esMultiplo(unsigned int tamanio);
};

#endif
