//
//  main.cpp
//  catalogManager
//
//  Created by 黄佳睿 on 14-10-30.
//  Copyright (c) 2014年 黄佳睿. All rights reserved.
//

#include <iostream>
#include "table&index.h"
#include "catalogManager.h"
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;

int main(int argc, const char * argv[]) {
//    int fd;
//    int i[1];
//    i[0] = 0;
//    fd = open("/Users/hjr/code/test/info", O_RDWR);
//    read(fd, i, sizeof(int));
//    cout<<i[0]<<endl;
    
    
    table tempTable;
    tempTable.tableName = "testtable";
    tempTable.primaryKeyName = "int";
    tempTable.attributeNum = 3;
    tempTable.recordSize = 100;
    attributeNode head;
    attributeNode one;
    attributeNode two;
    head.next = &one;
    one.next = &two;
    head.tableName = "testtable";
    head.attributeName = "int";
    head.type = '0';
    head.isUnique = '1';
    one.tableName = "testtable";
    one.attributeName = "char";
    one.length = 10;
    one.type = '1';
    one.isUnique = '0';
    two.tableName = "testtable";
    two.attributeName = "float";
    two.type = '2';
    two.isUnique = '0';
    
    catalogManager cat;
    int res;
    string str = "testtable";
   // res = cat.createTable(tempTable, head);
    cout<<"create table return:"<<res<<endl<<endl;
    
    
//    res = cat.dropTable(str);
//    cout<<"drop table return:"<<res<<endl;
    str = "testtable";
    
    res = cat.chooseTable(str);
    cout<<"choose table return:"<<res<<endl;
    if (res == 0) {
        
        table &T = cat.getTableInfo();
        cout<<"tableName:"<<T.tableName<<endl;
        cout<<"primaryKeyName:"<<T.primaryKeyName<<endl;
        cout<<"attributeNum:"<<T.attributeNum<<endl;
        cout<<"recordSize:"<<T.recordSize<<endl;
        cout<<"location:"<<T.location<<endl;
        cout<<"aLocation:"<<T.aLocation<<endl;
        cout<<"insertH:"<<T.insertH<<endl;
        cout<<"deleteH:"<<T.deleteH<<endl;
        cout<<"recordNum:"<<T.recordNum<<endl<<endl;

    
        attributeNode A;
        res = cat.getAttributeByIndex(2, A);
        cout<<"get attribute by name return:"<<res<<endl;
        cout<<"tableName:"<<A.tableName<<endl;
        cout<<"attributeName:"<<A.attributeName<<endl;
        cout<<"type:"<<A.type<<endl;
        cout<<"isUnique:"<<A.isUnique<<endl;
        cout<<"length:"<<A.length<<endl;
        cout<<"offset:"<<A.offset<<endl<<endl;
        
        str = "int";
        res = cat.getAttributeByName(str, A);
        cout<<"get attribute by name return:"<<res<<endl;
        cout<<"tableName:"<<A.tableName<<endl;
        cout<<"attributeName:"<<A.attributeName<<endl;
        cout<<"type:"<<A.type<<endl;
        cout<<"isUnique:"<<A.isUnique<<endl;
        cout<<"length:"<<A.length<<endl;
        cout<<"offset:"<<A.offset<<endl<<endl;
    }
    indexx tempIndex;
    tempIndex.tableName = "testtable";
    tempIndex.attributeName = "int";
 //   res = cat.createIndex(tempIndex);
    cout<<"create index response:"<<res<<endl<<endl;
    
    str = "testTable";
    string str2 = "int";
//    res = cat.dropIndex(str, str2);
//    cout<<"drop index return:"<<res<<endl;
    indexx I;
    str = "testTable";
    str2 = "int";
  //  res = cat.getIndex(str, str2, I);
    cout<<"get index return:"<<res<<endl;
    if (res == 0) {
        cout<<"tableName:"<<I.tableName<<endl;
        cout<<"attributeName:"<<I.attributeName<<endl;
        cout<<"head:"<<I.head<<endl;
        cout<<"left:"<<I.left<<endl<<endl;
    }
    
    
    
    
    return 0;
}
