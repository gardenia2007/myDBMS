/*
 * Index.cpp
 *
 *  Created on: 2012-10-17
 *      Author: y
 */

#include "Index.h"

Index::Index() {
	hash_num = 10;
	f = File();
}

block_addr Index::getBlockAddr(int value) {
	return simpleHash(value); //
}

bool Index::initIndex(string *tablePath, Model *m) {
	f.setTablePath(*tablePath);
	f.model = m;
	return initSimpleHash();
}

bool Index::updateIndex() {
	return true;
}

/************** HASH ***************/

bool Index::initSimpleHash() {
	if (f.writeEmptyBlock(0, hash_num))
		return true;
	else
		return false;
}

block_addr Index::simpleHash(int v) {
	// 假设con为 con.name = con.value2 形式
//	int data = atoi(con->value2);
	return v % hash_num;
}

block_addr Index::hash(int data) {
	return data % hash_num;
}

Index::~Index() {
	// TODO Auto-generated destructor stub
}

