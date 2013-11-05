/*
 * FechaYHora.cpp
 *
 *  Created on: 04/11/2013
 *      Author: marian
 */

#include "FechaYHora.h"

FechaYHora::FechaYHora() {
}

FechaYHora::~FechaYHora() {
}

string FechaYHora::setFechaAAAAMMDD(){

    time_t     now = time(0);
    struct tm  tstruct;

    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y%m%d", &tstruct);

    return buf;
}

string FechaYHora::setHoraHHMM(){
    time_t     now = time(0);
    struct tm  tstruct;

    char       buf[80];

    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%X", &tstruct);

    string hora="";
    string min="";
    unsigned int i;

    for(i= 0; (i < strlen(buf))&&(buf[i]!= ':'); i++){
    	hora+=buf[i];
    }

    for(unsigned int j = i+1; (j < strlen(buf))&&(buf[j]!= ':'); j++){
    	min+=buf[j];
    }

    hora += min;

    string resultado(hora);

    return resultado;
}

string FechaYHora::getFecha_DD_MM_AAAA(string AAAAMMDD){

	int i;
	string anio= "";
	string mes = "";
	string dia = "";
	string resultado = "";

	for(i= 0; i < 4; i++){
		anio += AAAAMMDD[i];
	}

	for(i = 4; i < 6; i++){
		mes+=AAAAMMDD[i];
	}
	for(i = 6; i < 8; i++){
		dia+=AAAAMMDD[i];
	}

	resultado = dia + "/" + mes + "/" + anio;

	return resultado;
}

string FechaYHora::getHoraHH_MM(string HHMM){

	int i;
	string hora = "";
	string min = "";
	string resultado = "";

	for(i = 0; i < 2; i++){
		hora+= HHMM[i];
	}
	for(i = 2; i < 4; i++){
		min += HHMM[i];
	}

	resultado = hora + ":" + min;

	return resultado;
}
