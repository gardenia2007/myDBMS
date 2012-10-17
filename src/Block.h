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

class Block {
public:
	Block();
	virtual ~Block();

private:
	void readBlock(int);
	void writeBlock(int);
};

#endif /* BLOCK_H_ */
