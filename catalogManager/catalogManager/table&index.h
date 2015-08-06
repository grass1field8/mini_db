//
//  table&index.h
//  catalogManager
//
//  Created by 黄佳睿 on 14-10-31.
//  Copyright (c) 2014年 黄佳睿. All rights reserved.
//

#ifndef __catalogManager__table_index__
#define __catalogManager__table_index__

#include <stdio.h>
#include <iostream>
using namespace std;
class attributeNode{
public:
    string tableName;
    string attributeName;
    char type;       //0, 1, 2 represent int, char(), float respectively
    char isUnique;
    int length;    //if char()
    void *data;
    int offset;
    int index;
    attributeNode *next;
    attributeNode(){
        data = NULL;
        next = NULL;
        index = -1;
    }
    ~attributeNode(){
        if (data != NULL) {
            free(data);
        }
    }
};

class table{
public:
    string tableName;
    string primaryKeyName;
    int attributeNum;
    int recordSize;
    int location;
    int aLocation;
    int insertH;
    int deleteH;
    int recordNum;
    table(){
        attributeNum = 0;
        recordSize = 0;
        recordNum = 0;
    }
};

class tableRow{
public:
    char type;
    void *data;
    tableRow *next;
    tableRow(){
        next = NULL;
    }
};

class indexx{
public:
    string tableName;
    string attributeName;
    int head;
    int left;
};


#endif /* defined(__catalogManager__table_index__) */
