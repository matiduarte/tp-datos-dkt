/*
 * FechaYHora.h
 *
 *  Created on: 04/11/2013
 *      Author: marian
 */

#ifndef FECHAYHORA_H_
#define FECHAYHORA_H_

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <time.h>

using namespace std;

class FechaYHora {
public:
	FechaYHora();
	virtual ~FechaYHora();

	//setea fecha actual en formato AAAAMMDD
	static string setFechaAAAAMMDD();

	//setea hora actual en formato HHMM
	static string setHoraHHMM();

	//devuelve fecha guardada en formato DD/MM/AAAA
	static string getFecha_DD_MM_AAAA(string AAAAMMDD);

	//devuelve hora guardada en formato HH:MM
	static string getHoraHH_MM(string HHMM);

};

#endif /* FECHAYHORA_H_ */
