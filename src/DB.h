/*
 * File.h
 *	文件操作类,维护DBMS的数据文件
 *	Hash索引
 *  Created on: 2012-10-14
 *      Author: y
 */

#ifndef FILE_H_
#define FILE_H_

#ifndef CONST_H_
#endif
#include "const.h"

#include "Index.h"
#include "File.h"

#include <iostream>
#include <fstream>
#include <string>
#ifdef linux
#include <sys/stat.h>
#elif WIN32
#include <direct.h>
#endif

using namespace std;

class DB {
public:
	DB();
	virtual ~DB();

	// 外部接口，供DBMS调用
	bool useDB(const char *);
	bool createDB(const char *);
	bool deleteDB();

	//basic table opreation
	bool createTable(const char *, Data *);
	bool insertTable(const char *, Data *);
	bool updateTable();
	bool deleteTable();

	bool select(Data *);

	bool read(char*);
	bool write();


private:
	char databaseName[MAX_DATABASE_NAME_SIZE];
	string dbName;
	fstream file;
	string dbPath, tablePath;
	Modal *property;

	int ChartoInt(char *);
        
	// 按块读写
	void readBlock(int);
	void writeBlock(int);

	//解析model
	void praseModel();
	//解析数据
	void praseData();

	bool initModal(string , Data *);
	int parseFiledType(char *);
	int getFiledSize(int, int);

};

#endif /* FILE_H_ */
