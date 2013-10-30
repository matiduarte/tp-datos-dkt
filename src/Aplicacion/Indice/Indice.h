#ifndef INDICE_H_
#define INDICE_H_

#include "../../CapaLogica/HashExtensible/Hash.h"
#include "../../CapaLogica/ArbolBMas/ArbolBMas.h"
#include "../../CapaLogica/ListaInvertida/ListaInvertida.h"
#include "..//Utiles/StringUtil.h"
#include "../Entidades/Usuario.h"
#include "../Entidades/Servicio.h"
#include "../Entidades/Consulta.h"
#include "../../Aplicacion/constantes.h"
#include <string.h>

using namespace std;

class Indice {

private:
	//Indices principales
	Hash* indiceUsuario;
	Hash* indiceServicio;
	Hash* indiceConsulta;

	//Necesito otro hash para las categorias. No esta en el enunciado pero se necesita
	Hash* indiceCategorias;
	ListaInvertida* listaCategoriasPorServicio;
	
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
	void agregarCategoriaServicio(Categoria* categoria, Servicio* servicio);
	void eliminarServicio(Servicio* servicio);

	//Metodos consulta
	void agregarConsulta(Consulta* consulta);
	void modificarConsulta(Consulta* consulta);
};

#endif /*INDICE_H_*/
