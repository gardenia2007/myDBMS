/*
 * Block.h
 *
 * 封装文件块操作
 *
 *  Created on: 2012-10-17
 *      Author: y
 */

#ifndef FILE_H_
#define FILE_H_
#include "const.h"

#include "Block.h"

#include <fstream>
#include <string>
#include <stdlib.h>
#include "sys/stat.h"

using namespace std;

//class Block;

class File {
public:

	friend class Index;
	File(); //
	virtual ~File();

	Model * model;

	bool setTablePath(string); //
	bool setBlockAddr(block_addr); // 设置起始块地址,if(block_addr == -1)，表示在非索引域的搜索，处理所有元组

//	bool readTuple(tuple *); // unused
	bool prepareFetchTuple();
	bool fetchTuple(tuple *); // 获取下一个元组，没有了就返回false
	bool writeTuple(Data *); // 写入一个元组
	bool deleteTuple();

	int getAttributeNumFromModel(Model *); //Num Of Attribute || also used by class DB

	bool writeEmptyBlock(block_addr, int); // 使用空块来初始化data文件

private:
	string tablePath;
	block_addr currentAddr, previousAddr;

//	fstream rdtable, wrtable;

	bool isDeleted();

	Block block;

	bool searchAll, newFile, writeEmptyBlockFlag;

	int getTupleSizeFromModel(Model *); //

	void updateRemainSpace(int);

	bool readNextBlock();

	int newBlock();

	bool updatePreviousBlock(block_addr); // 更新上一个Block的next_block值

	int getBlockNum(); // how many block in current file?

	bool readBlock(Block *, block_addr); // read a block from file to this->block
	bool writeBlock(Block *, block_addr); // write a this->block to file
};

#endif /* BLOCK_H_ */
