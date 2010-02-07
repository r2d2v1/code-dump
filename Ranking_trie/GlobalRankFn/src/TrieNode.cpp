//============================================================================
// Name        : TrieNode.cpp
// Author      : Vijay - [rvijax [ at ] gmail [ dot ] com]
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "TrieNode.h"

TrieNode::TrieNode(){
	this->count=0;
	this->name=-1;
	this->endFlag=false;
	//cout<<"TrieNode constr"<<endl;
	this->parent=NULL;
	//this->child=NULL;
}


TrieNode::~TrieNode(){
	//cout<<"TrieNode destr"<<endl;
	//this->child.clear();
	this->parent=NULL;
	//free(this);
}
void TrieNode::setEndFlag(bool in,const long &docID){
	this->forwardList.append(docID);
	this->endFlag=in;
}

//getters
bool TrieNode::getEndFlag(){
	return this->endFlag;
}


