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
	cout << table << endl;
	file.open((DATA_PATH + dbName + "/" + table + DATA_FILE_NAME).data());
	if (file.fail()) { // 数据文件不存在，即table不存在
		mkdir((DATA_PATH + dbName + "/" + table).data(), 0777);

		file.open((DATA_PATH + dbName + "/" + table + DATA_FILE_NAME).data(), ios::out);
		file.close();
		file.open((DATA_PATH + dbName + "/" + table + MODEL_FILE_NAME).data(), ios::out);

		return true;
	} else { // 表已经存在
		return false;
	}
	return true;
}

bool File::createDB(const char* databaseName) {
	this->dbName = string(databaseName);

	cout << dbName << endl;
	file.open((DATA_PATH + dbName + DB_BASIC_FILE_NAME).data());
	if (file.fail()) { // 数据文件不存在，即数据库不存在
		mkdir((DATA_PATH + dbName).data(), 0777);

		file.open((DATA_PATH + dbName + DB_BASIC_FILE_NAME).data(), ios::out);
		file.close();

		return true;
	} else {
		return false;
	}
}

bool File::useDB(const char* databaseName) {
	this->dbName.assign(databaseName);
//	cout << "file db name is" << this->dbName << endl;
	file.open((DATA_PATH + dbName + DATA_FILE_NAME).data(),
			ios::in | ios::out | ios::binary);
	if (file.fail()){ // 数据文件不存在，即数据库不存在
		cout << "file db name is" << this->dbName << endl;
		return false;
	} else
		return true;
}
