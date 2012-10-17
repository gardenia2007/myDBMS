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
	File(); //
	virtual ~File();

	Model * model;

	bool setTablePath(string); //
	bool setBlockAddr(block_addr); // 设置起始块地址,if(block_addr == -1)，表示在非索引域的搜索，处理所有元组

	tuple * getTuple(); // 获取下一个元组，没有了就返回false
	bool writeTuple(Data *); // 写入一个元组

private:
	string tablePath;
	block_addr addr, previous;

	Block block;

	bool searchAll, newFile;


	void readNextBlock();

	bool newBlock();

	bool updatePreviousBlock(block_addr); // 更新上一个Block的next_block值

	int getBlockNum(); // how many in current file?
	bool readBlock(Block *); // read a block from file to this->block
	bool writeBlock(Block *);// write a this->block to file
};

#endif /* BLOCK_H_ */
