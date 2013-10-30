#ifndef USUARIO_H_
#define USUARIO_H_

#include <string>
#include <vector>
#include "..//Utiles/StringUtil.h"
#include "../../Aplicacion/constantes.h"

using namespace std;

class Usuario {

private:
	int dni;
	string nombre;
	string apellido;
	string provincia;
	string contrasena;
	string tipo;
	vector<string> emails;
public:
	Usuario();
	~Usuario();
	string serializar();
	void desSerializar(string aDeserealizar);
	
	//Getters
	int getDni();
	string getNombre();
	string getApellido();
	string getProvincia();
	string getContrasena();
	string getTipo();
	vector<string> getEmails();
	
	//Setters
	void setDni(int dni);
	void setNombre(string nombre);
	void setApellido(string apellido);
	void setProvincia(string provincia);
	void setContrasena(string contrasena);
	void setTipo(string tipo);
	void setEmail(string email);
	
	
};

#endif /*USUARIO_H_*/
