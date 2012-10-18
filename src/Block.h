/*
 * Block.h
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

    char data[BLOCK_SIZE];
    int offset;

    void autoOffset(); // 写纪录时自动将偏移设置为block的可写入处

    bool eob(); // END OF BLOCK?
    bool eot(); // END OF TUPLE?

    int getRemainSpace();
    block_addr getNextBlockAddr();
    int getTupleSize(); // each tuple size from this->data(FILE)

    void readChar(char *, int size); // read char from this->block, not file
    int readInt(); // read a int num from this->block, not file

    void writeChar(const char *, int size); //write to this->block, not file
    void writeInt(int *); //write to this->block, not file

};

#endif /* BLOCK_H_ */
