#ifndef CONSTANTES_H_
#define CONSTANTES_H_

#include <string>
using namespace std;

struct registro{
	int codigo;
	char* nombre;
};

enum COMANDO{
	CARGA_MASIVA,
	LISTADO_ENTIDADES,
	BUSCAR_CLAVE,
	ORDENAR_CLAVE,
	ORDENAR_NOMBRE,
	AYUDA,
};

const string RUTA_ARCHIVOS_DAT = "data/";

const unsigned int MAX_REGISTROS_SORT = 100;

const char SEPARADOR_CAMPO = ',';
const unsigned int LONG_MAX_NOMBRE = 200;
const unsigned int BUFFER_DEFAULT = 2048;
const char FIN_LINEA = '\n';

#endif
