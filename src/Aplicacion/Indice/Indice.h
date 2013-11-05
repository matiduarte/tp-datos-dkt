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
	ArbolBMas* indiceServicioPorIdProveedor;
	//ArbolBMas* indiceConsultaPorIdServicioIdUsuario;
	ArbolBMas* indiceConsultaPorIdServicio;
	ArbolBMas* indiceConsultaPorIdUsuario;
	ArbolBMas* indiceConsultaPorIdServicioFechaHora;
	
	//Indice invertido y arbol de palabras para busqueda por terminos clave
	ListaInvertida* indiceOcurrenciasTerminos;
	ArbolBMas* indiceTerminosId;
	Hash* indiceTerminos;
	//Creo que necestio un arbol y un archivo para ir guardando los terminos que aparecen
	void agregarCadenaATerminosRelevantes(string cadena, string idServicio);
	string obtenerNuevoId(string tipoId);
	void hidratarCategoriasDeServicio(Servicio* servicio);
	
public:
	Indice();
	Indice(string ruta);
	~Indice();
	

	//necesito hacer:
	//un metodo que me devuelva una lista de todos los usuarios en el sistema
	//

	//Metodos de usuarios
	bool agregarUsuario(Usuario* usuario);
	void modificarUsuario(Usuario* usuario);
	bool elimininarUsuario(Usuario* usuario);
	Usuario* buscarUsuario(string dni, string contrasena, bool &error);
	
	//Metodos servicios
	bool agregarServicio(Servicio* servicio);
	void agregarCategoriaServicio(Categoria* categoria, Servicio* servicio);
	bool eliminarServicio(Servicio* servicio);
	vector<Servicio*> buscarServiciosPorUsuario(Usuario* usuario);
	vector<Servicio*> buscarServiciosPorPalabrasClave(string query);
	vector<Servicio*> buscarServiciosCategoria(Categoria* categoria);

	//Metodos consulta
	bool agregarConsulta(Consulta* consulta);
	void modificarConsulta(Consulta* consulta);
	vector<string> parsearConsulta(string consulta);
	vector<Consulta*> buscarConsultasHechasAUsuario(Usuario* usuario);
	vector<Consulta*> buscarConsultasPorServicio(Servicio* servicio);
};

#endif /*INDICE_H_*/
