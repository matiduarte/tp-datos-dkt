#ifndef HashClave_H_
#define HashClave_H_

using namespace std;

//#include <string>
#include "Serializable.h"
//#include "ElementKey.h"

class HashClave : public Serializable {
	
public:
	HashClave();
	HashClave(string valor, bool desSerializar);
//HashClave(const string aDesSerializar);
	//HashClave(const HashClave& elKey); //Constructor Copia
	virtual ~HashClave();

	bool operator==  (const HashClave& other) const;
	bool operator!=  (const HashClave& other) const;
	virtual string serializar()const;
	friend ostream& operator<< (ostream& out, const HashClave& ptrObj);
	int hashear(const int& mod)const;

private:
	string key;
	virtual void desSerializar(const string aDesSerializar);

};

#endif /* HashClave_H_ */
