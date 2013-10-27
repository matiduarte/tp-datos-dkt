#ifndef CATEGORIA_H_
#define CATEGORIA_H_

#include <string>

using namespace std;

class Categoria {

private:
	int id;
	string nombre;
	string descripcion;
public:
	Categoria();
	~Categoria();
	string serializar();
	void desSerializar(string aDeserealizar);
	
	//Getters
	int getId();
	string getNombre();
	string getDescripcion();
	
	//Setters
	void setId(int id);
	void setNombre(string nombre);
	void setDescripcion(string apellido);
};

#endif /*USUARIO_H_*/
