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

		initModal(table, data);

		return true;
	} else { // 表已经存在
		return false;
	}
	return true;
}

bool File::initModal(string tableName, Data * data) {
	file.write(tableName.data(), MAX_TABLE_NAME_SIZE); // table name
	int zero = 0;
	file.write(reinterpret_cast<char *>(&zero), 4); // 一个元组所占size(bit)
	file.write(reinterpret_cast<char *>(&zero), 4); // 表中所有属性的个数
	file.write(reinterpret_cast<char *>(&zero), 24); // 凑够 32 byte
	Data *p = data;
	int type, size, filedSize = 0, filedNum = 0;
	while (p != NULL) {
		file.write(p->name, MAX_FIELD_NAME_SIZE); // 32 bytes
		type = parseFiledType(p->value);
		size = getFiledSize(type, p->num);
		file.write(reinterpret_cast<char *>(&type), sizeof(int));// 4 bytes
		file.write(reinterpret_cast<char *>(&size), sizeof(int));// 4 bytes
		file.write(reinterpret_cast<char *>(&zero), 24);// 24 bytes

		filedNum++;
		filedSize++; // 删除标识位
		filedSize+=size;

		p = p->next;
	}

	file.seekp(MAX_TABLE_NAME_SIZE, ios::beg);
	file.write(reinterpret_cast<char *>(&filedSize), sizeof(int));// 一个元组的大小(bit)
	file.write(reinterpret_cast<char *>(&filedNum), sizeof(int));// 属性个数

	file.close();
	return true;
}

int File::parseFiledType(char *type) {
	string str(type);
	if (str == "int")
		return TYPE_INT;
	else if (str == "char")
		return TYPE_CHAR;
	else
		return TYPE_INVAILD;
}

int File::getFiledSize(int type, int num){
	switch(type){
	case TYPE_INT:
		return sizeof(int);
	case TYPE_CHAR:
		return num * sizeof(char);
	default:
		return 0;
	}
}

bool File::createDB(const char* databaseName) {
	this->dbName = string(databaseName);

	dbPath = DATA_PATH + dbName + PATH_SPARATOR;
//	cout << dbName << endl;
	file.open((dbPath + MODAL_FILE_NAME).data());
	if (file.fail()) { // 数据文件不存在，即数据库不存在
		mkdir((dbPath).data(), 0777);

		file.open((dbPath + MODAL_FILE_NAME).data(), ios::out);
		file.close();

		return true;
	} else {
		return false;
	}
}

bool File::useDB(const char* databaseName) {
	this->dbName.assign(databaseName);
//	cout << "file db name is" << this->dbName << endl;
	dbPath = (DATA_PATH + dbName + "/");
	file.open((dbPath + MODAL_FILE_NAME).data(),
			ios::in | ios::out | ios::binary);
	if (file.fail()) { // 数据文件不存在，即数据库不存在
		cout << "file db name is" << this->dbName << endl;
		return false;
	} else
		return true;
}
