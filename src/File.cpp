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
	previous = -1;
	addr = 0;

	searchAll = false;
	newFile = false;
}

File::~File() {
	// TODO Auto-generated destructor stub
}

//tuple * File::getTuple() {
//
//}

bool File::writeTuple(Data * d) {
	if(getBlockNum() == 0){
		newBlock();
	}else{
		readBlock(&block);
	}

	if (block.getRemainSpace() <= 0) {
		readNextBlock();
	} else {
		block.autoOffset();
		Data *p = d;
		int i = 0, num;
		while(p != NULL){
			switch(model[i].type){
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
		}
	}
	return true;
}

void File::readNextBlock() {
	addr = block.getNextBlockAddr();
	if (addr == -1)
		newBlock();
	readBlock(&block);
}

bool File::newBlock(){
	block_addr newBlockAddr = getBlockNum();
	int tupleSize = block.getTupleSize(), noNextBlock = -1;
	int numOfBlock = ( BLOCK_SIZE - BLOCK_HEAD_SIZE ) / tupleSize;
	block.offset = 0;
	block.writeInt(&numOfBlock);
	block.writeInt(&tupleSize);
	block.writeInt(&noNextBlock);
	// 更新上一个块的next_block
	updatePreviousBlock(newBlockAddr);
	return true;
}

bool File::updatePreviousBlock(block_addr addr){
	if(addr == 0)
		return true;
	Block tmp = Block();
	readBlock(&tmp);

	tmp.offset = BLOCK_HEAD_NEXT_OFFSET;
	tmp.writeInt(&addr);

	writeBlock(&tmp);
	return true;
}

bool File::readBlock(Block *block) {
	// if file is empty
	if(getBlockNum() == 0){
		newFile = true;
		return false;
	}
	fstream rdtable;
	rdtable.open((tablePath).data(), ios::in | ios::binary);
	rdtable.seekg(addr * BLOCK_SIZE);
	rdtable.read(block->data, BLOCK_SIZE);
	rdtable.close();
	return true;
}

bool File::writeBlock(Block *block) {
	fstream wrtable;
	wrtable.open((tablePath).data(), ios::out | ios::binary);
	wrtable.seekg(BLOCK_SIZE * addr);
	wrtable.write(block->data, BLOCK_SIZE);
	wrtable.close();
	return false;
}

int File::getBlockNum() {
	struct stat info;
	stat(tablePath.data(), &info);
	int num = info.st_size / BLOCK_SIZE;
	return num;
}

bool File::setTablePath(string path) {
	this->tablePath = path;
	return true;
}

bool File::setBlockAddr(block_addr addr) {
	if (addr >= 0) {
		this->searchAll = false;
		this->addr = addr;
	} else {
		// set the flag
		this->searchAll = true;
		this->addr = 0;
	}
	return true;
}

