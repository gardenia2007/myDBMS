/*
 * const.h
 *
 *  Created on: 2012-10-12
 *      Author: y
 */

#ifndef CONST_H_
#define CONST_H_

// 数据库文件路径
#define DATA_PATH		"/home/y/myDBMS/"
// 数据库基础文件名
#define DB_BASIC_FILE_NAME	"db"
#define DATA_FILE_NAME	"data"
#define MODAL_FILE_NAME	"model"
#define TMP_FILE_NAME	"tmp"

#define MAX_SQL_LENGTH	128
#define MAX_DBNAME_LEGTH	32

#define DEFAULT_SIZE		32

#define MAX_NAME_SIZE			32
#define MAX_VALUE_SIZE			32
#define MAX_CONDITION_SIZE		32
#define MAX_DATABASENAME_SIZE	32

#define INVAILD_OPREATE	-1
#define CREATE_TABLE	1
#define UPDATE_TABLE	2
#define DELETE_TABLE	3
#define SELECT_TABLE	4
#define INSERT_TABLE	5
#define CREATE_DATABASE	6
#define DELETE_DATABASE	7
#define USE_DATABASE	8

#define BLOCK_SIZE		1024


struct Data{
	char name[MAX_NAME_SIZE];
	char value[MAX_VALUE_SIZE];
	char opreate;
	char value2[MAX_VALUE_SIZE];
	int num;
	Data * next;
};

#endif /* CONST_H_ */
