#ifndef INDICE_H_
#define INDICE_H_

#include "../../CapaLogica/ListaInvertida/ListaInvertida.h"
#include "../../CapaLogica/HashExtensible/Hash.h"
#include "../../CapaLogica/ArbolBMas/ArbolBMas.h"
#include "../LectorConfig/LectorConfig.h"
#include "../../Aplicacion/constantes.h"
#include "../Diccionario/Diccionario.h"
#include "..//Utiles/StringUtil.h"
#include "../Entidades/Servicio.h"
#include "../Entidades/Consulta.h"
#include "../Entidades/Usuario.h"
#include "../Entidades/PedidoCotizacion.h"
#include <string.h>

using namespace std;

class Indice {

private:
	//Diccionario
	Diccionario* diccionario;
	
	//Indices principales
	Hash* indiceUsuario;
	Hash* indiceServicio;
	Hash* indiceConsulta;
	Hash* indicePedidoCotizacion;

	//Necesito otro hash para las categorias. No esta en el enunciado pero se necesita
	Hash* indiceCategorias;
	ListaInvertida* listaCategoriasPorServicio;
	
	Hash* indiceClaves;
	
	//Indices secundarios
	ArbolBMas* indiceUsuarioPorProvincia;
	ArbolBMas* indiceUsuarioPorTipo;
	ArbolBMas* indiceServicioPorCategoria;
	ArbolBMas* indiceServicioPorIdProveedor;
	ArbolBMas* indiceConsultaPorIdServicio;
	ArbolBMas* indiceConsultaPorIdUsuario;
	ArbolBMas* indicePedidoCotizacionPorIdServicio;
	ArbolBMas* indicePedidoCotizacionPorIdUsuario;
	
	ArbolBMas* indiceGeneralEntidades;
	ArbolBMas* indiceCategoriaPorNombre;
	
	//Este indice probablemente este de mas o hay que cambiarlo
	ArbolBMas* indiceConsultaPorIdServicioFechaHora;
	
	//Indice invertido y arbol de palabras para busqueda por terminos clave
	ListaInvertida* indiceOcurrenciasTerminos;
	ArbolBMas* indiceTerminosId;
	Hash* indiceTerminos;
	//Creo que necestio un arbol y un archivo para ir guardando los terminos que aparecen
	void agregarCadenaATerminosRelevantes(string cadena, string idServicio);
	void hidratarCategoriasDeServicio(Servicio* servicio);
	vector<string> parsearConsulta(string consulta);
	string obtenerNuevoId(string tipoId);
	
public:
	Indice();
	Indice(string ruta);
	~Indice();
	

	//Metodos de usuarios
	bool agregarUsuario(Usuario* usuario);
	void modificarUsuario(Usuario* usuario);
	bool elimininarUsuario(Usuario* usuario);
	Usuario* buscarUsuario(string dni, string contrasena, bool &error);
	Usuario* buscarUsuario(string dni, bool &error);
	
	//Metodos servicios
	bool agregarServicio(Servicio* servicio);
	
	bool agregarCategoriaServicio(Categoria* categoria, Servicio* servicio);
	bool eliminarServicio(Servicio* servicio);
	vector<Servicio*> buscarServiciosPorUsuario(Usuario* usuario);
	vector<Servicio*> buscarServiciosPorPalabrasClave(string query);
	vector<Servicio*> buscarServiciosCategoria(Categoria* categoria);

	
	//Meotodos categorias
	bool agregarCategoria(Categoria* categoria);
	bool eliminarCategoria(string nombreCategoria);
	void modificarCategoria(Categoria* categoria);
	Categoria* buscarCategoria(string nombreCategoria, bool &error);

	//Metodos consulta
	bool agregarConsulta(Consulta* consulta);
	void modificarConsulta(Consulta* consulta);
	vector<Consulta*> buscarConsultasHechasAUsuario(Usuario* usuario);
	vector<Consulta*> buscarConsultasPorServicio(Servicio* servicio);
	
	//Generales
	vector<Usuario*> obtenerTodosLosUsuarios();
	vector<Categoria*> obtenerTodasLasCategorias(bool& error);
	
	//Metodos consulta
	bool agregarPedidoCotizacion(PedidoCotizacion* pedido);
	//void modificarConsulta(Consulta* consulta);
	vector<PedidoCotizacion*> buscarPedidosCotizacionHechasAUsuario(Usuario* usuario);
	vector<PedidoCotizacion*> buscarPedidosCotizacionPorServicio(Servicio* servicio);

	bool agregarClaveSistema(string clave);
	string buscarClaveSistema(bool &error);
};

#endif /*INDICE_H_*/
