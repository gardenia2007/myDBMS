/*
 * Block.cpp
 *
 *  Created on: 2012-10-17
 *      Author: y
 */

#include "File.h"

File::File() {
	block = Block();
	model = NULL;
	previousAddr = -1;
	currentAddr = 0;

	searchAll = false;
	newFile = false;
}

File::~File() {
	// TODO Auto-generated destructor stub
}

/********** PUBLIC FUNCTION *************/

bool File::readTuple(tuple *t) {
	int numOfAttr = getAttributeNumFromModel(model);
	if (currentAddr != -1) {
		if (block.eob()) {
			readNextBlock();
		}
		return true;
	} else {
		return false;
	}
}

bool File::prepareFetchTuple() {
	if (getBlockNum() == 0) {
		return false;
	} else {
		readBlock(&block, currentAddr);
		block.offset = BLOCK_HEAD_SIZE;
		return true;
	}
}

bool File::fetchTuple(tuple *t) {
	char *tmp;

	// TODO maybe have a bug
	if (block.eot()) { // 如果读到最后一个元组
		return false;
	} else if (block.eob()) {
		if (!readNextBlock()) // 如果没有下一块了,返回false
			return false;
		block.offset = BLOCK_HEAD_SIZE;
	} else {
		for (int i = 0; model[i].no != -1; i++) {
			tmp = new char[model[i].size];
			block.readChar(tmp, model[i].size);
			t[i] = tmp;
		}
	}
	return true;
}

bool File::setTablePath(string path) {
	this->tablePath = path;
	return true;
}

bool File::setBlockAddr(block_addr addr) {
	if (addr >= 0) {
		this->searchAll = false;
		this->currentAddr = addr;
	} else {
		// set the flag
		this->searchAll = true;
		this->currentAddr = 0;
	}
	this->previousAddr = -1;
	return true;
}

bool File::writeTuple(Data * d) {
	if (getBlockNum() == 0) {
		newBlock();
	} else {
		readBlock(&block, currentAddr);
	}

	if (block.getRemainSpace() <= 0) { // 如果当前块没有足够空间`
		if (!readNextBlock()) // nextBlock读取失败，即没有下一个块
			newBlock();
	} else {
		block.autoOffset();
		Data *p = d;
		int i = 0, num;
		while (p != NULL) {
			switch (model[i].type) {
			case TYPE_INT:
				num = atoi(p->name);
				block.writeInt(&num);
				break;
			case TYPE_CHAR:
				block.writeChar(p->name, model[i].size);
				break;
			default:
				return false;
			}
			i++;
			p = p->next;
		}
	}
	updateRemainSpace(-1);
	if (writeBlock(&block, currentAddr))
		return true;
	else
		return false;
}

/********** PRIVATE FUNCTION *************/

bool File::readNextBlock() {
	previousAddr = currentAddr;

	currentAddr = block.getNextBlockAddr();
	if (currentAddr != -1) {
		readBlock(&block, currentAddr);
		return true;
	} else {
		return false;
	}
}

bool File::newBlock() {
	currentAddr = getBlockNum();
	int tupleSize = getTupleSizeFromModel(model), noNextBlock = -1; //TODO 需要从model中获取tupleSize
	int numOfBlock = (BLOCK_SIZE - BLOCK_HEAD_SIZE) / tupleSize;
	block.offset = 0;
	block.writeInt(&numOfBlock);
	block.writeInt(&tupleSize);
	block.writeInt(&noNextBlock);
	// 更新上一个块的next_block
	updatePreviousBlock(currentAddr);
	return true;
}

int File::getAttributeNumFromModel(Model *m) {
	int i;
	for (i = 0; m[i].no != -1; i++)
		;
	return i;
}

int File::getTupleSizeFromModel(Model *m) {
	int i;
	for (i = 0; m[i].no != -1; i++)
		;
	return m[i - 1].size;
}

void File::updateRemainSpace(int space) {
	int current = block.getRemainSpace();
	current += space; // update
	block.offset = BLOCK_HEAD_REMAIN_OFFSET;
	block.writeInt(&current);
}

bool File::updatePreviousBlock(block_addr addr) {

	if (newFile || previousAddr == -1)
		return true;

	Block tmp = Block();
	readBlock(&tmp, previousAddr);

	tmp.offset = BLOCK_HEAD_NEXT_OFFSET;
	tmp.writeInt(&addr);

	writeBlock(&tmp, currentAddr);
	return true;
}

bool File::readBlock(Block *block, block_addr addr) {
	// if file is empty
	if (getBlockNum() == 0) {
		newFile = true;
		return false;
	}
	fstream rdtable;
	rdtable.open((tablePath + DATA_FILE_NAME).data(), ios::in | ios::binary);
	rdtable.seekg(addr * BLOCK_SIZE);
	rdtable.read(block->data, BLOCK_SIZE);
	rdtable.close();
	return true;
}

bool File::writeBlock(Block *block, block_addr addr) {
	fstream wrtable;
	wrtable.open((tablePath + DATA_FILE_NAME).data(), ios::out | ios::binary);
	wrtable.seekg(BLOCK_SIZE * addr);
	wrtable.write(block->data, BLOCK_SIZE);
	wrtable.close();
	return true;
}

int File::getBlockNum() {
	ifstream f;
	f.open((tablePath + DATA_FILE_NAME).data(), ios::binary);
	if (f.fail()) { // 如果打开失败，新建文件
		ofstream out;
		out.open((tablePath + DATA_FILE_NAME).data(), ios::binary);
		out.close();
		return 0;
	} else {
		f.seekg(0, ios::end);
		return f.tellg();
	}
}
