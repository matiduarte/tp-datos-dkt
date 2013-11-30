#ifndef PEDIDOCOTIZACION_H_
#define PEDIDOCOTIZACION_H_

#include <string>
#include "../Utiles/StringUtil.h"
#include "../../Aplicacion/constantes.h"
#include "../../Aplicacion/LectorConfig/LectorConfig.h"

#include "../Utiles/FechaYHora.h"

using namespace std;

class PedidoCotizacion {

private:
	int id;
	int idServicio;
	int idUsuario;
	string pedido;
	string fechaPedido;
	string horaPedido;

public:
	PedidoCotizacion();
	~PedidoCotizacion();
	string serializar();
	void desSerializar(string aDeserealizar);
	
	//Getters
	int getId();
	int getIdServicio();
	int getIdUsuario();
	string getPedido();
	string getFechaPedido();
	string getHoraPedido();
	
	//Setters
	void setId(int id);
	void setIdServicio(int idServicio);
	void setIdUsuario(int idUsuario);
	void setPedido(string consulta);
	void setFechaPedido(string fecha);
	void setHoraPedido(string hora);
};

#endif
