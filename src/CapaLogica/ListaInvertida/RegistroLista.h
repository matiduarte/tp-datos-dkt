#ifndef REGISTROLISTA_H_
#define REGISTROLISTA_H_

#include "../HashExtensible/Serializable.h"
#include "../HashExtensible/Convertidor.h"
#include <string>

using namespace std;

class RegistroLista : public Serializable {

private:
	string idPalabra;
	string valor;

public:
	RegistroLista(const string aDeserealizar);
	RegistroLista(string idPalabra, string valor);
	virtual ~RegistroLista();
	virtual string serializar()const;
	string getValor();
private:
	virtual void desSerializar(const string data);
};

#endif
