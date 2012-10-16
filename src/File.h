/*
 * File.h
 *	文件操作类,维护DBMS的数据文件
 *	堆文件 + B+树索引
 *  Created on: 2012-10-14
 *      Author: y
 */

#ifndef FILE_H_
#define FILE_H_

#ifndef CONST_H_
#include "const.h"
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>

using namespace std;

class File {
public:
	File();
	virtual ~File();

	// 外部接口，供DBMS调用
	bool useDB(const char *);
	bool createDB(const char *);
	bool deleteDB();

	//basic table opreation
	bool createTable(const char *, Data *);
	bool insertTable();
	bool updateTable();
	bool deleteTable();


	bool read(char*);
	bool write();


private:
	char databaseName[MAX_DATABASENAME_SIZE];
	string dbName;
	fstream file;
	string dbPath, tablePath, dataPath, modalPath, tempPath;
	char data[1024];


	// 按块读写
	void readBlock(int block);
	void writeBlock(int block);

};

#endif /* FILE_H_ */
