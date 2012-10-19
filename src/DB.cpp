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

bool DB::select(Data *property, Data *tables, Data *qulification) {

	Index i = Index();
	block_addr addr = i.getBlock(tables);

	const char * tableName = tables->name;

	preparePathModelAddr(tableName, addr);
	if (!f.prepareFetchTuple()) // data文件为空
		return 0;

	int numOfAttribute = f.getAttributeNumFromModel(model);
	tuple *p = new tuple[numOfAttribute];
	bool result;
	this->selecttmp = false;

	int numOfResult = 0;
	while (f.fetchTuple(p)) {
		tuple x;
		result = true;
		Data *q = qulification;
		while (q != NULL) {
			result = this->tupleJudge(q, x, p, numOfAttribute);
			if (result == true)
				q = q->next;
			else {
				break;
			}
		}
		if (result == true) {
			numOfResult++;
			this->transform(numOfAttribute, p);
			this->selecttmp = true;
			result = this->insertTmp(tableName, this->data);
		}
		deleteNewAttribute(p, numOfAttribute);
	}
	if (numOfResult > 0) {
		this->selecttmp = true;
		result = this->showSelect(tableName, numOfAttribute, property);
	}
	return result;
}

void DB::transform(int numOfAttribute, tuple *&p) {
	Data *q = new Data;
	this->data = q;
	q->next = q;
	for (int i = 0; i < numOfAttribute; i++) {
		q = q->next;
		switch (this->model[i].type) {
		case TYPE_INT:
			char tmp[DEFAULT_SIZE];
			sprintf(tmp, "%d", this->ChartoInt(p[i]));
			strcpy(q->name, tmp);
			break;
		case TYPE_CHAR:
			strcpy(q->name, p[i]);
			break;
		default:
			break;
		}
		q->next = new Data;
	}
	delete q->next;
	q->next = NULL;
}

bool DB::tupleJudge(Data*&q, tuple&x, tuple*&p, int numOfAttribute) {
	for (int i = 0; i < numOfAttribute; i++) {
		if (strcmp(q->name, model->name) == 0) {
			x = p[i];
			switch (model->type) {
			case TYPE_INT:
				return this->compareInt(q, x);
				break;
			case TYPE_CHAR:
				if (strcmp(q->value2, x) == 0)
					return true;
				else
					return false;
				break;
			default:
				return false;
				break;
			}
		}
	}
	return true;
}

bool DB::compareInt(Data *&q, tuple & x) {
	char tmp = q->value[0];
	switch (tmp) {
	case '=':
		if (this->ChartoInt(x) == atoi(q->value2))
			return true;
		else
			return false;
		break;
	case '>':
		if (this->ChartoInt(x) > atoi(q->value2))
			return true;
		else
			return false;
		break;
	default:
		if (this->ChartoInt(x) < atoi(q->value2))
			return false;
		else
			return 1;
		break;
	}
}

bool DB::insertTmp(const char *tableName, Data *data) {
	block_addr blockAddr = 0;
	preparePathModelAddr(tableName, blockAddr);

	if (tmpf.writeTuple(data))
		return true;
	else
		return false;
}

bool DB::showSelect(const char * tableName, int numOfAttribute,
		Data *&property) {
	this->preparePathModelAddr(tableName, 0);
	if (!tmpf.prepareFetchTuple())
		return false;
	else {
		tuple *p = new tuple[numOfAttribute];
		int numOfResult = 0;
		Data *q = property;
		while (tmpf.fetchTuple(p)) {
			numOfResult++;
			cout << numOfResult << ":";
			while (q != NULL) {
				for (int i = 0; i < numOfAttribute; i++) {
					if (strcmp(q->name, model[i].name) == 0) {
						switch (model[i].type) {
						case TYPE_INT:
							cout << this->ChartoInt(p[i]) << " ";
							break;
						case TYPE_CHAR:
							cout << p[i] << " ";
							break;
						default:
							break;
						}
						break;
					}
				}
				q = q->next;
			}
			cout << endl;
			deleteNewAttribute(p, numOfAttribute);
		}
	}
	deletePath(tmpPath);
	return true;
}

void DB::deleteNewAttribute(tuple *t, int num) {
	for (int i = 0; i < num; i++) {
		delete[] t[i];
	}
}

