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
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <cctype>
#include <dirent.h>
#include <cstdio>
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
	char tableName[MAX_TABLE_NAME_SIZE];

	timeval *begTime, *endTime;
	char * runTime;

	Data *data; // 这是一个链表
	Data *attribute;
	Data *tables;
	Data *qualification;
	int operate;

	void parseOpreate();

	void dispatchSql();

	bool useDB();
	bool createDB();
	bool deleteDB();

	//basic table opreation
	void createTable();
	void insertTable();
	void updateTable();
	void deleteTable();
	void displayTable();
	void deleteTuple();

	//parse opreate sql
	bool parseCreateTable();
	bool parseInsertTable();
	bool parseUpdateTable();
	bool parseUpdateChange(Data *&, string);
	bool parseDeleteTable();
	bool parseDBName();
	bool parseInsert();
	bool praseSelect(Data *&, string);
	bool praseConnditon(Data *&);
	bool parseUpdate(Data *&, string);
	bool parseDisplay();

	bool showDetails(); //show
	bool showDir(char * word);

	bool describe();   //describe table;


	//bool createDatabase();
	//bool deleteDatabase();

	void select();

	//char * getFirstWord();
	int getNextWord(char*, int);
	bool endOfSql();

	void begin();
	void end();

};

#endif /* DBMS_H_ */
