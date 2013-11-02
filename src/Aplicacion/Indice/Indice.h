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
#include "../Diccionario/Diccionario.h"
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
	

	//a partir de nombre y contraseña, buscar y devolver el usuario, error=true si no se encuentra
	//	Usuario* buscarUsuario(string nombre, string contrasena, bool &error);

	//necesito hacer:
	//un metodo que me devuelva una lista de preguntas / cotizaciones hechas a determinado usuario
	//un metodo para buscar productos segun: usuario / palabras clave / servicio / categoria
	//un metodo que me devuelva una lista de todos los usuarios en el sistema
	//
	//que los dos metodos de eliminar devuelvan un bool

	//creo que nada mas... con esto ya tengo para rato :P

	//Metodos de usuarios

	bool agregarUsuario(Usuario* usuario);
	void modificarUsuario(Usuario* usuario);
	void elimininarUsuario(Usuario* usuario);
	
	//Metodos servicios
	bool agregarServicio(Servicio* servicio);
	void agregarCategoriaServicio(Categoria* categoria, Servicio* servicio);
	void eliminarServicio(Servicio* servicio);

	//Metodos consulta
	bool agregarConsulta(Consulta* consulta);
	void modificarConsulta(Consulta* consulta);
	vector<string> parsearConsulta(string consulta);
};

#endif /*INDICE_H_*/
