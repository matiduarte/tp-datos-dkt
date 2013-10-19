//#include "main_hash.h"
//
//using namespace std;
//
//int mainHash (int argc, char** argv){
//	//Se operarán a través de la consola con los siguientes parámetros:
//	//-­B = Buscar una clave (clave).
//	//-­I = Ingresar datos a la estructura (clave;valor).
//	//-­M = Modificar un dato ya existente en la estructura (clave;valor).
//	//-­Q = Quitar un dato de la estructura (clave).
//	//-­S = Volcar todos los datos de la estructura en un archivo de texto plano para que pueda
//	//		ser analizado en una observación inmediata (sin datos en binario).
//	//-­h = Ayuda para la operación con la aplicación.
//
//	if(argc == 1){
//		string s = argv[0];
//		uso(s); //ejecucion sin parametros
//		return 0;
//	}
//
//	const char* const opciones="hBIMQS";
//	int opcion = getopt_long(argc,argv,opciones,NULL,NULL);
//
//	if (opcion == 'h'){
//		comandoHelp(argv[0]);
//		return 0;
//	}
//
//	string pathArchivoConfig = argv[PARAM_ARCHIVO_CONFIG];
//
//	string pathArchivoTabla, pathArchivoNodos, pathArchivoExport;
//	int tamBloqueTabla, tamBloqueNodos;
//	bool sobreEscribir;
//
//	cargarConfig(pathArchivoConfig, &pathArchivoTabla, &pathArchivoNodos, &tamBloqueTabla, &tamBloqueNodos, &pathArchivoExport, &sobreEscribir);
//	//Validacion de tamBloque
//	validarTamBloque(tamBloqueNodos,tamBloqueTabla);
//
//	Hash hash(pathArchivoTabla, pathArchivoNodos, tamBloqueTabla, tamBloqueNodos, sobreEscribir);
//
//	switch(opcion){
//		case 'h':
//			comandoHelp(argv[0]);
//			return 0;
//		case 'B': {
//			string clave = argv[3];
//			comandoBusqueda(clave, &hash);
//			break;
//		}
//		case 'I': {
//			string clave = argv[3];
//			string valor = argv[4];
//			comandoIngresar(clave, valor, &hash);
//			break;
//		}
//		case 'M': {
//			string clave = argv[3];
//			string valor = argv[4];
//			comandoModificar(clave, valor, &hash);
//			break;
//
//		}
//		case 'Q': {
//			string clave = argv[3];
//			comandoQuitar(clave, &hash);
//			break;
//		}
//		case 'S': {
//			comandoExportar(pathArchivoExport, &hash);
//			break;
//		}
//		default: {
//			uso(argv[0]);
//			break;
//		}
//	}
//	return 0;
//}
//
//void cargarConfig(const string pathArchivoConfig, string* pathArchivoTabla, string* pathArchivoNodos,
//		int* tamBloqueTabla, int* tamBloqueNodos, string* pathArchivoExport, bool* sobreEscribir){
//
//	ifstream archivoConfig;
//	archivoConfig.open(pathArchivoConfig.c_str());
//	if (archivoConfig.fail()){
//		cout << "ERROR al abrir el archivo config." << endl;
//		return;
//	}
//	string linea;
//	archivoConfig >> linea;
//	*pathArchivoTabla = parsearLinea(linea);
//	archivoConfig >> linea;
//	*pathArchivoNodos = parsearLinea(linea);
//	archivoConfig >> linea;
//	*tamBloqueTabla = stringToInt(parsearLinea(linea));
//	archivoConfig >> linea;
//	*tamBloqueNodos = stringToInt(parsearLinea(linea));
//	archivoConfig >> linea;
//	*pathArchivoExport = parsearLinea(linea);
//	archivoConfig >> linea;
//	if (parsearLinea(linea) == "true")
//		*sobreEscribir = true;
//	else
//		*sobreEscribir = false;
//
//
//}
//
//int stringToInt(string valor){
//	int retorno;
//	istringstream stream(valor);
//	return stream>>retorno? retorno:-1;
//}
//
//string parsearLinea(string linea){
//	int pos = linea.find_first_of("=");
//	return linea.substr(pos+1, linea.size() - (pos+1));
//}
//
//// Imprime informacion sobre el uso desde linea de comandos
//void uso(const string nombre){
//	cout<<"Uso:\n\t"<<nombre<<" [path_archivo_configuracion] [opciones] [clave] [valor]"<<endl;
//	cout<<"Opciones:"<<endl;
//	cout<<"\t-h\t\tImprimir esta informacion y terminar."<<endl;
//	cout<<"\t-B\t\tBuscar una clave en la estructura."<<endl;
//	cout<<"\t-I\t\tIngresar datos a la estructura."<<endl;
//	cout<<"\t-M\t\tModificar un dato ya existente en la estructura."<<endl;
//	cout<<"\t-Q\t\tQuitar un dato de la estructura."<<endl;
//	cout<<"\t-S\t\tVolcar todos los datos de la estructura en un archivo de texto plano."<<endl;
//}
//
//void comandoHelp(const string nombre_ejecutable) {
//	cout<<endl;
//	cout<<"Formato de ejecucion: "<<nombre_ejecutable<<" [nombre_archivo_configuracion] [comando] [clave] [valor]"<<endl;
//	cout<<endl<<"Comandos:"<<endl;
//	cout<<"-I: para ingresar uno o mas datos por entrada estandar."<<endl;
//	cout<<"-B: para buscar el registro indicado por entrada estandar"<<endl;
//	cout<<"-M: para modificar el contenido del registro indicado por entrada estandar"<<endl;
//	cout<<"-Q: para eliminar el registro indicado por entrada estandar"<<endl;
//	cout<<"-S: para exportar todo el arbol en un archivo de texto"<<endl;
//	cout<<endl<<"Para mas informacion vea la documentacion adjunta"<<endl;
//	cout<<endl;
//}
//
//void comandoBusqueda(const string clave, Hash* hash){
//	try {
//		string valor = hash->buscarElemento(clave);
//		cout << "Resultado: (" << clave << ";" << valor << ")" << endl;
//	} catch (ExceptionElementoNoEncontrado e){
//		cout << "ERROR - " << e.what() << endl;
//	}
//}
//
//void comandoIngresar(const string clave, string valor, Hash* hash){
//	try {
//		hash->insertarElemento(clave, valor);
//		cout << "Insercion exitosa." << endl;
//	} catch (ExceptionElementoKeyYaIngresado e){
//		cout << "ERROR - " << e.what() << endl;
//	}
//}
//
//void comandoModificar(const string clave, string valor, Hash* hash){
//	try {
//		hash->modificarElemento(clave, valor);
//		cout << "Modificacion exitosa." << endl;
//	} catch (ExceptionElementoNoEncontrado e){
//		cout << "ERROR - " << e.what() << endl;
//	}
//}
//
//void comandoQuitar(const string clave, Hash* hash){
//	try {
//		hash->elminarElemento(clave);
//		cout << "Eliminacion exitosa." << endl;
//	} catch (ExceptionElementoNoEncontrado e){
//		cout << "ERROR - " << e.what() << endl;
//	}
//}
//
//void comandoExportar(const string archivoExportar, Hash* hash){
//	//Creo la salida
//	//Archivo archivo(archivoExportar);
//	//archivo.getStream() << *hash;
//	//cout << "Exportacion Exitosa." << endl;
//}
//
//void validarTamBloque(const int tamBloqueNodos,const int tamBloqueTabla){
//	//Relacion de 512*2 a la x bytes
//
//
//	//tamBloqueNodos/512
//
//}
