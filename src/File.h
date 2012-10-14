/*
 * File.h
 *	文件操作类,维护DBMS的数据文件
 *  Created on: 2012-10-14
 *      Author: y
 */

#ifndef FILE_H_
#define FILE_H_
#include "const.h"

class File {
public:
	File();
	virtual ~File();

	// 外部接口，供DBMS调用
	bool selectDatabase(char* databaseName);

	bool read();
	bool write();

private:
	char databaseName[MAX_DATABASENAME_SIZE];
	// 按块读写
	void readBlock();
	void writeBlock();

};

#endif /* FILE_H_ */
