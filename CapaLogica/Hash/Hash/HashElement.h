#ifndef HASHELEMENT_H_
#define HASHELEMENT_H_

#include "Serializable.h"
#include "Convertidor.h"
#include "HashClave.h"
#include <string>

using namespace std;

class HashElement : public Serializable {

private:
	HashClave* clave;
	string valor;

public:
	HashElement(const string aDesSerializar);
	HashElement(const HashClave& key, const string val);
	HashElement(const HashElement& old);
	virtual ~HashElement();
	virtual string serializar()const;
	friend ostream& operator<< (ostream& out,const HashElement& ptrObj);

	virtual bool operator== (const HashElement& other) const;
	virtual bool operator==  (const HashClave& key) const;


	const string& getValor() const;
	void setValor(const string& nvoValor);
	const HashClave& getClave()const;

private:
	virtual void desSerializar(const string data);
};

#endif /* HASHELEMENT_H_ */
