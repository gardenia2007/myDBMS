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
	writeEmptyBlockFlag = false;
}

File::~File() {
	// TODO Auto-generated destructor stub
}

/********** PUBLIC FUNCTION *************/

// fetch元组前设置offset为第一条元组的位置
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

	if (block.eob()) {
		if (!readNextBlock()) // 如果没有下一块了,返回false
			return false;
		block.offset = BLOCK_HEAD_SIZE;
	}

	while (block.eot()) { // 如果读到当前块最后一个元组, 跳过没有内容的块
		// 如果是搜索全部的块，则继续顺序读下一块
		if (searchAll) {
			if (!readNextBlock()) // 如果没有下一块了,返回false
				return false;
			block.offset = BLOCK_HEAD_SIZE;
		} else {
			return false;
		}
	}

	if (isDeleted()) {
		return fetchTuple(t);
	}

	for (int i = 0; model[i].no != -1; i++) {
		tmp = new char[model[i].size];
		block.readChar(tmp, model[i].size);
		t[i] = tmp;
	}
	block.offset += DELETE_FLAG_SIZE;

	return true;
}

bool File::writeTuple(Data * d) {
	if (getBlockNum() == 0) { // 如果文件为空
		currentAddr = newBlock();
	} else {
		readBlock(&block, currentAddr);
	}

	while(block.getRemainSpace() <= 0) { // 如果当前块没有足够空间
		if (!readNextBlock()) // nextBlock读取失败，即没有下一个块
			currentAddr = newBlock();

	}

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
	char unDeleteF = NOT_DELETED_FLAG;
	block.writeChar(&unDeleteF, DELETE_FLAG_SIZE);

	updateRemainSpace(-1);
	if (writeBlock(&block, currentAddr))
		return true;
	else
		return false;
}

bool File::deleteTuple() {
	int tmp = block.offset;
	block.offset -= DELETE_FLAG_SIZE;
	char deleteF = DELETED_FLAG;
	block.writeChar(&deleteF, DELETE_FLAG_SIZE);
	block.offset = tmp;
	writeBlock(&block, currentAddr);
	return true;
}

bool File::isDeleted() {
	int tmp = block.offset;
	int a = block.getTupleSize();
	a += tmp;
	a -= DELETE_FLAG_SIZE;
	block.offset = a;
	char flag;

	block.readChar(&flag, DELETE_FLAG_SIZE);

	if (flag == DELETED_FLAG)
		return true;
	else {
		block.offset = tmp;
		return false;
	}

}

bool File::writeEmptyBlock(block_addr addr, int num) {
	writeEmptyBlockFlag = !writeEmptyBlockFlag; // true;

	for (int i = 0; i < num; i++) {
		currentAddr = newBlock();
		writeBlock(&block, currentAddr);
	}

	writeEmptyBlockFlag = !writeEmptyBlockFlag; // false;
	return true;
}

bool File::setTablePath(string path) {
	this->tablePath = path;
	return true;
}

bool File::setBlockAddr(block_addr addr) {
	if (addr == NOT_INDEX_FEILD) {
		// set the flag
		this->searchAll = true;
		this->currentAddr = 0;
	} else {
		this->searchAll = false;
		this->currentAddr = addr;
	}
	this->previousAddr = -1;
	return true;
}

/********** PRIVATE FUNCTION *************/

bool File::readNextBlock() {
	previousAddr = currentAddr;

	if (searchAll) // if not index field
		currentAddr++;
	else
		currentAddr = block.getNextBlockAddr();

	if (getBlockNum() > currentAddr && currentAddr != -1) {
		readBlock(&block, currentAddr);
		return true;
	} else {
		return false;
	}
}

int File::newBlock() {
	block_addr newAddr = getBlockNum();
	int tupleSize = getTupleSizeFromModel(model), noNextBlock = -1; //TODO 需要从model中获取tupleSize
	int numOfBlock = (BLOCK_SIZE - BLOCK_HEAD_SIZE) / tupleSize;
	block.offset = 0;
	block.writeInt(&numOfBlock);
	block.writeInt(&tupleSize);
	block.writeInt(&noNextBlock);
	// 更新上一个块的next_block
	updatePreviousBlock(newAddr);
	return newAddr;
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
	return m[i].size;
}

void File::updateRemainSpace(int spaceIncreasement) {
	int current = block.getRemainSpace();
	current += spaceIncreasement; // update
	block.offset = BLOCK_HEAD_REMAIN_OFFSET;
	block.writeInt(&current);
}

bool File::updatePreviousBlock(block_addr addr) {

	if (newFile || previousAddr == -1 || writeEmptyBlockFlag)
		return true;

	Block tmp = Block();
	readBlock(&tmp, previousAddr);

	tmp.offset = BLOCK_HEAD_NEXT_OFFSET;
	tmp.writeInt(&addr);

	writeBlock(&tmp, previousAddr);
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
	wrtable.open((tablePath + DATA_FILE_NAME).data(),
			ios::in | ios::out | ios::binary); //
	wrtable.seekp(BLOCK_SIZE * addr, ios::beg);
	wrtable.write(block->data, BLOCK_SIZE);
	wrtable.close();
	return true;
}

//  得到当前数据表的块总数
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
		return (f.tellg() / BLOCK_SIZE);
	}
}
