#ifndef SERVICIO_H_
#define SERVICIO_H_

#include <string>
#include <vector>
#include "Categoria.h"

using namespace std;

class Servicio {

private:
	int id;
	int idProveedor;
	string nombre;
	string descripcion;
	string tipo;
	vector<Categoria*> categorias;
	
public:
	Servicio();
	~Servicio();
	string serializar();
	void desSerializar(string aDeserealizar);
	
	//Getters
	int getId();
	int getIdProveedor();
	string getNombre();
	string getDescripcion();
	string getTipo();
	vector<Categoria*> getCategorias();
	
	//Setters
	void setId(int id);
	void setIdProveedor(int idProveedor);
	void setNombre(string nombre);
	void setDescripcion(string apellido);
	void setTipo(string tipo);
	void setCategoria(Categoria* categoria);
};

#endif /*USUARIO_H_*/
