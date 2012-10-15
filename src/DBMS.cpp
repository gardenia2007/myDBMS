/*
 * DBMS.cpp
 *
 *  Created on: 2012-10-12
 *      Author: y
 */

#include "DBMS.h"

DBMS::DBMS() {
	this->operate = SELECT_TABLE;
	this->file = new File();
	this->pos = 0;
	data = NULL;
}

void DBMS::run() {
	cout << "I am running!" << endl;
	while (1) {
		getline(cin, this->sql);
		this->parseOpreate();
		this->dispatchSql();
		cout << this->result << endl;
	}
}

void DBMS::select() {
	cout << "i am select" << endl;
	char str[12] = "reading";
	this->file->read(str);
}

bool DBMS::insertTable() {
//	if(!this->parseInsert())
	return false;
//	this->file->insertTable();
	return true;
}

bool DBMS::parseCreateTable() {
	if (getNextWord(tableName, 32) == -1)
		return false;
	this->pos++; // skip '('

	//
	Data *p = new Data;
	data = p; // head
	p->next = p;
	while (!endOfSql()) {
		p = p->next;
		getNextWord(p->name, 32);
		p->length = getNextWord(p->value, 32);
		p->next = new Data;
	}
	delete p->next;
	p->next = NULL;

	return true;
}
bool DBMS::createTable() {
	cout << "create table" << endl;
	if (!parseCreateTable())
		return false;
	if (!file->createTable(tableName, data))
		return false;
	return true;
}

bool DBMS::deleteTable() {

	return true;

}

bool DBMS::updateTable() {
	return true;
}

bool DBMS::parseCreateDB() {
	if (this->getNextWord(this->dbName, MAX_DBNAME_LEGTH) != -1)
		return true;
	else
		return false;
}

bool DBMS::createDB() {
	if (!this->parseCreateDB())
		return false;
	if (!this->file->createDB(this->dbName))
		return false;
	return true;
}

bool DBMS::deleteDB() {

	return true;
}

bool DBMS::endOfSql() {
	if (sql.length() == pos || (sql.length() == pos + 1 && sql[pos] == ')')) // sql结束
		return true;
	else
		return false;
}

int DBMS::getNextWord(char* word, int size = 32) {
	int i = 0, numOfChar, j;
	char tmpNumOfChar[32];
	while (1) {
		if (endOfSql()) { // sql结束
			word[i] = '\0';
			return 0;
		} else if (i >= size) { // word容量不够
			return -1;
		}
		switch (sql[pos]) {
		case ' ': //first 执行此函数前要去掉空格
		case ';':
		case ':':
		case ')':
		case ',':
			word[i] = '\0';
			pos++; // skip this null character
			return 0;
			break;
		case '(': // 只有insert语句的char的后面会有'('
			pos++;
			j = 0;
			while (sql[pos] != ')') { // 读取括号中得数字
				tmpNumOfChar[j] = sql[pos];
				pos++;
				j++;
			}
			word[i] = '\0';
			pos++;
			numOfChar = atoi(tmpNumOfChar);
			return numOfChar;
			break;
		default:
			word[i] = sql[pos];
			break;
		}
		this->pos++;
		i++;
	}
	return 0;
}

void DBMS::dispatchSql() {
	cout << this->operate << endl;
	switch (this->operate) {
	case SELECT_TABLE:
		this->select();
		break;
	case INSERT_TABLE:
		this->insertTable();
		break;
	case UPDATE_TABLE:
		this->updateTable();
		break;
	case DELETE_TABLE:
		this->deleteTable();
		break;
	case CREATE_TABLE:
		this->createTable();
		break;
	case CREATE_DATABASE:
		this->createDB();
		break;
	case DELETE_DATABASE:
		this->deleteDB();
		break;
	default:
		return;
	}
}

void DBMS::parseOpreate() {
	this->pos = this->sql.find(' ');
	string word = this->sql.substr(0, this->pos);
	this->pos++; // skip white space
//	cout << word << endl;

	if (word == "select")
		this->operate = SELECT_TABLE;
	else if (word == "update")
		this->operate = UPDATE_TABLE;
	else if (word == "insert")
		this->operate = INSERT_TABLE;
	else if (word == "createtable")
		this->operate = CREATE_TABLE;
	else if (word == "deletetable")
		this->operate = DELETE_TABLE;
	else if (word == "createdb")
		this->operate = CREATE_DATABASE;
	else if (word == "deletedb")
		this->operate = DELETE_DATABASE;
	else
		this->operate = INVAILD_OPREATE;
}

DBMS::~DBMS() {
	// TODO Auto-generated destructor stub
}

