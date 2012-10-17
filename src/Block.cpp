/*
 * Block.cpp
 *
 *  Created on: 2012-10-17
 *      Author: y
 */

#include "Block.h"

Block::Block() {
	// TODO Auto-generated constructor stub



}

tuple * Block::getTuple(){
	char * str = "aksdjf";

	tuple tmp = new tuple[3];

	tmp[0] = str;
	tmp[1] = str;
	tmp[2] = str + 1;


	return tmp;
}

Block::~Block() {
	// TODO Auto-generated destructor stub
}

