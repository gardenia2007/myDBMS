/*
 * DBMS.cpp
 *
 *  Created on: 2012-10-12
 *      Author: y
 */

#include "DBMS.h"

DBMS::DBMS() {
	this->operate = SELECT_TABLE;
	this->db = new DB();
	this->pos = 0;
	data = NULL;
	attribute = NULL;
	tables = NULL;
}

void DBMS::run() {
	cout << "Welcome to myDBMS!\nPress 'q' to exit" << endl;
	while (1) {
		result = "";
		cout << ">";
		getline(cin, this->sql);
		if (sql == "q") {
			cout << "bye" << endl;
			return;
		}
		this->parseOpreate();
		this->dispatchSql();
		cout << this->result << endl;
	}
}

bool DBMS::praseSelect(Data *&q, string seg) {
	Data *p = new Data;
	q = p;
	p->next = p;
	char tmp[MAX_NAME_SIZE];
	string str = "";
	int judge;
	while (!endOfSql()) {
		judge = this->getNextWord(tmp, DEFAULT_SIZE);
		if (judge == -1) {
			return false;
		} else {
			if (str.append(tmp) == seg) {
				delete p->next;
				p->next = NULL;
				return true;
			} else {
				p = p->next;
				str = "";
				strcpy(p->name, tmp);
				p->next = new Data;
			}
		}
	}
	return true;
}

bool DBMS::praseConnditon(Data *&q) {
	Data *p = new Data;
	q = p;
	p->next = p;
	int judge;
	while (!endOfSql()) {
		p = p->next;
		judge = this->getNextWord(p->name, DEFAULT_SIZE);
		if (judge != -1)
			judge = this->getNextWord(p->value, DEFAULT_SIZE);
		if (judge != -1)
			judge = this->getNextWord(p->value2, DEFAULT_SIZE);
		if (judge == -1)
			return false;
		else {
			p->next = new Data;
		}
	}
	delete p->next;
	p->next = NULL;
	return true;
}

void DBMS::select() {
	if (praseSelect(attribute, "from") && praseSelect(tables, "where")
			&& praseConnditon(qualification)) {
		db->select(attribute, tables, qualification);
	} else {
		result = "inquire failed!";
	}
}

bool DBMS::parseInsert() {
	int i = 0;
	Data *p = new Data;

	if (getNextWord(tableName, DEFAULT_SIZE) == -1)
		return false;
	pos++; // skip '('

	data = p; // head
	p->next = p;
	//TODO 没有检查返回值是-1的情况
	while (!endOfSql()) {
		p = p->next;
		getNextWord(p->name, DEFAULT_SIZE);
		p->num = i++; // 这里的num表示字段的序号
		p->next = new Data;
	}
	delete p->next;
	p->next = NULL;

	return true;
}

void DBMS::insertTable() {
	if (!parseInsert() || !db->insertTable(tableName, data)) {
		result = "insert table <" + string(this->tableName) + "> fail!";
		//        return false;
	} else {
		result = "insert table <" + string(this->tableName) + "> success!";
		//        return true;
	}
}

bool DBMS::parseCreateTable() {
	if (getNextWord(tableName, 32) == -1)
		return false;
	this->pos++; // skip '('

	// 生成链表
	Data *p = new Data;
	data = p; // head
	p->next = p;
	//TODO 没有检查返回值是-1的情况
	while (!endOfSql()) {
		p = p->next;
		getNextWord(p->name, 32);
		p->num = getNextWord(p->value, 32); // 如果是char，返回值是char的长度，否则为0
		p->next = new Data;
	}
	delete p->next;
	p->next = NULL;

	return true;
}

bool DBMS::parseDeleteTable() {
	if (this->getNextWord(this->tableName, DEFAULT_SIZE) == -1)
		return false;
	else
		return true;
}

void DBMS::deleteTable() {
	if (!this->parseDeleteTable() || !db->deleteTable()) {
		result = "delete table <" + string(this->tableName) + "> fail!";
		//        return false;
	} else {
		result = "delete table <" + string(this->tableName) + "> success!";
		//        return true;
	}
}

bool DBMS::parseUpdateTable() {
	bool judge = true;
	judge = this->praseSelect(this->tables, "set");
	if (judge == true) {
		strcpy(this->tableName, this->tables->name);
		judge = this->parseUpdateChange(this->data, "where");
	}
	if (judge == true)
		judge = this->praseConnditon(this->qualification);
	return judge;
}

