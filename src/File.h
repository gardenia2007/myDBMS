/*
 * Block.h
 *
 * 封装文件块操作
 *
 *  Created on: 2012-10-17
 *      Author: y
 */

#ifndef BLOCK_H_
#define BLOCK_H_
#include "const.h"

#include <fstream>
#include <string>

using namespace std;

//#include <string>
//#include "File.h"

class File {
public:
	File(); //
	virtual ~File();

	bool setTablePath(string); //
	bool setBlockAddr(block_addr); // 设置起始块地址,if(block_addr == -1)，表示在非索引域的搜索，处理所有元组

	tuple * getTuple(); // 获取下一个元组，没有了就返回false
	bool writeTuple(Data *); // 写入一个元组

private:
//	File file;
	char block[1024];
	string tablePath;
	block_addr addr;
	bool searchAll;

	bool hasRemainSpace();

	void getNextBlockAddr();
	void readNextBlock();
	void getTupleSize();
	void readData(char *);
	void writeData(const char *);

	void readBlock(block_addr);
	void writeBlock(block_addr);
};

#endif /* BLOCK_H_ */
