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
	index = Index();
}

DB::~DB() {
	//	file.close();
}

bool DB::deleteTuple(Data *property, Data *tables, Data *qulification) {

	const char * tableName = tables->name;

	preparePathModelAddr(tableName, qulification);
	if (!f.prepareFetchTuple()) // data文件为空
		return 0;

	int numOfAttribute = f.getAttributeNumFromModel(model);
	tuple *p = new tuple[numOfAttribute];
	bool result;

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
			deleteNewAttribute(p, numOfAttribute);
			return f.deleteTuple();
		}
		deleteNewAttribute(p, numOfAttribute);
	}
	return result;
}

bool DB::getPKValue(Data * q, int *pk) {
//	int numfAttribute = f.getAttributeNumFromModel(model);
	const char * equal = "=";
	while (q != NULL) {
		//
		if (strcmp(q->name, "true"))
			return false;

		// 名字匹配，且操作是等于时，
		if (strcmp(q->name, model[0].name) == 0
				&& strcmp(q->value, equal) == 0) {
			*pk = atoi(q->value2);
			return true;
		}
		q = q->next;
	}
	return false;
}

// 单个表的选择
bool DB::select(Data *property, Data *tables, Data *qulification) {

	bool result;
	if (tables->next != NULL) {
		this->link(property, tables, qulification);
	} else {
		const char * tableName = tables->name;
		this->selecttmp = false;

		preparePathModelAddr(tableName, qulification);

		if (!f.prepareFetchTuple()) // data文件为空
			return 0;

		int numOfAttribute = f.getAttributeNumFromModel(model);
		tuple *p = new tuple[numOfAttribute];
		int numOfResult = 0;

		if (strcmp(qulification->name, "true") == 0) {
			numOfResult = this->allToTmp(numOfAttribute, p, tableName);
		} else {
			numOfResult = this->partToTmp(numOfAttribute, p, qulification,
					tableName);
		}

		if (numOfResult > 0) {
			this->selecttmp = true;
			result = this->showSelect(tableName, numOfAttribute, property);
		}
		this->selecttmp = false;
	}
	this->selecttmp = false;
	return result;
}

int DB::allToTmp(int numOfAttribute, tuple *p, const char * tableName) {
	int numOfResult = 0;
	while (f.fetchTuple(p)) {
		numOfResult++;
		this->transform(numOfAttribute, p);
		this->selecttmp = true;
		this->insertTmp(tableName, this->data);
		deleteNewAttribute(p, numOfAttribute);
	}
	return numOfResult;
}

int DB::partToTmp(int numOfAttribute, tuple *p, Data *qulification,
		const char * tableName) {
	int numOfResult = 0;
	bool result = true;
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
	return numOfResult;
}

