/*
 * File.h
 *	文件操作类,维护DBMS的数据文件
 *	Hash索引
 *  Created on: 2012-10-14
 *      Author: y
 */

#ifndef DB_H_
#define DB_H_

#include "const.h"

#include "Index.h"
#include "File.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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
	bool deleteDB(const char *);

	//basic table opreation
	bool createTable(const char *, Data *);
	bool insertTable(const char *, Data *);
	bool updateTable(Data *, Data *, Data *);
	bool deleteTuple(Data *, Data *, Data *);
	bool deleteTable(const char *);

	bool select(Data *, Data *, Data *);


private:
	char databaseName[MAX_DATABASE_NAME_SIZE];
	string dbName;
	fstream file;
	string dbPath, tablePath, tmpPath;
	Model *model;
	Data *data;
	bool selecttmp;

	File f, tmpf;

	void preparePathModelAddr(const char *, int); // 根据表名设置表文件路径，解析model，设置欲读块的起始地址

	void setTablePath(const char *);

	int ChartoInt(char *);

	void deleteNewAttribute(tuple *t, int);

	//解析model
	void praseModel();
	//解析数据
	void praseData();

	bool initModal(const char *, Data *);
	int parseFiledType(char *);
	int getFiledSize(int, int);

	bool deletePath(string path);

	bool compareInt(Data *&, tuple &);
	bool tupleJudge(Data *&, tuple &, tuple *&, int);
	void transform(int, tuple *&);
	bool insertTmp(const char *, Data *);
	bool showSelect(const char *, int, Data *&);
	void showAll(int, tuple *&);
	void showPart(int, tuple *&, Data *&);
	int allToTmp(int, tuple *&, const char *);
	int partToTmp(int, tuple *&, Data *&, const char *);

	bool updateData(Data *&, int, tuple *&);
	void updateInt(Data *&, tuple &);

};

#endif /* FILE_H_ */
