#include "Programa/Programa.h"
#include "../Excepcion.h"


int main(int argc, char* argv[]){

	try{

		Programa* programa = new Programa();
		programa->ejecutarPrograma();
		delete programa;

	}catch(Excepcion &e){

	}

	return 0;
}