void DB::transform(int numOfAttribute, tuple *p) {
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

bool DB::tupleJudge(Data *q, tuple x, tuple *p, int numOfAttribute) {
	for (int i = 0; i < numOfAttribute; i++) {
		if (strcmp(q->name, model[i].name) == 0) {
			x = p[i];
			switch (model[i].type) {
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

bool DB::compareInt(Data *q, tuple x) {
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
	case '<':
		if (this->ChartoInt(x) < atoi(q->value2))
			return true;
		else
			return false;
		break;
	default:
		return false;
	}
}

bool DB::insertTmp(const char *tableName, Data *data) {

	preparePathModelAddr(tableName, NULL);

	if (tmpf.writeTuple(data))
		return true;
	else
		return false;
}

bool DB::showSelect(const char * tableName, int numOfAttribute,
		Data *property) {
	int numOfResult = 0;
	showTableHead(numOfAttribute, property, true);
	cout << "------------------------------" << endl;
	preparePathModelAddr(tableName, NULL);
	if (!tmpf.prepareFetchTuple())
		return false;
	else {
		tuple *p = new tuple[numOfAttribute];
		while (tmpf.fetchTuple(p)) {
			numOfResult++;
			cout << numOfResult << ":\t";
			if (strcmp(property->name, "*") == 0) {
				this->showAll(numOfAttribute, p);
			} else {
				this->showPart(numOfAttribute, p, property);
			}
			cout << endl;
			deleteNewAttribute(p, numOfAttribute);
		}
	}
	cout << "------------------------------" << endl << "find " << numOfResult
			<< " results." << endl;
	deletePath(tmpPath);
	return true;
}

void DB::showTableHead(int num, Data * property, bool br = true) {
	if (br)
		cout << "no:\t";
	if (strcmp(property->name, "*") == 0) {
		for (int i = 0; i < num; i++) {
			cout << model[i].name << "\t";
		}
	} else {
		Data *q = property;
		while (q != NULL) {
			for (int i = 0; i < num; i++)
				if (strcmp(q->name, model[i].name) == 0)
					cout << model[i].name << "\t";
			q = q->next;
		}
	}
	if (br)
		cout << endl;
}

void DB::showPart(int numOfAttribute, tuple *p, Data *property) {
	Data *q = property;
	while (q != NULL) {
		for (int i = 0; i < numOfAttribute; i++) {
			if (strcmp(q->name, model[i].name) == 0) {
				switch (model[i].type) {
				case TYPE_INT:
					cout << this->ChartoInt(p[i]) << "\t";
					break;
				case TYPE_CHAR:
					cout << p[i] << "\t";
					break;
				default:
					break;
				}
				break;
			}
		}
		q = q->next;
	}
}

void DB::showAll(int numOfAttribute, tuple *p) {
	for (int i = 0; i < numOfAttribute; i++) {
		switch (model[i].type) {
		case TYPE_INT:
			cout << this->ChartoInt(p[i]) << "\t";
			break;
		case TYPE_CHAR:
			cout << p[i] << "\t";
			break;
		default:
			break;
		}
	}
}

void DB::deleteNewAttribute(tuple *t, int num) {
	for (int i = 0; i < num; i++) {
		delete[] t[i];
	}
}

bool DB::insertTable(const char *tableName, Data * data) {
	preparePathModelAddr(tableName, NULL);

	// insert 需要特殊处理，因为数据格式不是qulification那种---->设计原因。。
	// 有时间再改吧
	int pk = atoi(data->name);
	block_addr addr = index.getBlockAddr(pk);
	f.setBlockAddr(addr);

	if (f.writeTuple(data))
		return true;
	else
		return false;
}

bool DB::createTable(const char* tableName, Data * data) {
	this->selecttmp = false;
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

		praseModel();
		index.initIndex(&tablePath, model);

		return true;
	} else { // 表已经存在
		return false;
	}
	return true;
}

bool DB::deleteDB(const char * databaseName) {
	this->dbName = string(databaseName);
	dbPath = DATA_PATH + dbName + PATH_SPARATOR;
	this->deletePath(this->dbPath);
	return true;
}

bool DB::deleteTable(const char * tableName) {
	setTablePath(tableName);
	this->deletePath(this->tablePath);
	return true;
}

bool DB::updateTable(Data *property, Data *tables, Data *qulification) {

	const char * tableName = tables->name;

	preparePathModelAddr(tableName, qulification);
	if (!f.prepareFetchTuple()) // data文件为空
		return 0;

	int numOfAttribute = f.getAttributeNumFromModel(model);
	tuple *p = new tuple[numOfAttribute];
	bool result;

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
			f.deleteTuple();
			result = this->makeNewTuple(tableName, p, numOfAttribute, property);
			deleteNewAttribute(p, numOfAttribute);
			break;
		}
		deleteNewAttribute(p, numOfAttribute);
	}
	return result;
}

bool DB::updateData(Data *property, int numOfAttribute, tuple *p) {
	Data *q = property;
	int tmp;
	while (q != NULL) {
		for (int i = 0; i < numOfAttribute; i++) {
			if (strcmp(q->name, model[i].name) == 0) {
				switch (model[i].type) {
				case TYPE_INT:
					tmp = atoi(q->value2);
					strcpy(p[i], reinterpret_cast<char *>(&tmp));
					break;
				case TYPE_CHAR:
					strcpy(p[i], q->value2);
					break;
				default:
					break;
				}
			}
		}
		q = q->next;
	}
	if (q == NULL)
		return true;
	else
		return false;
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
		filedSize += size;

		p = p->next;
	}
	filedSize += DELETE_FLAG_SIZE; // 删除标识位

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

void DB::preparePathModelAddr(const char *tableName, Data * q) {
	int pk;
	block_addr addr = -1;

	if (this->selecttmp == false) {
		this->setTablePath(tableName);
		f.setTablePath(tablePath);

		this->praseModel();
		f.model = this->model;

		if (getPKValue(q, &pk))
			addr = index.getBlockAddr(pk);

		f.setBlockAddr(addr);
	} else {
		this->setTablePath(tableName);
		tmpf.setTablePath(tmpPath);

		this->praseModel();
		tmpf.model = this->model;

		tmpf.setBlockAddr(0);
	}
}

bool DB::deletePath(string path) {
	system(("rm -f -r " + path).data());
	return true;
}

bool DB::makeNewTuple(const char * tableName, tuple *p, int numOfAttribute,
		Data *property) {
	bool result;
	result = this->updateData(property, numOfAttribute, p);
	if (result == true) {
		Data * data = new Data;
		Data * q = data;
		q->next = q;
		for (int i = 0; i < numOfAttribute; i++) {
			q = q->next;
			switch (model[i].type) {
			case TYPE_INT:
				sprintf(q->name, "%d", this->ChartoInt(p[i]));
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
		result = this->insertTable(tableName, data);
	}
	return result;
}

bool DB::link(Data *property, Data *tables, Data *qulification) {
	bool result;
	File file1 = File();
	File file2 = File();
	this->preWork(tables, &file1);
	this->preWork(tables->next, &file2);
	result = this->compareTuple(&file1, &file2, qulification);

	return result;
}

void DB::preWork(Data *tables, File *file) {
	this->setTablePath(tables->name);
	file->setTablePath(tablePath);

	file->setBlockAddr(-1);
	file->prepareFetchTuple();

	this->praseModel();
	file->model = this->model;
}

bool DB::compareTuple(File *file1, File *file2, Data *qulification) {
	char tmp1[MAX_NAME_SIZE];
	char tmp2[MAX_NAME_SIZE];
	this->cutString(qulification, tmp1, tmp2);
	int num1 = file1->getAttributeNumFromModel(file1->model);
	int num2 = file2->getAttributeNumFromModel(file2->model);
	int i, j, numOfResult = 0;
	tuple *p = new tuple[num1];
	tuple *q = new tuple[num2];
	while (file1->fetchTuple(p)) {
		for (i = 0; i < num1; i++) {
			if ((strcmp(tmp1, file1->model[i].name)) == 0)
				break;
		}
		file2->setBlockAddr(-1);
		file2->prepareFetchTuple();
		while (file2->fetchTuple(q)) {
			for (j = 0; j < num2; j++) {
				if ((strcmp(tmp2, file2->model[j].name)) == 0)
					break;
			}
			if ((strcmp(p[i], q[j])) == 0) {
				numOfResult++;
				this->showLink(file1, file2, p, q, j, numOfResult, num1, num2);
			}
		}
	}
	cout << "------------------------------" << endl << "find " << numOfResult
			<< " results." << endl;
	return true;
}

void DB::cutString(Data * qulification, char *tmp1, char *tmp2) {
	string tmp = "";
	tmp.assign(qulification->name);
	int i = tmp.find(".") + 1;
	int j = tmp.length();
	tmp.copy(tmp1, j - i, i);
	tmp1[j-i] = '\0';
	tmp = "";
	tmp.assign(qulification->value2);
	i = tmp.find(".") + 1;
	j = tmp.length();
	tmp.copy(tmp2, j - i, i);
	tmp2[j-i] = '\0';
}

void DB::showLink(File *file1, File *file2, tuple *p, tuple *q, int j,
		int numOfResult, int num1, int num2) {
	if (numOfResult == 1) {
		cout << "no:" << "\t";
		for (int i = 0; i < num1; i++) {
			cout << file1->model[i].name << "\t";
		}
		for (int k = 0; k < num2; k++) {
			if (k != j)
				cout << file2->model[k].name << "\t";
		}
		cout << endl;
		cout << "------------------------------" << endl;
	}
	cout << numOfResult << ":\t";
	for (int i = 0; i < num1; i++) {
		switch (file1->model[i].type) {
		case TYPE_INT:
			cout << this->ChartoInt(p[i]) << "\t";
			break;
		case TYPE_CHAR:
			cout << p[i] << "\t";
			break;
		default:
			break;
		}
	}
	for (int k = 0; k < num2; k++) {
		if (k != j) {
			switch (file2->model[k].type) {
			case TYPE_INT:
				cout << this->ChartoInt(q[k]) << "\t";
				break;
			case TYPE_CHAR:
				cout << q[k] << "\t";
				break;
			default:
				break;
			}
		}
	}
	cout << endl;
}

