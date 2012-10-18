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

class Index {
public:
    Index();
    virtual ~Index();

    int getBlock(Data *); // 根据条件返回block指针（即块号）


};

#endif /* INDEX_H_ */
