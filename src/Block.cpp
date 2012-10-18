/*
 * Block.cpp
 *
 *  Created on: 2012-10-17
 *      Author: y
 */

#include "Block.h"

Block::Block() {
	offset = 0;
}

Block::~Block() {
	// TODO Auto-generated destructor stub
}

void Block::autoOffset() {
	int sizeOfTuple = getTupleSize();
	int remain = getRemainSpace();
	int totalNumOfTuple = (BLOCK_SIZE - BLOCK_HEAD_SIZE) / sizeOfTuple;

	offset = (totalNumOfTuple - remain) * sizeOfTuple + BLOCK_HEAD_SIZE;
}

void Block::writeInt(int * num) {
	writeChar(reinterpret_cast<char *>(num), sizeof(int));
}

void Block::writeChar(const char * d, int size) {
	for (int i = 0; i < size; i++) {
		data[offset++] = d[i];
	}
}

void Block::readChar(char * d, int size) {
	for (int i = 0; i < size; i++) {
		d[i] = data[offset++];
	}
}

int Block::readInt() {
	int *remain;
	remain = reinterpret_cast<int *>(data + offset);
	return *remain;
}

int Block::getRemainSpace() {
	int tmp = offset;
	offset = BLOCK_HEAD_REMAIN_OFFSET;
	int r = readInt();
	offset = tmp;
	return r;
}

int Block::getTupleSize() {
	int tmp = offset;
	offset = BLOCK_HEAD_LENGTH_OFFSET;
	int r = readInt();
	offset = tmp;
	return r;
}

block_addr Block::getNextBlockAddr() {
	int tmp = offset;
	offset = BLOCK_HEAD_NEXT_OFFSET;
	int r = readInt();
	offset = tmp;
	return r;
}

bool Block::eob() {
	if (BLOCK_SIZE - offset < getTupleSize())
		return true;
	else
		return false;
}

bool Block::eot() {
	if ((BLOCK_SIZE - offset) / getTupleSize() <= getRemainSpace())
		return true;
	else
		return false;
}

