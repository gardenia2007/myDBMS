/*
 * DBMS.h
 *
 *  Created on: 2012-10-12
 *      Author: y
 */


#ifndef DBMS_H_
#define DBMS_H_

#ifndef CONST_H_
#include "const.h"
#endif

#include <iostream>
#include <string>
#include <stdlib.h>
#include "DB.h"

using namespace std;


class DBMS {
public:
	DBMS();
	virtual ~DBMS();
	void run();

private:
	string sql, result;

	DB *db;
	unsigned int pos; //当前解析位置
	char dbName[MAX_DBNAME_LEGTH];
	char tableName[MAX_DBNAME_LEGTH];

	Data *data; // 这是一个链表
	int operate;


	void parseOpreate();

	void dispatchSql();

	bool useDB();
	bool createDB();
	bool deleteDB();

	//basic table opreation
	bool createTable();
	bool insertTable();
	bool updateTable();
	bool deleteTable();

	//parse opreate sql
	bool parseCreateTable();
	bool parseDBName();
	bool parseInsert();

	//bool createDatabase();
	//bool deleteDatabase();

	void select();


	//char * getFirstWord();
	int getNextWord(char* ,int);
	bool endOfSql();


};

#endif /* DBMS_H_ */
