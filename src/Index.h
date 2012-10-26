/*
 * Index.h
 *
 * 索引
 *
 *  Created on: 2012-10-17
 *      Author: y
 */

#ifndef INDEX_H_
#define INDEX_H_
#include "const.h"
#include "Block.h"
#include "File.h"

class Index {
public:
    Index();
    virtual ~Index();

    block_addr getBlockAddr(int); // 根据索引域返回block指针（即块号）

    bool initIndex(string *tablePath, Model *); // 建立索引
    bool updateIndex(); // 更新索引

private:

    File f;

    int hash_num;

    block_addr simpleHash(int); // 简单Hash
    block_addr hash(int); // Hash 函数
    bool initSimpleHash();

};

#endif /* INDEX_H_ */
