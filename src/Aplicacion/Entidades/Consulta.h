#ifndef CONSULTA_H_
#define CONSULTA_H_

#include <string>
#include "../Utiles/StringUtil.h"
#include "../../Aplicacion/constantes.h"
#include "../../Aplicacion/LectorConfig/LectorConfig.h"

#include "../Utiles/FechaYHora.h"

using namespace std;

class Consulta {

private:
	int id;
	int idServicio;
	int idUsuario;
	string consulta;
	string fechaConsulta;
	string horaConsulta;
	string respuesta;
	string fechaRespuesta;
	string horaRespuesta;
	bool oculta;

public:
	Consulta();
	~Consulta();
	string serializar();
	void desSerializar(string aDeserealizar);
	
	//Getters
	int getId();
	int getIdServicio();
	int getIdUsuario();
	string getConsulta();
	string getFechaConsulta();
	string getHoraConsulta();
	string getRespuesta();
	string getFechaRespuesta();
	string getHoraRespuesta();
	bool getOculta();
	
	//Setters
	void setIdServicio(int idServicio);
	void setIdUsuario(int idUsuario);
	void setConsulta(string consulta);
	void setRespuesta(string respuesta);
	void setFechaRespuesta(string fechaRespuesta);
	void setHoraRespuesta(string horaRespuesta);
	void setOculta(bool oculta);
};

#endif
