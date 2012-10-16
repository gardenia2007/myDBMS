/*
 * File.cpp
 *
 *  Created on: 2012-10-14
 *      Author: y
 */

#include "File.h"
using namespace std;

File::File() {

}

File::~File() {
//	file.close();
}

bool File::read(char* str) {
	cout << str << endl;
	return true;
}

bool File::createTable(const char* tableName, Data * data) {
	string table(tableName);
	tablePath = dbPath + table + PATH_SPARATOR;
	file.open((tablePath + DATA_FILE_NAME).data());
	if (file.fail()) { // 数据文件不存在，即table不存在
		mkdir((tablePath).data(), 0777);

		file.open((tablePath + DATA_FILE_NAME).data(), ios::out);
		file.close();
		file.open((tablePath + MODAL_FILE_NAME).data(), ios::out);

		return true;
	} else { // 表已经存在
		return false;
	}
	return true;
}

bool File::createDB(const char* databaseName) {
	this->dbName = string(databaseName);

	dbPath = DATA_PATH + dbName +  PATH_SPARATOR;
//	cout << dbName << endl;
	file.open((dbPath + DB_BASIC_FILE_NAME).data());
	if (file.fail()) { // 数据文件不存在，即数据库不存在
		mkdir((dbPath).data(), 0777);

		file.open((dbPath + DB_BASIC_FILE_NAME).data(), ios::out);
		file.close();

		return true;
	} else {
		return false;
	}
}

bool File::useDB(const char* databaseName) {
	this->dbName.assign(databaseName);
//	cout << "file db name is" << this->dbName << endl;
	dbPath =(DATA_PATH + dbName + "/");
	file.open((dbPath + MODAL_FILE_NAME).data(), ios::in | ios::out | ios::binary);
	if (file.fail()){ // 数据文件不存在，即数据库不存在
		cout << "file db name is" << this->dbName << endl;
		return false;
	} else
		return true;
}
