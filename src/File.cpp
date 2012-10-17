/*
 * Block.cpp
 *
 *  Created on: 2012-10-17
 *      Author: y
 */

#include "File.h"

File::File() {

}

File::~File() {
	// TODO Auto-generated destructor stub
}

tuple * File::getTuple() {

}

bool File::writeTuple(Data * d){
//	if(!hasRemainSpace()){
//		readNextBlock();
//	}else{

//	}

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

void File::readBlock(block_addr addr) {
	fstream rdtable;
	rdtable.open((tablePath).data(), ios::in | ios::binary);
	rdtable.seekg(addr * BLOCK_SIZE);
	rdtable.read(block, BLOCK_SIZE);
	rdtable.close();
}

void File::writeBlock(block_addr addr) {
	fstream wrtable;
	wrtable.open((tablePath).data(), ios::out | ios::binary);
	wrtable.seekg(BLOCK_SIZE * addr);
	wrtable.write(block, BLOCK_SIZE);
	wrtable.close();
}

