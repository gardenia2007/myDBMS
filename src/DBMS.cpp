/*
 * DBMS.cpp
 *
 *  Created on: 2012-10-12
 *      Author: y
 */

#include "DBMS.h"


DBMS::DBMS() {
	// TODO Auto-generated constructor stub

}

void DBMS::run(){
	cout << "I am running!" << endl;
	while(1){
		cin >> this->sql;
		this->parseSql();
		this->dispatchSql();
	}
}

DBMS::~DBMS() {
	// TODO Auto-generated destructor stub
}