bool DB::insertTable(const char *tableName, Data * data) {
	// TODO blockAddr 根据 INDEX 确定
	block_addr blockAddr = 0;
	preparePathModelAddr(tableName, blockAddr);

	if (f.writeTuple(data))
		return true;
	else
		return false;
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

bool DB::deleteTable() {

}

bool DB::updateTable() {

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
	dbPath = (DATA_PATH + dbName + PATH_SPARATOR);
	file.open((dbPath + MODEL_FILE_NAME).data(),
			ios::in | ios::out | ios::binary);
	if (file.fail()) { // 数据文件不存在，即数据库不存在
		cout << "file db name is" << this->dbName << endl;
		return false;
	} else
		return true;

}

bool DB::initModal(const char *tableName, Data * data) {
	file.write(tableName, MAX_TABLE_NAME_SIZE); // table name
	int zero = 0;
	file.write(reinterpret_cast<char *>(&zero), 4); // 表中所有属性的个数
	file.write(reinterpret_cast<char *>(&zero), 4); // 一个元组所占size(bit)
	file.write(reinterpret_cast<char *>(&zero), 24); // 凑够 32 byte
	Data *p = data;
	int type, size, filedSize = 0, filedNum = 0;
	while (p != NULL) {
		file.write(p->name, MAX_FIELD_NAME_SIZE); // 32 bytes
		type = parseFiledType(p->value);
		size = getFiledSize(type, p->num);
		file.write(reinterpret_cast<char *>(&type), sizeof(int)); // 4 bytes
		file.write(reinterpret_cast<char *>(&size), sizeof(int)); // 4 bytes
		file.write(reinterpret_cast<char *>(&zero), 24); // 24 bytes

		filedNum++;
		//		filedSize++; // 删除标识位
		filedSize += size;

		p = p->next;
	}

	file.seekp(MAX_TABLE_NAME_SIZE, ios::beg);
	file.write(reinterpret_cast<char *>(&filedNum), sizeof(int)); // 属性个数
	file.write(reinterpret_cast<char *>(&filedSize), sizeof(int)); // 一个元组的大小(bit)

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

int DB::getFiledSize(int type, int num) {
	switch (type) {
	case TYPE_INT:
		return sizeof(int);
	case TYPE_CHAR:
		return num * sizeof(char);
	default:
		return 0;
	}
}

int DB::ChartoInt(char *temp) {
	int *result;
	result = reinterpret_cast<int *>(temp);
	return *result;
}

void DB::praseModel() {
	fstream rdmodel;
	rdmodel.open((this->tablePath + MODEL_FILE_NAME).data(),
			ios::in | ios::binary);
	rdmodel.seekg(32);
	char temp[4];
	//读属性总数
	rdmodel.read(temp, 4);
	int model_num = ChartoInt(temp);

	Model *p = new Model[model_num + 1];

	p[model_num].no = -1;
	// 读一个元组的大小
	rdmodel.read(temp, 4);
	p[model_num].size = ChartoInt(temp);

	this->model = p;

	for (int i = 0; i < model_num; i++) {
		//读属性名
		rdmodel.seekg(64 + 64 * i);
		rdmodel.read(p[i].name, 32);
		rdmodel.read(temp, 4);
		p[i].type = ChartoInt(temp);
		rdmodel.read(temp, 4);
		p[i].size = ChartoInt(temp);
		p[i].no = i;
	}

	rdmodel.close();
}

void DB::setTablePath(const char *tableName) {
	string table(tableName);
	if (this->selecttmp == false) {
		tablePath = dbPath + table + PATH_SPARATOR;
	} else {
		tmpPath =
				dbPath + table + PATH_SPARATOR + TMP_FILE_NAME + PATH_SPARATOR;
#ifdef linux
		mkdir((tmpPath).data(), 0777);
#elif WIN32
		mkdir((tmpPath).data());
#endif
	}
}

void DB::preparePathModelAddr(const char *tableName, block_addr addr) {
	if (this->selecttmp == false) {
		this->setTablePath(tableName);
		f.setTablePath(tablePath);

		this->praseModel();
		f.model = this->model;

		f.setBlockAddr(addr);
	} else {
		this->setTablePath(tableName);
		tmpf.setTablePath(tmpPath);

		this->praseModel();
		tmpf.model = this->model;

		tmpf.setBlockAddr(addr);
	}
}

bool DB::deletePath(string path){
	system(("rm -f -r " + path).data());
}
