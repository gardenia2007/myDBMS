/*
 * File.cpp
 *
 *  Created on: 2012-10-14
 *      Author: y
 */

#include "DB.h"

DB::DB() {
	f = File();
	model = NULL;
}

DB::~DB() {
//	file.close();
}

void DB::setTablePath(const char *tableName){
	string table(tableName);
	tablePath = dbPath + table + PATH_SPARATOR;
}

bool DB::insertTable(const char *tableName, Data *data){
	this->setTablePath(tableName);
	f.setTablePath(tablePath);
	f.model = this->model;

	if(f.writeTuple(data))
		return true;
	else
		return false;
}

bool DB::select(Data *property, Data *tables, Data *qulification){
	Index i = Index();
	block_addr addr = i.getBlock(tables);

	File b = File();
	b.setTablePath(tables->name);
	b.setBlockAddr(addr);

	tuple *p;
	do{
            

	}while(p != NULL);

	return true;
}

bool DB::createTable(const char* tableName, Data * data) {
	setTablePath(tableName);
	file.open((tablePath + DATA_FILE_NAME).data());
	if (file.fail()) { // 数据文件不存在，即table不存在
#ifdef linux
		mkdir((tablePath).data(), 0777);
#elif WIN32
                mkdir((tablePath).data());
#endif

		file.open((tablePath + DATA_FILE_NAME).data(), ios::out);
		file.close();

		file.open((tablePath + MODEL_FILE_NAME).data(), ios::out);

		initModal(tableName, data);

		return true;
	} else { // 表已经存在
		return false;
	}
	return true;
}

bool DB::deleteTable(){
    
}

bool DB::updateTable(){
    
}

bool DB::initModal(const char *tableName, Data * data) {
	file.write(tableName, MAX_TABLE_NAME_SIZE); // table name
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

int DB::parseFiledType(char *type) {
	string str(type);
	if (str == "int")
		return TYPE_INT;
	else if (str == "char")
		return TYPE_CHAR;
	else
		return TYPE_INVAILD;
}

int DB::getFiledSize(int type, int num){
	switch(type){
	case TYPE_INT:
		return sizeof(int);
	case TYPE_CHAR:
		return num * sizeof(char);
	default:
		return 0;
	}
}

bool DB::createDB(const char* databaseName) {
	this->dbName = string(databaseName);

	dbPath = DATA_PATH + dbName + PATH_SPARATOR;
//	cout << dbName << endl;
	file.open((dbPath + MODEL_FILE_NAME).data());
	if (file.fail()) { // 数据文件不存在，即数据库不存在
#ifdef linux
		mkdir((dbPath).data(), 0777);
#elif WIN32
                mkdir((dbPath).data());
#endif

		file.open((dbPath + MODEL_FILE_NAME).data(), ios::out);
		file.close();

		return true;
	} else {
		return false;
	}
}

bool DB::useDB(const char* databaseName) {
	this->dbName.assign(databaseName);
//	cout << "file db name is" << this->dbName << endl;
	dbPath = (DATA_PATH + dbName + "/");
	file.open((dbPath + MODEL_FILE_NAME).data(),
			ios::in | ios::out | ios::binary);
	if (file.fail()) { // 数据文件不存在，即数据库不存在
		cout << "file db name is" << this->dbName << endl;
		return false;
	} else
		return true;
}

int DB::ChartoInt(char *temp) {
    int *result;
    result = reinterpret_cast<int *> (temp);
    return *result;
}


void DB::praseModel() {
    fstream rdmodel;
    rdmodel.open((this->tablePath + MODEL_FILE_NAME).data(), ios::in | ios::binary);
    rdmodel.seekg(32);
    char temp[4];
    //读属性总数
    rdmodel.read(temp, 4);
    int model_num = ChartoInt(temp);

    Model *p = new Model[model_num];

    this->model = p;

    for (int i = 0; i < model_num; i++) {
        //读属性名
        rdmodel.seekg(64 + 32 * i);
        rdmodel.read(p->name, 32);
        rdmodel.read(temp, 4);
        p->type =  ChartoInt(temp);
		rdmodel.read(temp, 4);
        p->size =  ChartoInt(temp);
        p->no = i;
    }

    rdmodel.close();
}

void DB::praseData() {

}

