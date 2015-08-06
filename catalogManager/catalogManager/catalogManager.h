//
//  catalogManager.h
//  catalogManager
//
//  Created by 黄佳睿 on 14-10-30.
//  Copyright (c) 2014年 黄佳睿. All rights reserved.
//

#ifndef __catalogManager__catalogManager__
#define __catalogManager__catalogManager__

#include <stdio.h>
#include <iostream>
#include "table&index.h"
using namespace std;




class catalogManager{
private:
    table choosedTable;
public:
    int chooseTable(string tableName);
    int createTable(table &tempTable, attributeNode &head);
    
    int updateTable(void);
    table &getTableInfo(void);
    int getAttributeByName(string attributeName, attributeNode &tempAttribute);
    int getAttributeByIndex(int index, attributeNode &tempAttribute);
    int dropTable(string tableName);
    
    int createIndex(indexx &tempIndex);
    int dropIndex(string tableName, string attributeName);
    int getIndex(string tableName, string attributeName, indexx &tempIndex);
    
};

#endif /* defined(__catalogManager__catalogManager__) */