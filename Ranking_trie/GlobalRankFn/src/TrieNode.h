/*
 * TrieNode.h
 *
 *  Created on: Nov 3, 2009
 *      Author: vijay
 */

#ifndef TRIENODE_H_
#define TRIENODE_H_

#include "keyarray.h"
#include "array.h"
#include "assert.h"
#include "stdlib.h"

class TrieNode{
public:
	long name;
	bool endFlag;
	int count;
	TrieNode *parent;
	Array<long> forwardList;
	Array<TrieNode*> child;
	KeyArray<unsigned long> keys;

	TrieNode();
	virtual ~TrieNode();

	/**
	 * getters & setters
	 */

	void setParent(TrieNode* Parent){this->parent=parent;}
	//long getName(){return this->name;}
	//void setName(long name){this->name=name;}
	void setEndFlag(bool in,const long &docID);
	bool getEndFlag();

	//Array<long>& getForwardList(){return this->forwardList;};

};


#endif /* TRIENODE_H_ */
