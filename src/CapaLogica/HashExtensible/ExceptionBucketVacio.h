/*
 * ExceptionBucketVacio.h
 *
 *  Created on: 30/04/2010
 *      Author: hmike
 */

#ifndef EXCEPTIONBUCKETVACIO_H_
#define EXCEPTIONBUCKETVACIO_H_

#include <exception>

using namespace std;

class ExceptionBucketVacio : public exception {

public:
	virtual const char* what() const throw(){
		return "Bucket vacio.";
	}
};


#endif /* EXCEPTIONBUCKETVACIO_H_ */
