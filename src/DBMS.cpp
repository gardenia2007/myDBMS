/*
 * DBMS.cpp
 *
 *  Created on: 2012-10-12
 *      Author: y
 */

#include "DBMS.h"

DBMS::DBMS() {
    this->operate = SELECT_TABLE;
    this->db = new DB();
    this->pos = 0;
    data = NULL;
}

void DBMS::run() {
    cout << "Welcome to myDBMS!\nPress 'q' to exit" << endl;
    while (1) {
        result = "";
        cout << ">";
        getline(cin, this->sql);
        if (sql == "q") {
            cout << "bye" << endl;
            return;
        }
        this->parseOpreate();
        this->dispatchSql();
        cout << this->result << endl;
    }
}

bool DBMS::praseSelect(Data *q, string seg) {
    Data *p = new Data;
    q = p;
    p->next = NULL;
    string str = "";
    int judge = this->getNextWord(p->name, DEFAULT_SIZE);
    while (!endOfSql()) {
        if (judge == -1) {
            return false;
        } else {
            if (str.append(p->name) == seg) {
                return true;
            } else {
                str = "";
                p->next = new Data;
                p = p->next;
                judge = this->getNextWord(p->name, DEFAULT_SIZE);
            }
        }
    }
}

bool DBMS::praseSelConnditon() {
    Data *p = new Data;
    this->qualification = p;
    p->next = NULL;
    int judge;
    this->getNextWord(p->name, DEFAULT_SIZE);
    while (!endOfSql()) {
        judge = this->getNextWord(p->name, DEFAULT_SIZE);
        if (judge != -1)
            judge = this->getNextWord(p->value, DEFAULT_SIZE);
        if (judge != -1)
            judge = this->getNextWord(p->value2, DEFAULT_SIZE);
        if (judge == -1)
            return false;
        else {
            p->next = new Data;
            p = p->next;
        }
    }
    return true;
}

void DBMS::select() {
<<<<<<< HEAD
    bool judge = true;
    judge = this->praseSelect(data, "from");
    if (judge == true)
        judge = this->praseSelect(tables, "where");
    if (judge == true)
        judge = this->praseSelConnditon();
    if(judge == true){
        
    }else{
        
    }
=======
	cout << "i am select" << endl;
>>>>>>> 7316c5c6b2007f46e35b18518cdb5476a6609877
}

bool DBMS::parseInsert() {
    int i = 0;
    Data *p = new Data;

    if (getNextWord(tableName, DEFAULT_SIZE) == -1)
        return false;
    pos++; // skip '('

    data = p; // head
    p->next = p;
    //TODO 没有检查返回值是-1的情况
    while (!endOfSql()) {
        p = p->next;
        getNextWord(p->name, DEFAULT_SIZE);
        p->num = i++; // 这里的num表示字段的序号
        p->next = new Data;
    }
    delete p->next;
    p->next = NULL;

    return true;
}

void DBMS::insertTable() {
    if (!parseInsert() || !db->insertTable(tableName, data)) {
        result = "insert table <" + string(this->tableName) + "> fail!";
//        return false;
    } else {
        result = "insert table <" + string(this->tableName) + "> success!";
//        return true;
    }
}

bool DBMS::parseCreateTable() {
    if (getNextWord(tableName, 32) == -1)
        return false;
    this->pos++; // skip '('

    // 生成链表
    Data *p = new Data;
    data = p; // head
    p->next = p;
    //TODO 没有检查返回值是-1的情况
    while (!endOfSql()) {
        p = p->next;
        getNextWord(p->name, 32);
        p->num = getNextWord(p->value, 32); // 如果是char，返回值是char的长度，否则为0
        p->next = new Data;
    }
    delete p->next;
    p->next = NULL;

    return true;
}

void DBMS::deleteTable() {

//    return true;

}

void DBMS::updateTable() {
//    return true;
}

void DBMS::createTable() {
    if (!parseCreateTable() || !db->createTable(tableName, data)) {
        result = "create table <" + string(this->tableName) + "> fail!";
//        return false;
    } else {
        result = "create table <" + string(this->tableName) + "> success!";
//        return true;
    }
}

