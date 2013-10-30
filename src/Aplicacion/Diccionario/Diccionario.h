#ifndef DICCIONARIO_H_
#define DICCIONARIO_H_

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include "..//Utiles/StringUtil.h"
#include "../constantes.h"

using namespace std;

class Diccionario {

private:
	map<string, int> stopWords;
	void cargarDiccionario();
public:
	Diccionario();
	~Diccionario();
	
	bool esStopWord(string palabra);
};

#endif
