#include "Diccionario.h"

Diccionario::Diccionario(){
	this->cargarDiccionario();
}

void Diccionario::cargarDiccionario(){
	ifstream archivoPalabras(rutaConfig_Dic.c_str());
	if(archivoPalabras.fail()){
		cout << "no cargo:" << this->stopWords.size()<<endl;
		return;
	}
    map<string, int> dicPalabras;

    string linea;
    int counter = 0;
    while(getline(archivoPalabras,linea)){
    	vector<string> palabras = StringUtil::split(linea,' ');
    	for(unsigned int i=0; i < palabras.size();i++){
    		dicPalabras.insert(make_pair(palabras.at(i), counter));
    		counter++;
    	}   	
    }
    this->stopWords = dicPalabras;
}

bool Diccionario::esStopWord(string palabra){
	if( this->stopWords.find(palabra) == this->stopWords.end() ) {
		return false;
	}
	return true;
}

Diccionario::~Diccionario(){
}