bool DBMS::useDB() {
    if (!this->parseDBName() || !this->db->useDB(this->dbName)) {
        result = "database <" + string(dbName) + "> not found!";
        return false;
    } else {
        result = "use database <" + string(dbName) + "> success!";
        return true;
    }
}

bool DBMS::createDB() {
    if (!this->parseDBName() || !this->db->createDB(this->dbName)) {
        result = "create database <" + string(dbName) + "> fail!";
        return false;
    } else {
        result = "create database <" + string(dbName) + "> success!";
        return true;
    }
}

bool DBMS::deleteDB() {
    //TODO 没写删除数据库
    if (!this->parseDBName()) {
        result = "delete database <" + string(dbName) + "> fail!";
        return false;
    } else {
        result = "delete database <" + string(dbName) + "> success!";
        return false;
    }
}

bool DBMS::parseDBName() {
    if (this->getNextWord(this->dbName, MAX_DBNAME_LEGTH) != -1)
        return true;
    else
        return false;
}

bool DBMS::endOfSql() {
    if (sql.length() == pos || (sql.length() == pos + 1 && sql[pos] == ')')) // sql结束
        return true;
    else
        return false;
}

int DBMS::getNextWord(char* word, int size = 32) {
    int i = 0, numOfChar, j;
    char tmpNumOfChar[32];
    while (1) {
        if (endOfSql()) { // sql结束
            word[i] = '\0';
            return 0;
        } else if (i >= size) { // word容量不够
            return -1;
        }
        switch (sql[pos]) {
            case ' ': //first 执行此函数前要去掉空格
            case ';':
            case ':':
            case ')':
            case ',':
                word[i] = '\0';
                pos++; // skip this null character
                return 0;
                break;

            case '"': // 如果读到"，那么简单的忽略(对insert语句来说)
                pos++;
                break;

            case '(': // 只有insert语句的char的后面会有'('
                pos++;
                j = 0;
                while (sql[pos] != ')') { // 读取括号中得数字
                    tmpNumOfChar[j] = sql[pos];
                    pos++;
                    j++;
                }
                word[i] = '\0';
                pos++; // skip ')'
                pos++; // if have ',' skip it
                numOfChar = atoi(tmpNumOfChar);
                return numOfChar;
                break;

            case '=':
            case '>':
            case '<':
                word[i] = sql[pos];
                pos++;
                return 0;
                break;

            default:
                word[i] = sql[pos];
                pos++;
                i++;
                break;
        }
    }
    return 0;
}

void DBMS::dispatchSql() {
    //	cout << this->operate << endl;
    switch (this->operate) {
        case SELECT_TABLE:
            this->select();
            break;
        case INSERT_TABLE:
            this->insertTable();
            break;
        case UPDATE_TABLE:
            this->updateTable();
            break;
        case DELETE_TABLE:
            this->deleteTable();
            break;
        case CREATE_TABLE:
            this->createTable();
            break;
        case CREATE_DATABASE:
            this->createDB();
            break;
        case USE_DATABASE:
            this->useDB();
            break;
        case DELETE_DATABASE:
            this->deleteDB();
            break;
        default:
            return;
    }
}

void DBMS::parseOpreate() {
    this->pos = this->sql.find(' ');
    string word = this->sql.substr(0, this->pos);
    this->pos++; // skip white space
    //	cout << word << endl;

    if (word == "select")
        this->operate = SELECT_TABLE;
    else if (word == "update")
        this->operate = UPDATE_TABLE;
    else if (word == "insert")
        this->operate = INSERT_TABLE;
    else if (word == "createtable")
        this->operate = CREATE_TABLE;
    else if (word == "deletetable")
        this->operate = DELETE_TABLE;
    else if (word == "use")
        this->operate = USE_DATABASE;
    else if (word == "createdb")
        this->operate = CREATE_DATABASE;
    else if (word == "deletedb")
        this->operate = DELETE_DATABASE;
    else
        this->operate = INVAILD_OPREATE;
}

DBMS::~DBMS() {
    // TODO Auto-generated destructor stub
}

