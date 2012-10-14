/*
 * DBMS.h
 *
 *  Created on: 2012-10-12
 *      Author: y
 */


#ifndef DBMS_H_
#define DBMS_H_

#include <iostream>
#include <string>
#include "const.h"

using namespace std;

struct Data{
	char name[MAX_NAME_SIZE];
	char value[MAX_VALUE_SIZE];
};


class DBMS {
public:
	DBMS();
	virtual ~DBMS();
	void run();

private:
	string sql;
	Data data[MAX_CONDITION_SIZE];
	int operate;

	//解析SQL
	bool parseSql();
	int parseOpreate(string word);


	void dispatchSql();

	bool createDatabase();
	bool deleteDatabase();

	//basic table opreation
	bool createTable();
	bool insertTable();
	bool updateTable();
	bool deleteTable();

	bool createDatabase();
	bool deleteDatabase();

	void select();




};

#endif /* DBMS_H_ */
