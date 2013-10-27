#ifndef INDICE_H_
#define INDICE_H_

#include "../../CapaLogica/HashExtensible/Hash.h"
#include "../../CapaLogica/ArbolBMas/ArbolBMas.h"
#include "../../CapaLogica/ListaInvertida/ListaInvertida.h"
#include "../../CapaLogica/HashExtensible/Convertidor.h"
#include "../Entidades/Usuario.h"
#include "../Entidades/Servicio.h"
#include <string.h>

using namespace std;

class Indice {

private:
	//Indices principales
	Hash* indiceUsuario;
	Hash* indiceServicio;
	Hash* indiceConsulta;

	//Indices secundarios
	ArbolBMas* indiceUsuarioPorProvincia;
	ArbolBMas* indiceUsuarioPorTipo;
	ArbolBMas* indiceServicioPorCategoria;
	ArbolBMas* indiceConsultaPorIdServicioIdUsuario;
	ArbolBMas* indiceConsultaPorIdServicioFechaHora;
	
	//Indice invertido
	ListaInvertida* indiceOcurrenciasTerminos;
	//Creo que necestio un arbol y un archivo para ir guardando los terminos que aparecen
public:
	Indice();
	~Indice();
	
	//Metodos de usuarios
	void agregarUsuario(Usuario* usuario);
	void modificarUsuario(Usuario* usuario);
	void elimininarUsuario(Usuario* usuario);
	
	//Metodos servicios
	void agregarServicio(Servicio* servicio);
	void eliminarServicio(Servicio* servicio);
};

#endif /*INDICE_H_*/
