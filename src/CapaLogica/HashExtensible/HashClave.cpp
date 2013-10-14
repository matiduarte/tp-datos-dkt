
#include "HashClave.h"

//HashClave::HashClave(const HashClave& elKey){
//	key = new ElementKey<string>(*(elKey.key));
//}

HashClave::HashClave(string valor,bool desSerealizar){
	if(desSerealizar){
		key="";
		desSerializar(valor);
	}else{
		key = valor;
	}
}

//HashClave HashClave::desSerializarHashClave(const string aDesSerializar):Serializable(){
//	key=NULL;
//	desSerializar(aDesSerializar);
//}

ostream& operator<< (ostream& os, const HashClave& ptrObj){
	os << "Clave :" << ptrObj.key;
	return os;
}


string HashClave::serializar()const{
	//return key.serializar();
	string strRetorno;
	ostringstream retorno;
	retorno << key;
	strRetorno = retorno.str();
	return strRetorno;
}

void HashClave::desSerializar(const string elemSerializado){
	key = elemSerializado;
}

bool HashClave::operator==  (const HashClave& other) const{
	return key == other.key;
}

bool HashClave::operator!=  (const HashClave& other) const{
	return key != other.key;
}

int HashClave::hashear(const int& mod)const{
	//usamos dispersion modular
	int asciTotal=0;
    for(unsigned int i=0;i< key.size();i++){
    	asciTotal += (int)key[i];
    }

    return (asciTotal%mod);
	//return ((key)%mod);
}

HashClave::~HashClave(){
	if (key != "") {
		//delete key;
	}
}
