//
//  recordManager.cpp
//  recordManager
//
//  Created by 黄佳睿 on 14-11-5.
//  Copyright (c) 2014年 黄佳睿. All rights reserved.
//

#include "recordManager.h"
#include "catalogManager.h"

string num2str(double i)
{
    stringstream ss;
    ss<<i;
    return ss.str();
}

string num2str(int i)
{
    stringstream ss;
    ss<<i;
    return ss.str();
}


double str2num(string s)
{
    double num;
    stringstream ss(s);
    ss>>num;
    return num;
}

response *recordManager::selectRecord(request &req, bufferManager &buf){
    response *resH = NULL;
    
    
    catalogManager cat;
    
    if(cat.chooseTable(req.tableName) != 0){
        resH = new response;
        resH->error = TABLE_ERROR;
        return resH;
    }
    table &choosedTable = cat.getTableInfo();
    
    //cout<<"record size:"<<choosedTable.recordSize<<endl;
    //cout<<"i&d:"<<choosedTable.insertH<<" "<<choosedTable.deleteH<<endl;
    //get the block contains table data
    char *buffer;
    char *record;
    buffer = (char *)buf.readBlock("/Users/hjr/code/test/testdb", choosedTable.location);
    
    //initial the mark
    int offset = choosedTable.insertH;
    
    while (offset >= 0) {
        
        record = buffer + offset*choosedTable.recordSize;
        *(record + choosedTable.recordSize - 1) = '1';
        offset = *((int *)(record + choosedTable.recordSize - 1 - sizeof(int)));
    }
    
    
    condition *tempCondition;
    
    attributeNode attributeInfo;
    //loop all condition and mark record
    
    offset = choosedTable.insertH;
    while (offset >= 0) {
        //get record
        record = buffer + offset*choosedTable.recordSize;
        
        tempCondition = req.conditionHead;
        while (tempCondition != NULL) {
            //get attribute info
            if(cat.getAttributeByName(tempCondition->attributeName, attributeInfo)){
                resH = new response;
                resH->error = ATTRIBUTE_ERROR;
                return resH;
            }
        
        
            
            //judge and mark
            if (attributeInfo.type == '0') {
                int a, b;
                string str;
                str = (char *)tempCondition->data;
                a = (int)str2num(str);
                b = *((int *)(record + attributeInfo.offset));
                if (tempCondition->type == '0') {
                    if (a != b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '1'){
                    if (a >= b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '2'){
                    if (a <= b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '3'){
                    if (a > b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else{
                    if (a < b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
            }
            
            else if (attributeInfo.type == '1') {
                char *a, *b;
                a = (char *)tempCondition->data;
                b = (char *)(record + attributeInfo.offset);
                if (tempCondition->type == '0') {
                    if (strcmp(a, b) != 0) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '1'){
                    if (strcmp(a, b) > 0 || strcmp(a, b) == 0) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '2'){
                    if (strcmp(a, b) <0 || strcmp(a, b) == 0) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '3'){
                    if (strcmp(a, b) > 0) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else{
                    if (strcmp(a, b) < 0) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
            }
            
            else{
                float a, b;
                string str;
                str = (char *)tempCondition->data;
                a = str2num(str);
                b = *((float *)(record + attributeInfo.offset));
                if (tempCondition->type == '0') {
                    if (a != b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '1'){
                    if (a >= b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '2'){
                    if (a <= b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '3'){
                    if (a > b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else{
                    if (a < b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
            }
            
            
            
            
            
            tempCondition = tempCondition->next;
            
        }
        
        offset = *( (int *)(record + choosedTable.recordSize - sizeof(int) -1) );
    }
    
    response *tempRes = NULL;
    int f1 = 0;
    offset = choosedTable.insertH;
    while (offset >= 0) {
        record = buffer + offset*choosedTable.recordSize;
        if (*(record + choosedTable.recordSize - 1) == '1') {
            attributeNode *a;
            a = req.attributeHead;
            tableRow *h = NULL;
            tableRow *t = NULL;
            int f2 = 0;
            while (a != NULL) {
                if (cat.getAttributeByName(a->attributeName, attributeInfo) && cat.getAttributeByIndex(a->index, attributeInfo)) {
                    //cout<<cat.getAttributeByIndex(a->index, attributeInfo)<<endl;
                    resH = new response;
                    resH->error = ATTRIBUTE_ERROR;
                    return resH;
                }
                if (f2 == 0) {
                    t = new tableRow;
                    h = t;
                    f2 = 1;
                }
                else{
                    t->next = new tableRow;
                    t = t->next;
                }
                t->type = attributeInfo.type;
                t->data = (void *)malloc(attributeInfo.length);
                memcpy(t->data, record + attributeInfo.offset, attributeInfo.length);
                a = a->next;
            }
            if (f1 == 0) {
                tempRes = new response;
                resH = tempRes;
                f1 = 1;
            }
            else{
                tempRes->next = new response;
                tempRes = tempRes->next;
            }
            tempRes->tableRowHead = h;
        }
        offset = *( (int *)(record + choosedTable.recordSize - sizeof(int) -1) );
        
    }
    
    
    
    //write back table infomation
    buf.writeBlock("/Users/hjr/code/test/testdb", choosedTable.location, (void *)buffer);
    cat.updateTable();
    return resH;
}


//
//
response *recordManager::deleteRecord(request &req, bufferManager &buf){
    response *resH = NULL;
    
    
    catalogManager cat;
    
    if(cat.chooseTable(req.tableName) != 0){
        resH = new response;
        resH->error = 2;
        return resH;
    }
    table &choosedTable = cat.getTableInfo();
    
    
    //get the block contains table data
    char *buffer;
    char *record;
    buffer = (char *)buf.readBlock("/Users/hjr/code/test/testdb", choosedTable.location);
    
    //initial the mark
    int offset = choosedTable.insertH;
    
    while (offset >= 0) {
        
        record = buffer + offset*choosedTable.recordSize;
        *(record + choosedTable.recordSize - 1) = '1';
        offset = *((int *)(record + choosedTable.recordSize - 1 - sizeof(int)));
    }
    
    
    condition *tempCondition;
    
    attributeNode attributeInfo;
    //loop all condition and mark record
    
    offset = choosedTable.insertH;
    while (offset >= 0) {
        //get record
        record = buffer + offset*choosedTable.recordSize;
        
        tempCondition = req.conditionHead;
        while (tempCondition != NULL) {
            //get attribute info
            if(cat.getAttributeByName(tempCondition->attributeName, attributeInfo)){
                resH = new response;
                resH->error = ATTRIBUTE_ERROR;
                return resH;
            }
            
            
            
            //judge and mark
            if (attributeInfo.type == '0') {
                int a, b;
                string str;
                str = (char *)tempCondition->data;
                a = (int)str2num(str);
                b = *((int *)(record + attributeInfo.offset));
                if (tempCondition->type == '0') {
                    if (a != b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '1'){
                    if (a >= b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '2'){
                    if (a <= b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '3'){
                    if (a > b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else{
                    if (a < b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
            }
            
            else if (attributeInfo.type == '1') {
                char *a, *b;
                a = (char *)tempCondition->data;
                b = (char *)(record + attributeInfo.offset);
                if (tempCondition->type == '0') {
                    if (strcmp(a, b) != 0) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '1'){
                    if (strcmp(a, b) > 0 || strcmp(a, b) == 0) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '2'){
                    if (strcmp(a, b) <0 || strcmp(a, b) == 0) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '3'){
                    if (strcmp(a, b) > 0) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else{
                    if (strcmp(a, b) < 0) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
            }
            
            else{
                float a, b;
                string str;
                str = (char *)tempCondition->data;
                a = str2num(str);
                b = *((float *)(record + attributeInfo.offset));
                if (tempCondition->type == '0') {
                    if (a != b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '1'){
                    if (a >= b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '2'){
                    if (a <= b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '3'){
                    if (a > b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else{
                    if (a < b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
            }
            
            
            
            
            
            tempCondition = tempCondition->next;
            
        }
        
        offset = *( (int *)(record + choosedTable.recordSize - sizeof(int) -1) );
    }

    
    int temp;
    offset = choosedTable.insertH;
    temp = choosedTable.insertH;
    while (offset >= 0) {
        int nextP;
        cout<<"insertHead:"<<choosedTable.insertH<<endl;
        cout<<"deletehead:"<<choosedTable.deleteH<<endl;
        cout<<"temp:"<<temp<<endl;
        cout<<"offset:"<<offset<<endl;
        record = buffer + offset*choosedTable.recordSize;
        nextP = *( (int *)(record + choosedTable.recordSize - sizeof(int) -1) );
        if (*(record + choosedTable.recordSize - 1) == '1') {
            // if head deleted
            cout<<endl;
            cout<<"here"<<endl;
            if (offset == choosedTable.insertH) {
                choosedTable.insertH = *( (int *)(record + choosedTable.recordSize - sizeof(int) -1) );
                memcpy((record + choosedTable.recordSize - sizeof(int) -1), &choosedTable.deleteH, sizeof(int));
                choosedTable.deleteH = offset;
            }
            else{
                memcpy(buffer + (temp+1)*choosedTable.recordSize - 1 - sizeof(int), (record + choosedTable.recordSize - sizeof(int) -1), sizeof(int));
                memcpy((record + choosedTable.recordSize - sizeof(int) -1), &choosedTable.deleteH, sizeof(int));
                choosedTable.deleteH = offset;
            }
            choosedTable.recordNum--;
            
        }
        else{
            temp = offset;
        }
        offset = nextP;
    }
    
    //write back table infomation
    buf.writeBlock("/Users/hjr/code/test/testdb", choosedTable.location, (void *)buffer);
    cat.updateTable();
    resH = new response;
    return resH;
}



//
//
response *recordManager::updateRecord(request &req, bufferManager &buf){
    response *resH = NULL;
    
    
    catalogManager cat;
    if(cat.chooseTable(req.tableName) != 0){
        resH = new response;
        resH->error = 2;
        return resH;
    }
    table &choosedTable = cat.getTableInfo();
    
    
    //get the block contains table data
    char *buffer;
    char *record;
    buffer = (char *)buf.readBlock("/Users/hjr/code/test/testdb", choosedTable.location);
    
    //initial the mark
    int offset = choosedTable.insertH;
    
    while (offset >= 0) {
        
        record = buffer + offset*choosedTable.recordSize;
        *(record + choosedTable.recordSize - 1) = '1';
        offset = *((int *)(record + choosedTable.recordSize - 1 - sizeof(int)));
    }
    
    
    condition *tempCondition;
    
    attributeNode attributeInfo;
    //loop all condition and mark record
    
    offset = choosedTable.insertH;
    while (offset >= 0) {
        //get record
        record = buffer + offset*choosedTable.recordSize;
        
        tempCondition = req.conditionHead;
        while (tempCondition != NULL) {
            //get attribute info
            if(cat.getAttributeByName(tempCondition->attributeName, attributeInfo)){
                resH = new response;
                resH->error = ATTRIBUTE_ERROR;
                return resH;
            }
            
            
            
            //judge and mark
            if (attributeInfo.type == '0') {
                int a, b;
                string str;
                str = (char *)tempCondition->data;
                a = (int)str2num(str);
                b = *((int *)(record + attributeInfo.offset));
                if (tempCondition->type == '0') {
                    if (a != b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '1'){
                    if (a >= b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '2'){
                    if (a <= b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '3'){
                    if (a > b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else{
                    if (a < b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
            }
            
            else if (attributeInfo.type == '1') {
                char *a, *b;
                a = (char *)tempCondition->data;
                b = (char *)(record + attributeInfo.offset);
                if (tempCondition->type == '0') {
                    if (strcmp(a, b) != 0) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '1'){
                    if (strcmp(a, b) > 0 || strcmp(a, b) == 0) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '2'){
                    if (strcmp(a, b) <0 || strcmp(a, b) == 0) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '3'){
                    if (strcmp(a, b) > 0) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else{
                    if (strcmp(a, b) < 0) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
            }
            
            else{
                float a, b;
                string str;
                str = (char *)tempCondition->data;
                a = str2num(str);
                b = *((float *)(record + attributeInfo.offset));
                if (tempCondition->type == '0') {
                    if (a != b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '1'){
                    if (a >= b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '2'){
                    if (a <= b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else if(tempCondition->type == '3'){
                    if (a > b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
                else{
                    if (a < b) {
                        *(record + choosedTable.recordSize - 1) = '0';
                    }
                }
            }
            
            
            
            
            
            tempCondition = tempCondition->next;
            
        }
        
        offset = *( (int *)(record + choosedTable.recordSize - sizeof(int) -1) );
    }
    
    response *tempRes = NULL;
    int f1 = 0;
    offset = choosedTable.insertH;
    while (offset >= 0) {
        record = buffer + offset*choosedTable.recordSize;
        if (*(record + choosedTable.recordSize - 1) == '1') {
            
            attributeNode *a;
            a = req.attributeHead;
            tableRow *h = NULL;
            tableRow *t = NULL;
            int f2 = 0;
            while (a != NULL) {
                if (cat.getAttributeByName(a->attributeName, attributeInfo) && cat.getAttributeByIndex(a->index, attributeInfo)) {
                    //cout<<cat.getAttributeByIndex(a->index, attributeInfo)<<endl;
                    resH = new response;
                    resH->error = ATTRIBUTE_ERROR;
                    return resH;
                }
                if (f2 == 0) {
                    t = new tableRow;
                    h = t;
                    f2 = 1;
                }
                else{
                    t->next = new tableRow;
                    t = t->next;
                }
                t->type = attributeInfo.type;
                if (attributeInfo.type == '0') {
                    string str;
                    int ii;
                    str = (char *)a->data;
                    ii = (int)str2num(str);
                    memcpy(record + attributeInfo.offset, &ii, attributeInfo.length);
                }
                if(attributeInfo.type == '1'){
                    memcpy(record + attributeInfo.offset, a->data, attributeInfo.length);
                }
                if(attributeInfo.type == '2'){
                    string str;
                    float ff;
                    str = (char *)a->data;
                    ff = str2num(str);
                    memcpy(record + attributeInfo.offset, &ff, attributeInfo.length);
                }
                
                t->data = (void *)malloc(attributeInfo.length);
                memcpy(t->data, record + attributeInfo.offset, attributeInfo.length);
                a = a->next;
            }
            if (f1 == 0) {
                tempRes = new response;
                resH = tempRes;
                f1 = 1;
            }
            else{
                tempRes->next = new response;
                tempRes = tempRes->next;
            }
            tempRes->tableRowHead = h;
        }
        
        offset = *( (int *)(record + choosedTable.recordSize - sizeof(int) -1) );
        
    }

        
    //write back table infomation
    buf.writeBlock("/Users/hjr/code/test/testdb", choosedTable.location, (void *)buffer);
    cat.updateTable();
    resH = new response;
    return resH;
}

//
//
response *recordManager::insertRecord(request &req, bufferManager &buf){
    response *resH = NULL;
    
    
    catalogManager cat;
    //cout<<"table name:"<<req.tableName<<endl;
    if(cat.chooseTable(req.tableName) != 0){
        resH = new response;
        resH->error = TABLE_ERROR;
        return resH;
    }
    table &choosedTable = cat.getTableInfo();
    
    
    //get the block contains table data
    char *buffer;
    char *record;
    buffer = (char *)buf.readBlock("/Users/hjr/code/test/testdb", choosedTable.location);
    
    int offset;
    offset = choosedTable.deleteH;
    if (offset >= 0) {
        record = buffer + offset*choosedTable.recordSize;
        choosedTable.deleteH = *( (int *)(record + choosedTable.recordSize - sizeof(int) -1) );
        memset(record, 0, choosedTable.recordSize);
        memcpy(record+choosedTable.recordSize-1-sizeof(int), &(choosedTable.insertH), sizeof(int));
        choosedTable.insertH = offset;
    }
    else{
        record = buffer + choosedTable.recordNum*choosedTable.recordSize;
        memset(record, 0, choosedTable.recordSize);
        memcpy(record+choosedTable.recordSize-1-sizeof(int), &(choosedTable.insertH), sizeof(int));
        choosedTable.insertH = choosedTable.recordNum;
    }
    attributeNode *a;
    attributeNode attributeInfo;
    a = req.attributeHead;
    
    while (a != NULL) {
        if (cat.getAttributeByName(a->attributeName, attributeInfo) && cat.getAttributeByIndex(a->index, attributeInfo)) {
            resH = new response;
            resH->error = ATTRIBUTE_ERROR;
            return resH;
        }
        if (attributeInfo.type == '0') {
            string str;
            int ii;
            str = (char *)a->data;
            ii = (int)str2num(str);
            memcpy(record + attributeInfo.offset, &ii, attributeInfo.length);
        }
        if(attributeInfo.type == '1'){
            memcpy(record + attributeInfo.offset, a->data, attributeInfo.length);
        }
        if(attributeInfo.type == '2'){
            string str;
            float ff;
            str = (char *)a->data;
            ff = str2num(str);
            memcpy(record + attributeInfo.offset, &ff, attributeInfo.length);
        }
        a = a->next;
    }
    
    choosedTable.recordNum++;
    //write back table infomation
    //cout<<"choosed table location:"<<choosedTable.location<<endl;
    buf.writeBlock("/Users/hjr/code/test/testdb", choosedTable.location, (void *)buffer);
    //cout<<"tableInfo:"<<choosedTable.recordSize<<choosedTable.recordNum<<endl;
    cat.updateTable();
    resH = new response;
    return resH;
}

