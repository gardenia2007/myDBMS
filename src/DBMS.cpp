/*
 * DBMS.cpp
 *
 *  Created on: 2012-10-12
 *      Author: y
 */

#include "DBMS.h"

DBMS::DBMS() {
	// TODO Auto-generated constructor stub
	this->operate = SELECT_TABLE;
}

void DBMS::run() {
	cout << "I am running!" << endl;
	while (1) {
		cin >> this->sql;
		this->parseSql();
		//this->dispatchSql();
	}
}

bool DBMS::parseSql() {
	string firstWord = this->sql.substr(0, this->sql.find(' '));
	//cout << firstWord;
	this->operate = this->parseOpreate(firstWord);
	if (this->operate == INVAILD_OPREATE)
		return false;

	return true;
}

void DBMS::dispatchSql() {
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
		this->createDatabase();
		break;
	case DELETE_TABLE:
		this->deleteDatabase();
		break;
	default:
		return;
	}
}

int DBMS::parseOpreate(string word) {
	if (word == "select")
		return SELECT_TABLE;
	else if (word == "update")
		return UPDATE_TABLE;
	else if (word == "insert")
		return INSERT_TABLE;
	else if (word == "create")
		return CREATE_TABLE;
	else if (word == "delete")
		return DELETE_TABLE;
	else
		return INVAILD_OPREATE;
}

DBMS::~DBMS() {
	// TODO Auto-generated destructor stub
}

