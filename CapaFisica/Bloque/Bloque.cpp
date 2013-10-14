#include "Bloque.h"

Bloque::Bloque() {
	tamanioBuffer = leerTamanioBuffer();
	buffer = new char[tamanioBuffer];
	bloquesLeidos = 0;
	posicionActual = sizeof(cantidadRegistros);
	cantidadRegistros = 0;
	memset(buffer, 0, tamanioBuffer);
}

Bloque::~Bloque() {
	delete[] buffer;
}

unsigned int Bloque::leerTamanioBuffer(){

	ifstream configLectura;
	ofstream configEscritura;
	unsigned int tamanioBuffer;

	configLectura.open("tamanioBuffer.txt");
	if(!configLectura){									//si no existe el archivo de tamaño de buffer mostrar mensaje,
		cout<<"No se encontró un archivo de configuración de buffer, procediendo a establecer el valor por defecto ("<<BUFFER_DEFAULT<<" bytes)"<<endl;
		configEscritura.open("tamanioBuffer.txt");		//crearlo,
		configEscritura<<BUFFER_DEFAULT;				//y escribirle el tamaño por default
		configEscritura.close();

		tamanioBuffer= BUFFER_DEFAULT;

	} else configLectura >> tamanioBuffer;				//si existe el archivo, leerlo

	configLectura.close();

//	if(!esMultiplo(tamanioBuffer)) throw new ExcepcionBufferIncorrecto();	//el numero debe ser multiplo de 512 bytes

	return tamanioBuffer;
}

bool Bloque::esMultiplo(unsigned int buffer){
	return (buffer%512 == 0);
}

void Bloque::vaciar(){
	//deja libres los primeros bytes para poder usarlos como campo de control de numero de registros
	posicionActual = sizeof(cantidadRegistros);
	cantidadRegistros = 0;
	memset(buffer, 0, tamanioBuffer);
}

bool Bloque::vacio(){
	return (cantidadRegistros <= 0);
}

void Bloque::subirAArchivo(Archivo& archivo){
	//antes de subirlo al .dat, escribir el num de registros que tenga
	memcpy(buffer, (char*)&cantidadRegistros, sizeof(cantidadRegistros));
	archivo.subir(buffer, tamanioBuffer);
}

bool Bloque::hayEspacio(registro* reg){

	unsigned int longitudNombre = strlen(reg->nombre);
	int bytesDisponibles = tamanioBuffer - posicionActual;

	int longitudRegistroBytes = sizeof(reg->codigo) + sizeof(longitudNombre) + longitudNombre;

	return (bytesDisponibles >= longitudRegistroBytes);
}

void Bloque::cargar(registro* reg){

	unsigned int longitudNombre = strlen(reg->nombre) +1; //+1 por el '\0'

	memcpy(&this->buffer[posicionActual], (char*)&reg->codigo, sizeof(reg->codigo));
	posicionActual += sizeof(reg->codigo);

	memcpy(&this->buffer[posicionActual], (char*)&longitudNombre, sizeof(longitudNombre));
	posicionActual += sizeof(longitudNombre);

	memcpy(&this->buffer[posicionActual], reg->nombre, longitudNombre);
	posicionActual += longitudNombre;

	this->cantidadRegistros += 1;
}

void Bloque::cargarDesdeArchivo(Archivo &archivo){

	archivo.leer(buffer, tamanioBuffer, bloquesLeidos);
	bloquesLeidos++;
	memcpy((char*)&cantidadRegistros, buffer, sizeof(cantidadRegistros));	//obtiene el numero de registros del bloque
	posicionActual = sizeof(cantidadRegistros);	//deja la posicion lista para comenzar a sacar registros
}

registro* Bloque::sacarSiguienteRegistro(){

	registro* reg = new registro;

	//saca el codigo del buffer
	memcpy(&reg->codigo, (char*)&buffer[posicionActual], sizeof(reg->codigo));
	posicionActual += sizeof(reg->codigo);

	//saca el longNombre del buffer
	unsigned int longitudNombre;
	memcpy(&longitudNombre, (char*)&buffer[posicionActual], sizeof(longitudNombre));
	posicionActual += sizeof(longitudNombre);

	//saca el nombre del buffer
	reg->nombre = new char[longitudNombre];
	memcpy(reg->nombre, (char*)&buffer[posicionActual], longitudNombre);
	posicionActual += longitudNombre;

	this->cantidadRegistros -= 1;

	return reg;
}

registro* Bloque::buscarClave(char* clave, bool& encontrado){

	registro* reg = new registro;

	unsigned int longitudNombre;
	int codigoActual;
	int codigoBuscado= atoi(clave);

	reg->codigo = codigoBuscado;

		for(unsigned int i = 0 ; (i < cantidadRegistros)&& !encontrado ; i++){

			//saca el codigo del buffer
			memcpy((char*)&codigoActual, (char*)&buffer[posicionActual], sizeof(codigoActual));
			posicionActual += sizeof(codigoActual);

			//saca el longNombre del buffer (para posible offset)
			memcpy((char*)&longitudNombre, (char*)&buffer[posicionActual], sizeof(longitudNombre));
			posicionActual += sizeof(longitudNombre);

			if(codigoActual == codigoBuscado){
				//si encuentro el codigo, copio el nombre
				reg->nombre = new char[longitudNombre];
				memcpy(reg->nombre, (char*)&buffer[posicionActual], longitudNombre);

				encontrado = true;
			}
				posicionActual += longitudNombre;	//si no encuentro el codigo, hago un offset para pasar al registro siguiente
		}

	return reg;
}
