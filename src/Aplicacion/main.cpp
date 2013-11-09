#include "Programa/Programa.h"
#include "../Excepcion.h"

int main(int argc, char* argv[]){

	try{

		Programa* programa = new Programa();
		programa->ejecutar();
		delete programa;

	}catch(Excepcion &e){
		e.mostrarMensaje();
		cout<< "Se produjo un error inesperado, abortando..."<<endl;

		//escribo el vector de variables en el archivo config
		//haciendo delete al lector
		LectorConfig * lector = LectorConfig::getLector("");
		delete lector;
	}

	return 0;
}
