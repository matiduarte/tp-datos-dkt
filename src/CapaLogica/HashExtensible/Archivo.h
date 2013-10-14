
#ifndef ARCHIVO_H_
#define ARCHIVO_H_

using namespace std;

#include <fstream>
#include <string>
#include <iostream>

class Archivo {

protected:
	fstream archivo;

public:
	Archivo(const string& r,const bool& sobreEscribir);
	fstream& getStream();
	virtual ~Archivo();
};

#endif /* ARCHIVO_H_ */