bool DBMS::parseUpdateChange(Data *&q, string seg) {
	Data *p = new Data;
	q = p;
	p->next = p;
	char tmp[MAX_NAME_SIZE];
	string str = "";
	int judge;
	while (!endOfSql()) {
		judge = this->getNextWord(tmp, DEFAULT_SIZE);
		if (judge == -1) {
			return false;
		} else {
			if (str.append(tmp) == seg) {
				delete p->next;
				p->next = NULL;
				return true;
			} else {
				p = p->next;
				str = "";
				strcpy(p->name, tmp);
				this->getNextWord(p->value, DEFAULT_SIZE);
				this->getNextWord(p->value2, DEFAULT_SIZE);
				p->next = new Data;
			}
		}
	}
	return true;
}

void DBMS::updateTable() {
	if (!this->parseUpdateTable() || !db->updateTable()) {
		result = "update table <" + string(this->tableName) + "> fail!";
		//        return false;
	} else {
		result = "update table <" + string(this->tableName) + "> success!";
		//        return true;
	}
}

void DBMS::createTable() {
	if (!parseCreateTable() || !db->createTable(tableName, data)) {
		result = "create table <" + string(this->tableName) + "> fail!";
		//        return false;
	} else {
		result = "create table <" + string(this->tableName) + "> success!";
		//        return true;
	}
}

bool DBMS::useDB() {
	if (!this->parseDBName() || !this->db->useDB(this->dbName)) {
		result = "database <" + string(dbName) + "> not found!";
		return false;
	} else {
		result = "use database <" + string(dbName) + "> success!";
		return true;
	}
}

bool DBMS::createDB() {
	if (!this->parseDBName() || !this->db->createDB(this->dbName)) {
		result = "create database <" + string(dbName) + "> fail!";
		return false;
	} else {
		result = "create database <" + string(dbName) + "> success!";
		return true;
	}
}

bool DBMS::deleteDB() {
	//TODO 没写删除数据库
	if (!this->parseDBName()) {
		result = "delete database <" + string(dbName) + "> fail!";
		return false;
	} else {
		result = "delete database <" + string(dbName) + "> success!";
		return false;
	}
}

bool DBMS::parseDBName() {
	if (this->getNextWord(this->dbName, MAX_DBNAME_LEGTH) != -1)
		return true;
	else
		return false;
}

bool DBMS::endOfSql() {
	if (sql.length() == pos || (sql.length() == pos + 1 && sql[pos] == ')')) // sql结束
		return true;
	else
		return false;
}

int DBMS::getNextWord(char* word, int size = 32) {
	int i = 0, numOfChar, j;
	bool readSim = false;
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
			if (readSim) { // 如果在insert语句的字符串中出现，则不能忽略了
				word[i] = sql[pos];
				pos++;
				i++;
				break;
			} else {
				word[i] = '\0';
				pos++; // skip this null character
				return 0;
			}
		case ';':
		case ':':
		case ')':
		case ',':
			word[i] = '\0';
			pos++; // skip this null character
			return 0;
			break;

		case '"': // 如果读到"，那么忽略，并设置标志(对insert语句来说)
			pos++;
			readSim = !readSim; // 初始为false
			break;

		case '(': // 只有create table语句的char的后面会有'('
			pos++;
			j = 0;
			while (sql[pos] != ')') { // 读取括号中得数字
				tmpNumOfChar[j] = sql[pos];
				pos++;
				j++;
			}
			word[i] = '\0';
			pos++; // skip ')'
			pos++; // if have ',' skip it
			numOfChar = atoi(tmpNumOfChar);
			return numOfChar;
			break;

		case '=':
		case '>':
		case '<':
		case '1':
			word[i] = sql[pos];
			word[i + 1] = '\0';
			pos += 2;
			return 0;
			break;

		case '*':
			word[i] = sql[pos];
			word[i + 1] = '\0';
			pos += 2;
			return 0;
			break;

		default:
			word[i] = sql[pos];
			pos++;
			i++;
			break;
		}
	}
}

void DBMS::dispatchSql() {
	//	cout << this->operate << endl;
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
	case USE_DATABASE:
		this->useDB();
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
	else if (word == "use")
		this->operate = USE_DATABASE;
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

