//
//  request&response.h
//  recordManager
//
//  Created by 黄佳睿 on 14-11-5.
//  Copyright (c) 2014年 黄佳睿. All rights reserved.
//

#ifndef __recordManager__request_response__
#define __recordManager__request_response__

#include <ostream>
#include "table&index.h"
const int STYNAX_ERROR = 1;
const int TABLE_ERROR = 2;
const int ATTRIBUTE_ERROR = 3;
const int OTHER_ERROR = 4;
using namespace std;

class condition{
public:
    char type;   //0, 1, 2, 3, 4 represent =, >, <, >=, <= repectively
    void *data;
    string attributeName;
    condition *next;
    condition(){
        next = NULL;
        data = NULL;
        type = '9';
    }
    ~condition(){
        if (data != NULL) {
            free(data);
        }
    }
};

class request{
public:
    attributeNode *attributeHead;
    condition *conditionHead;
    string tableName;
    request(){
        attributeHead = NULL;
        conditionHead = NULL;
    }
};

class response{
public:
    int error;
    tableRow *tableRowHead;
    response *next;
    response(){
        error = 0;
        tableRowHead = NULL;
        next = NULL;
    }
};

#endif /* defined(__recordManager__request_response__) */
