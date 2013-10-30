#ifndef SERIALIZABLE_H_
#define SERIALIZABLE_H_

using namespace std;
#include<string>
#include "Convertidor.h"

class Serializable {
public:
	Serializable(const string separador = "|");
	//Solo para cuando se crean objetos en memoria en primer instancia
	//despues tienen q usar siempre el constructor de arriba
	virtual ~Serializable();
	virtual string serializar()const  = 0;
protected:
	//Cte para separar los elementos
	string separadorElem;

	virtual void desSerializar(const string aDesSerializar)=0;


};

#endif /* SERIALIZABLE_H_ */
