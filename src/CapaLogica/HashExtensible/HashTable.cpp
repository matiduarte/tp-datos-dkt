#include "HashTable.h"

#include <iostream>

HashTable::HashTable(const string& ruta)
: Salvable(ruta){
	if (this->getCantidadDeBloques () == 0){
		//armo una nueva tabla (tabla vacia)...
		tamTabla = 0;
		//guardo en el tamano de la tabla...
		guardarTamTabla();
		//pongo en un estado valido la tabla, guardo en el primer bloque un elemento cero...
		setElement(0,0);
	} else {
	//levanta la tabla de disco y la cargar en memoria...
		string buf;
		leerBloque(0,&buf);
		tamTabla = Convertidor::stringToInt(buf);
	}
}

int HashTable::getElement(const HashClave& key){
	//hasheo la clave...
	int pos = key.hashear(tamTabla);
	//obtengo el bloque y devuelvo el valor...
	string buf;
	leerBloque(pos+1,&buf);
	return Convertidor::stringToInt(buf);
}

//PRIVADO
int HashTable::getElement(int pos){
	//obtengo el bloque y devuelvo el valor...
	string buf;
	leerBloque(pos+1,&buf);
	return Convertidor::stringToInt(buf);
}

void HashTable::setElement(int elem, const HashClave& key){
	if (tamTabla == 0){
		tamTabla++;
		guardarTamTabla();
	}
	//hasheo la clave...
	int pos = key.hashear(tamTabla);
	//guardo el bloque con el nuevo elemento...
	escribirBloque(Convertidor::intToString(elem), pos+1);
}

//PRIVADO
void HashTable::setElement(int elem, int pos){
	if (tamTabla == 0){
		tamTabla++;
		guardarTamTabla();
	}
	//guardo el bloque con el nuevo elemento...
	escribirBloque(Convertidor::intToString(elem), pos+1);
}


//DevolverNodoNuevo
//Verifico el tam de la tabla == dispersion del bloque rebalsado?
		// si es igual duplico la tabla
			//duplico la tabla, creo nuevo nodo
		//sino la tabla me devuelve busco la otra posicion en la tabla que apunta al mismo bloque, le creo un nuevo nodo vacio y hago q lo apunte
int HashTable::nodoNuevo(int tamDispersion, const HashClave& key){
	int nuevoNodo = buscarNodoVacio();
	int pos = key.hashear(tamTabla);
	if (tamTabla == tamDispersion){
		try {
			duplicarTabla();
		}catch (ExceptionTamTabla& e) {
			throw e;
		}
		setElement(nuevoNodo, pos);
	} else{
		recorrerActualizandoTabla(pos, tamDispersion*2, nuevoNodo);
	}
	return nuevoNodo;
}

int HashTable::nodoLibre(int tamDispersion, const HashClave& key){
	int pos = key.hashear(tamTabla);
	int salto = tamDispersion/2;
	int posAtras = pos-salto;
	if (posAtras<0){
		posAtras+=tamTabla;
	}
	int posAdelante = pos+salto;
	if (posAdelante>tamTabla){
		posAdelante-=tamTabla;
	}
	int retorno = -1;
	//si el tamTabla es 1, no puedo liberar nodo...
	if (tamTabla==1){
		return retorno;
	}
	//devuelvo -1 si no se hizo nada...
	if (getElement(posAtras) == getElement(posAdelante)){
		//creo un nuevo elemento con el valor de los elementos anteriores, que son iguales...
		int nuevoElem = getElement(posAtras);
		//recorro y actualizo la tabla...
		recorrerActualizandoTabla(pos, tamDispersion, nuevoElem);
		//si quedo una tabla espejo, la parto a la mitad...
		if (tablaEspejo()){
			reducirTabla();
		}
		//el valor a devolver es el num de nodo al que hay qe acutalizarle el tam de dispersion...
		retorno = nuevoElem;
	}
	return retorno;
}

int HashTable::buscarNodoVacio(){
	list<int> lista;
	for (int i=0; i<tamTabla; i++){
		int valor = getElement(i);
		lista.push_back(valor);
	}
	lista.sort();
	lista.unique();

	int primero = 0;
	list<int>::iterator it = lista.begin();
	bool encontrado = false;
	while ((!encontrado) && (it != lista.end())){
		if (primero != *it){
			encontrado = true;
		} else{
			primero++;
			it++;
		}
	}
	return primero;
}

void HashTable::duplicarTabla(){
	int fin = tamTabla;
	for (int i=0; i<fin; i++){
		int pos = i+1;
		string buf;
		leerBloque(pos,&buf);
		//guardo el bloque al final del archivo...
		escribirBloque(buf, tamTabla+pos);
	}
	//actualizo y guardo el nuevo tamTabla...
	tamTabla = tamTabla*2;
	guardarTamTabla();
}

void HashTable::reducirTabla(){
	//actualizo y guardo el nuevo tamTabla...
	tamTabla = tamTabla/2;
	guardarTamTabla();
}

void HashTable::recorrerActualizandoTabla(const int& posInicial, const int& salto, const int& nuevoElem){
	int i = posInicial;
	bool fin = false;
	//comenzando de posInicial, cada saltos del tamanio de salto, actualizo el elemento con nuevoElem.
	while (!fin){
		setElement(nuevoElem, i);
		i += salto;
		//si llegue al final de la tabla, vuelvo al ppio (recorrido circular de la tabla)
		if (i>=tamTabla){
			i -= tamTabla;
		}
		//si llegue a la pos inicial, es porque recorri toda la tabla (FIN)...
		if (i==posInicial){
			fin = true;
		}
	}
}

bool HashTable::tablaEspejo(){
	int mitad = tamTabla/2;
	int i=0;
	bool espejo = true;
	bool fin = false;
	while (!fin){
		//si encuentro un elemento diferente, ya NO es espejo (FIN)...
		if (getElement(i) != getElement(mitad+i)){
			espejo = false;
			fin = true;
		} else{
			i++;
			//si llegue a la mitad, recorri toda la tabla (FIN)...
			if (i==mitad){
				fin = true;
			}
		}
	}//fin while
	return espejo;
}

void HashTable::guardarTamTabla(){
	try {
		escribirBloque(Convertidor::intToString(tamTabla), 0);
	}catch (ExcepcionOverflowTamBloque& e){
		//cuando el tamTabla no entra para guardar, y hay qe agrandar el tam bloque de la tabla.
		throw ExceptionTamTabla();
	}
}

ostream& operator << (ostream& o, HashTable& tabla){
	o << "Tabla:\n\t";
	for (int i=0; i<tabla.tamTabla; i++){
		string buf;
		tabla.leerBloque(i+1,&buf);
		o << buf << " ";
	}
    return o;
}

HashTable::~HashTable(){
}
