//
//  interpreter.cpp
//  intrepreter
//
//  Created by 黄佳睿 on 14-11-11.
//  Copyright (c) 2014年 黄佳睿. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "interpreter.h"
#include "table&index.h"
#include "request&response.h"
#include <stdlib.h>
#include "recordManager.h"
#include "catalogManager.h"
using namespace std;
struct timeval beginTime, endTime;
struct timezone tz;
request req;
table tempTable;
indexx tempIndex;
attributeNode *ap;
condition *cp;
int ind;

//for test
void printReq(void);

void destroy(void);

int printRes(response *r);

int process(int state, char *word, bufferManager &buf, recordManager &rec, catalogManager &cat);

//return 0 when end, return -1 when quit
int phraser(bufferManager &buf, recordManager &rec, catalogManager &cat, int &flag, char *buffer, int &cur, int &end){
    ap = NULL;
    cp = NULL;
    ind = 1;
    char c;
    char word[50];
    int wordP;
    int state = 0;
    wordP = 0;
    string str;
    memset(word, 0, sizeof(word));
    if (flag == 0) {
        while ((c = getchar()) != EOF) {
            //get word
            if (c == '\t' || c == ' ' || c == '\n') {
                if (wordP == 0) {
                    continue;
                }
                else{
                    word[wordP] = '\0';
                    wordP = 0;
                    if ((state = process(state, word, buf, rec, cat)) == 0) {
                        destroy();
                        cout<<"Stynax error"<<endl;
                        cin>>str;
                        return 0;
                    }
                    if (state == -2) {
                        destroy();
                        return 0;
                    }
                    if (state == -1) {
                        destroy();
                        return 1;
                    }
                    if (state == -3) {
                        flag = 1;
                        cin>>str;
                        end = 0;
                        cur = 0;
                        fstream ff(str);
                        ff.read(buffer, 10000);
                        end = (int)ff.gcount() - 1;
//                        cout<<"path:"<<str<<endl;
//                        cout<<"cur:"<<cur<<endl;
//                        cout<<"end:"<<end<<endl;
                        gettimeofday(&beginTime, &tz);
                        break;
                    }
                    //test
//                    cout<<"word:"<<word<<endl;
//                    cout<<"state:"<<state<<endl;
                }
                continue;
            }
            else if((c <= 'Z' && c >= 'A') || (c >= 'a' && c <= 'z' ) || c == '_' || (c >= '0' && c <= '9') || c == '.'){
                word[wordP] = c;
                wordP++;
            }
            else{
                if (wordP != 0) {
                    word[wordP] = '\0';
                    if ((state = process(state, word, buf, rec, cat)) == 0) {
                        destroy();
                        cout<<"Stynax error"<<endl;
                        cin>>str;
                        return 0;
                    }
                    if (state == -2) {
                        destroy();
                        return 0;
                    }
                    if (state == -1) {
                        destroy();
                        return 1;
                    }
                    if(state == -3){
                        flag = 1;
                        cin>>str;
                        end = 0;
                        cur = 0;
                        fstream ff(str);
                        ff.read(buffer, 10000);
                        end = (int)ff.gcount() - 1;
                        //cout<<"path:"<<str<<endl;
                        //cout<<"cur:"<<cur<<endl;
                        //cout<<"end:"<<end<<endl;
                        gettimeofday(&beginTime, &tz);
                        break;
                    }
                    //test
                    //cout<<"word:"<<word<<endl;
                    //cout<<"state:"<<state<<endl;
                }
                word[0] = c;
                word[1] = '\0';
                wordP = 0;
                if ((state = process(state, word, buf, rec, cat)) == 0) {
                    destroy();
                    cout<<"Stynax error"<<endl;
                    cin>>str;
                    return 0;
                }
                if (state == -2) {
                    destroy();
                    return 0;
                }
                if (state == -1) {
                    destroy();
                    return 1;
                }
                if (state == -3) {
                    flag = 1;
                    cin>>str;
                    end = 0;
                    cur = 0;
                    fstream ff(str);
                    ff.read(buffer, 10000);
                    end = (int)ff.gcount() - 1;
//                    cout<<"path:"<<str<<endl;
//                    cout<<"cur:"<<cur<<endl;
//                    cout<<"end:"<<end<<endl;
                    gettimeofday(&beginTime, &tz);
                    break;
                }
                //test
//                cout<<"word:"<<word<<endl;
//                cout<<"state:"<<state<<endl;
            }
        }
    }
    
    //exefile
    destroy();
    state = 0;
    wordP = 0;
    while (cur <= end) {
        c = buffer[cur];
        cur++;
        //get word
        if (c == '\t' || c == ' ' || c == '\n') {
            if (wordP == 0) {
                continue;
            }
            else{
                word[wordP] = '\0';
                wordP = 0;
                if ((state = process(state, word, buf, rec, cat)) == 0) {
                    destroy();
                    cout<<"Stynax error"<<endl;
                    cin>>str;
                    return 0;
                }
                if (state == -2) {
                    destroy();
                    return 0;
                }
                if (state == -1) {
                    destroy();
                    return 1;
                }
                if (state == -3) {
                    cin>>str;
                    break;
                }
                //test
//                cout<<"word:"<<word<<endl;
//                cout<<"state:"<<state<<endl;
            }
            continue;
        }
        else if((c <= 'Z' && c >= 'A') || (c >= 'a' && c <= 'z' ) || c == '_' || (c >= '0' && c <= '9') || c == '.'){
            word[wordP] = c;
            wordP++;
        }
        else{
            if (wordP != 0) {
                word[wordP] = '\0';
                if ((state = process(state, word, buf, rec, cat)) == 0) {
                    destroy();
                    cout<<"Stynax error"<<endl;
                    cin>>str;
                    return 0;
                }
                if (state == -2) {
                    destroy();
                    return 0;
                }
                if (state == -1) {
                    destroy();
                    return 1;
                }
                if(state == -3){
                    cin>>str;
                    break;
                }
                //test
//                cout<<"word:"<<word<<endl;
//                cout<<"state:"<<state<<endl;
            }
            word[0] = c;
            word[1] = '\0';
            wordP = 0;
            if ((state = process(state, word, buf, rec, cat)) == 0) {
                destroy();
                cout<<"Stynax error"<<endl;
                cin>>str;
                return 0;
            }
            if (state == -2) {
                destroy();
                return 0;
            }
            if (state == -1) {
                destroy();
                return 1;
            }
            if (state == -3) {
                cin>>str;
                break;
            }
            //test
//            cout<<"word:"<<word<<endl;
//            cout<<"state:"<<state<<endl;
        }
    }
    gettimeofday(&endTime, &tz);
    cout<<"Use time: "<<endTime.tv_usec - beginTime.tv_usec<<"μs"<<endl;
    cout<<"Executing file end"<<endl;
    flag = 0;
    return 0;
}

void  printReq(void){
    cout<<"Print req:"<<endl;
    cout<<"tableName:"<<tempTable.tableName<<endl;
    cout<<"tableName:"<<req.tableName<<endl;
    attributeNode *a;
    condition *c;
    a = req.attributeHead;
    c = req.conditionHead;
    while (a != NULL) {
        cout<<"Attribute:"<<endl;
        cout<<"attribute name:"<<a->attributeName<<endl;
        cout<<"table name:"<<a->tableName<<endl;
        cout<<"type:"<<a->type<<endl;
        cout<<"isUnique:"<<a->isUnique<<endl;
        cout<<"length:"<<a->length<<endl;
        //cout<<"data:"<<(char *)a->data<<endl;
        //cout<<"data:"<<*((int *)a->data)<<endl;
        //cout<<"data:"<<*((float *)a->data)<<endl;
        cout<<"offset:"<<a->offset<<endl;
        cout<<"index:"<<a->index<<endl;
        a = a->next;
    }
    while (c != NULL) {
        cout<<"Condition:"<<endl;
        cout<<"type:"<<c->type<<endl;
        //cout<<"data:"<<(char *)c->data<<endl;
        //cout<<"data:"<<*((int *)c->data)<<endl;
        //cout<<"data:"<<*((float *)c->data)<<endl;
        cout<<"attribute name:"<<c->attributeName<<endl;
        c = c->next;
    }
    cout<<endl;
    return;
}

void destroy(void){
    if(req.attributeHead != NULL)
        delete req.attributeHead;
    if (req.conditionHead != NULL) {
        delete req.conditionHead;
    }
    req.attributeHead = NULL;
    req.conditionHead = NULL;
    ap = NULL;
    cp = NULL;
    ind = 1;
    return;
}

int printRes(response *r){
    //cout<<"Print res:"<<endl;
    if (r == NULL) {
        return 1;
    }
    cout<<"erro code:"<<r->error<<endl;
    tableRow *t;
    response *rr;
    rr = r;
    while (rr != NULL) {
        t = rr->tableRowHead;
        while (t != NULL) {
            if (t->type == '0') {
                //cout<<"int:";
                cout<<*((int *)(t->data))<<" ";
            }
            else if(t->type == '1'){
                //cout<<"char:";
                cout<<(char *)(t->data)<<" ";
            }
            else{
                //cout<<"float:";
                cout<<*((float *)(t->data))<<" ";
            }
            t = t->next;
        }
        rr = rr->next;
        cout<<endl;
    }
    
    return 0;
    
}

//return 0 when stynax erro, return -1 when quit, return -2 where other siuation
int process(int state, char *word, bufferManager &buf, recordManager &rec, catalogManager &cat){
    if (state == 0) {
        if (strcmp(word, "quit") == 0) {
            return -1;
        }
        else if(strcmp(word, "create") == 0){
            return 100;
        }
        else if(strcmp(word, "drop") == 0){
            return 200;
        }
        else if(strcmp(word, "select") == 0){
            return 300;
        }
        else if(strcmp(word, "insert") == 0){
            return 400;
        }
        else if(strcmp(word, "update") == 0){
            return 500;
        }
        else if(strcmp(word, "delete") == 0){
            return 600;
        }
        else if(strcmp(word, "exefile") == 0){
            return -3;
        }
        else
            return 0;
        
    }
    
    //create
    else if(state == 100){
        //create table
        if (strcmp(word, "table") == 0) {
            return 101;
        }
        //create index
        if (strcmp(word, "index") == 0) {
            return 700;
        }
        else
            return 0;
    }
    
    //create index
    else if(state == 700){
        return 701;
    }
    else if(state == 701){
        if(strcmp(word, "on") == 0)
            return 702;
        else
            return 0;
    }
    else if(state == 702){
        tempIndex.tableName = word;
        return 703;
    }
    else if(state == 703){
        tempIndex.attributeName = word;
        return 704;
    }
    else if(state == 704){
        if (strcmp(word, ";") == 0) {
            int i;
            if(cat.chooseTable(tempIndex.tableName)){
                cout<<"Table name erro\n"<<endl;
                return -2;
            }
            i = cat.createIndex(tempIndex);
            if (i == 1) {
                cout<<"Attribute name erro\n"<<endl;
                return -2;
            }
            if (i == 2) {
                cout<<"Index exist\n"<<endl;
                return -2;
            }
            //test
            cout<<"Create index success\n"<<endl;
            return -2;
        }
        else
            return 0;
    }
    
    //create table
    else if(state == 101){
        tempTable.tableName = word;
        return 102;
    }
    else if(state == 102){
        if (strcmp(word, "(") == 0) {
            return 103;
        }
        else
            return 0;
    }
    else if(state == 103){
        if (strcmp(word, "primarykey") == 0) {
            tempTable.primaryKeyName = word;
            return 103;
        }
        else{
            if (ap == NULL) {
                req.attributeHead = new attributeNode;
                req.attributeHead->attributeName = word;
                req.attributeHead->isUnique = '0';
                ap = req.attributeHead;
                tempTable.primaryKeyName = word;
            }
            else{
                ap->next = new attributeNode;
                ap->next->attributeName = word;
                ap->next->isUnique = '0';
                ap = ap->next;
            }
            tempTable.attributeNum++;
            return 104;
        }
    }
    else if(state == 104){
        if (strcmp(word, "int") == 0) {
            ap->type = '0';
            return 105;
        }
        else if(strcmp(word, "float") == 0){
            ap->type = '2';
            return 105;
        }
        else if(strcmp(word, "char") == 0){
            ap->type = '1';
            return 120;
        }
        else
            return 0;
    }
    else if(state == 120){
        if (strcmp(word, "(") == 0) {
            return 121;
        }
        else
            return 0;
    }
    else if(state == 121){
        string str;
        str = word;
        ap->length = (int)str2num(str);
        return 122;
    }
    else if(state == 122){
        if (strcmp(word, ")") == 0) {
            return 105;
        }
        else
            return 0;
    }
    else if(state == 105){
        if (strcmp(word, ")") == 0) {
            return 106;
        }
        else if(strcmp(word, ",") == 0){
            return 103;
        }
        else
            return 0;
    }
    else if(state == 106){
        if (strcmp(word, ";") == 0) {
            //printReq();
            if(cat.createTable(tempTable, *(req.attributeHead)) == 1){
                cout<<"Table exsist"<<endl;
                return -2;
            }
            cout<<"Create table success"<<endl;
            return -2;
        }
        else
            return 0;
    }
    
    
    
    
    
    
    //drop
    else if(state == 200){
        if (strcmp(word, "table") == 0) {
            return 201;
        }
        else
            return 0;
    }
    else if(state == 201){
        req.tableName = word;
        return 202;
    }
    else if(state == 202){
        if (strcmp(word, ";") == 0) {
            if (cat.dropTable(req.tableName) != 0) {
                cout<<"Table name erro"<<endl;
                return -2;
            }
            cout<<"Drop table success"<<endl;
            return -2;
        }
        else
            return 0;
    }
    
    //select
    else if(state == 300){
        if (strcmp(word, "*") == 0) {
            if (ap != NULL) {
                return 0;
            }
            else
                return 800;
        }
        else{
            if (ap == NULL) {
                req.attributeHead = new attributeNode;
                req.attributeHead->attributeName = word;
                ap = req.attributeHead;
            }
            else{
                ap->next = new attributeNode;
                ap->next->attributeName = word;
                ap = ap->next;
            }
            return 301;
        }
    }
    else if(state == 301){
        if(strcmp(word, "from") == 0)
            return 302;
        else if (strcmp(word, ",") == 0) {
            return 300;
        }
        else return 0;
    }
    else if(state == 302){
        req.tableName = word;
        return 303;
    }
    else if(state == 303){
        if (strcmp(word, "where") == 0) {
            return 310;
        }
        else if(strcmp(word, ";") == 0){
            //printReq();
            response *res;
            res = rec.selectRecord(req, buf);
            cout<<"ok"<<endl;
            if (res->error == TABLE_ERROR) {
                cout<<"Table name erro"<<endl;
                return -2;
            }
            if (res->error == ATTRIBUTE_ERROR) {
                cout<<"Attribute name erro"<<endl;
                return -2;
            }
            if (res->error == OTHER_ERROR) {
                cout<<"Other error"<<endl;
                return -2;
            }
            printRes(res);
            cout<<"select success"<<endl;
            delete res;
            return -2;
        }
        else
            return 0;
    }
    //select where
    else if(state == 310){
        if (cp == NULL) {
            req.conditionHead = new condition;
            req.conditionHead->attributeName = word;
            cp = req.conditionHead;
        }
        else{
            cp->next = new condition;
            cp->next->attributeName = word;
            cp = cp->next;
        }
        return 311;
    }
    else if(state == 311){
        if (strcmp(word, "=") == 0) {
            cp->type = '0';
            return 312;
        }
        else if(strcmp(word, ">") == 0){
            cp->type = '1';
            return 312;
        }
        else if(strcmp(word, "<") == 0){
            cp->type = '2';
            return 312;
        }
        else
            return 0;
    }
    else if(state == 312){
        if (strcmp(word, "=") == 0 && cp->type == '1') {
            cp->type = '3';
            return 312;
        }
        else if(strcmp(word, "=") == 0 && cp->type == '2'){
            cp->type = '4';
            return 312;
        }
        else{
            cp->data = malloc(100);
            strcpy((char *)cp->data, word);
            return 313;
        }
    }
    else if(state == 313){
        if (strcmp(word, "and") == 0) {
            return 310;
        }
        else if(strcmp(word, ";") == 0){
            //printReq();
            response *res;
            res = rec.selectRecord(req, buf);
            if (res->error == TABLE_ERROR) {
                cout<<"Table name erro"<<endl;
                return -2;
            }
            if (res->error == ATTRIBUTE_ERROR) {
                cout<<"Attribute name erro"<<endl;
                return -2;
            }
            if (res->error == OTHER_ERROR) {
                cout<<"Other error"<<endl;
                return -2;
            }
            printRes(res);
            cout<<"select success"<<endl;
            delete res;
            return -2;
        }
        else
            return 0;
    }
    //select all
    else if(state == 800){
        if(strcmp(word, "from") == 0){
            return 801;
        }
        else
            return 0;
    }
    else if(state == 801){
        req.tableName = word;
        return 802;
    }
    else if(state == 802){
        if (strcmp(word, "where") == 0) {
            return 810;
        }
        else if(strcmp(word, ";") == 0){
            if(cat.chooseTable(req.tableName) != 0){
                cout<<"Table name erro"<<endl;
                return 0;
            }
            table &T = cat.getTableInfo();
            int j = 2;
            req.attributeHead = new attributeNode;
            ap = req.attributeHead;
            if (T.attributeNum != 0) {
                req.attributeHead->index = 1;
            }
            while (j <= T.attributeNum) {
                ap->next = new attributeNode;
                ap->next->index = j;
                j++;
                ap = ap->next;
            }
            //printReq();
            response *res = NULL;
            res = rec.selectRecord(req, buf);
            if (res != NULL) {
                if (res->error == TABLE_ERROR) {
                    cout<<"Table name erro"<<endl;
                    return -2;
                }
                if (res->error == ATTRIBUTE_ERROR) {
                    cout<<"Attribute name erro"<<endl;
                    return -2;
                }
                if (res->error == OTHER_ERROR) {
                    cout<<"Other error"<<endl;
                    return -2;
                }
                printRes(res);
                delete res;
            }
            cout<<"select success"<<endl;
            return -2;
        }
        else
            return 0;
    }
    //select all where
    else if(state == 810){
        if (cp == NULL) {
            req.conditionHead = new condition;
            req.conditionHead->attributeName = word;
            cp = req.conditionHead;
        }
        else{
            cp->next = new condition;
            cp->next->attributeName = word;
            cp = cp->next;
        }
        return 811;
    }
    else if(state == 811){
        if (strcmp(word, "=") == 0) {
            cp->type = '0';
            return 812;
        }
        else if(strcmp(word, ">") == 0){
            cp->type = '1';
            return 812;
        }
        else if(strcmp(word, "<") == 0){
            cp->type = '2';
            return 812;
        }
        else
            return 0;
    }
    else if(state == 812){
        if (strcmp(word, "=") == 0 && cp->type == '1') {
            cp->type = '3';
            return 812;
        }
        else if(strcmp(word, "=") == 0 && cp->type == '2'){
            cp->type = '4';
            return 812;
        }
        else{
            cp->data = malloc(100);
            strcpy((char *)cp->data, word);
            return 813;
        }
    }
    else if(state == 813){
        if (strcmp(word, "and") == 0) {
            return 810;
        }
        else if(strcmp(word, ";") == 0){
            if(cat.chooseTable(req.tableName) != 0){
                cout<<"Table name erro"<<endl;
                return 0;
            }
            table &T = cat.getTableInfo();
            int j = 2;
            req.attributeHead = new attributeNode;
            ap = req.attributeHead;
            if (T.attributeNum != 0) {
                req.attributeHead->index = 1;
            }
            while (j <= T.attributeNum) {
                ap->next = new attributeNode;
                ap->next->index = j;
                j++;
                ap = ap->next;
            }
            //printReq();
            response *res;
            res = rec.selectRecord(req, buf);
            if (res->error == TABLE_ERROR) {
                cout<<"Table name erro"<<endl;
                return -2;
            }
            if (res->error == ATTRIBUTE_ERROR) {
                cout<<"Attribute name erro"<<endl;
                return -2;
            }
            if (res->error == OTHER_ERROR) {
                cout<<"Other error"<<endl;
                return -2;
            }
            printRes(res);
            cout<<"select success"<<endl;
            delete res;
            return -2;
        }
        else
            return 0;
    }

    
    
    
    
    //insert
    else if(state == 400){
        if (strcmp(word, "into") == 0) {
            return 401;
        }
        else
            return 0;
    }
    else if(state == 401){
        req.tableName = word;
        return 402;
    }
    else if(state == 402){
        if (strcmp(word, "values") == 0) {
            return 403;
        }
        else
            return 0;
    }
    else if(state == 403){
        if (strcmp(word, "(") == 0) {
            return 404;
        }
        else
            return 0;
    }
    else if(state == 404){
        if (ap == NULL) {
            req.attributeHead = new attributeNode;
            req.attributeHead->index = ind;
            req.attributeHead->data = malloc(100);
            strcpy((char *)req.attributeHead->data, word);
            ap = req.attributeHead;
        }
        else{
            ap->next = new attributeNode;
            ap->next->index = ind;
            ap->next->data = malloc(100);
            strcpy((char *)(ap->next->data), word);
            ap = ap->next;
        }
        ind++;
        return 405;
    }
    else if(state == 405){
        if (strcmp(word, ")") == 0) {
            return 406;
        }
        else if(strcmp(word, ",") == 0){
            return 404;
        }
        else
            return 0;
    }
    else if(state == 406){
        if(strcmp(word, ";") == 0){
            //printReq();
            response *res;
            res = rec.insertRecord(req, buf);
            if (res->error == TABLE_ERROR) {
                cout<<"Table name erro"<<endl;
                return -2;
            }
            if (res->error == ATTRIBUTE_ERROR) {
                cout<<"Attribute name erro"<<endl;
                return -2;
            }
            if (res->error == OTHER_ERROR) {
                cout<<"Other error"<<endl;
                return -2;
            }
            printRes(res);
            cout<<"Insert success"<<endl;
            delete res;
            return -2;
        }
        else
            return 0;
    }
     
    
    //update
    else if(state == 500){
        req.tableName = word;
        return 501;
    }
    else if(state == 501){
        if (strcmp(word, "set") == 0) {
            return 502;
        }
        else
            return 0;
    }
    else if(state == 502){
        if (ap == NULL) {
            req.attributeHead = new attributeNode;
            req.attributeHead->attributeName = word;
            ap = req.attributeHead;
        }
        else{
            ap->next = new attributeNode;
            ap->next->attributeName = word;
            ap = ap->next;
        }
        return 503;
    }
    else if(state == 503){
        if (strcmp(word, "=") == 0) {
            return 504;
        }
        else
            return 0;
    }
    else if(state == 504){
        ap->data = (char *)malloc(100);
        strcpy((char *)ap->data, word);
        return 505;
    }
    else if(state == 505){
        if(strcmp(word, "where") == 0)
            return 510;
        else if(strcmp(word, ";") == 0){
            response *res;
            //printReq();
            res = rec.updateRecord(req, buf);
            if (res->error == TABLE_ERROR) {
                cout<<"Table name erro"<<endl;
                return -2;
            }
            if (res->error == ATTRIBUTE_ERROR) {
                cout<<"Attribute name erro"<<endl;
                return -2;
            }
            if (res->error == OTHER_ERROR) {
                cout<<"Other error"<<endl;
                return -2;
            }
            printRes(res);
            cout<<"Update success"<<endl;
            delete res;
            return -2;
        }
        else if(strcmp(word, ",") == 0)
            return 502;
        else
            return 0;
    }
    //update where
    else if(state == 510){
        if (cp == NULL) {
            req.conditionHead = new condition;
            req.conditionHead->attributeName = word;
            cp = req.conditionHead;
        }
        else{
            cp->next = new condition;
            cp->next->attributeName = word;
            cp = cp->next;
        }
        return 511;
    }
    else if(state == 511){
        if (strcmp(word, "=") == 0) {
            cp->type = '0';
            return 512;
        }
        else if(strcmp(word, ">") == 0){
            cp->type = '1';
            return 512;
        }
        else if(strcmp(word, "<") == 0){
            cp->type = '2';
            return 512;
        }
        else
            return 0;
    }
    else if(state == 512){
        if (strcmp(word, "=") == 0 && cp->type == '1') {
            cp->type = '3';
            return 512;
        }
        else if(strcmp(word, "=") == 0 && cp->type == '2'){
            cp->type = '4';
            return 512;
        }
        else{
            cp->data = malloc(100);
            strcpy((char *)cp->data, word);
            return 513;
        }
    }
    else if(state == 513){
        if (strcmp(word, "and") == 0) {
            return 510;
        }
        else if(strcmp(word, ";") == 0){
            response *res;
            //printReq();
            res = rec.updateRecord(req, buf);
            if (res->error == TABLE_ERROR) {
                cout<<"Table name erro"<<endl;
                return -2;
            }
            if (res->error == ATTRIBUTE_ERROR) {
                cout<<"Attribute name erro"<<endl;
                return -2;
            }
            if (res->error == OTHER_ERROR) {
                cout<<"Other error"<<endl;
                return -2;
            }
            printRes(res);
            cout<<"Update success"<<endl;
            delete res;
            return -2;
        }
        else
            return 0;
    }
    
    
    //delete
    else if(state == 600){
        if (strcmp(word, "from") == 0) {
            return 601;
        }
        else
            return 0;
    }
    else if(state == 601){
        req.tableName = word;
        return 602;
    }
    else if(state == 602){
        if (strcmp(word, "where") == 0) {
            return 610;
        }
        else if(strcmp(word, ";") == 0){
            //printReq();
            response *res;
            res = rec.deleteRecord(req, buf);
            if (res->error == TABLE_ERROR) {
                cout<<"Table name erro"<<endl;
                return -2;
            }
            if (res->error == ATTRIBUTE_ERROR) {
                cout<<"Attribute name erro"<<endl;
                return -2;
            }
            if (res->error == OTHER_ERROR) {
                cout<<"Other error"<<endl;
                return -2;
            }
            cout<<"delete success"<<endl;
            delete res;
            return -2;
        }
        else
            return 0;
    }
    //delete where
    else if(state == 610){
        if (cp == NULL) {
            req.conditionHead = new condition;
            req.conditionHead->attributeName = word;
            cp = req.conditionHead;
        }
        else{
            cp->next = new condition;
            cp->next->attributeName = word;
            cp = cp->next;
        }
        return 611;
    }
    else if(state == 611){
        if (strcmp(word, "=") == 0) {
            cp->type = '0';
            return 612;
        }
        else if(strcmp(word, ">") == 0){
            cp->type = '1';
            return 612;
        }
        else if(strcmp(word, "<") == 0){
            cp->type = '2';
            return 612;
        }
        else
            return 0;
    }
    else if(state == 612){
        if (strcmp(word, "=") == 0 && cp->type == '1') {
            cp->type = '3';
            return 612;
        }
        else if(strcmp(word, "=") == 0 && cp->type == '2'){
            cp->type = '4';
            return 612;
        }
        else{
            cp->data = malloc(100);
            strcpy((char *)cp->data, word);
            return 613;
        }
    }
    else if(state == 613){
        if (strcmp(word, "and") == 0) {
            return 610;
        }
        else if(strcmp(word, ";") == 0){
            //printReq();
            response *res;
            res = rec.deleteRecord(req, buf);
            if (res->error == TABLE_ERROR) {
                cout<<"Table name erro"<<endl;
                return -2;
            }
            if (res->error == ATTRIBUTE_ERROR) {
                cout<<"Attribute name erro"<<endl;
                return -2;
            }
            if (res->error == OTHER_ERROR) {
                cout<<"Other error"<<endl;
                return -2;
            }
            cout<<"delete success"<<endl;
            delete res;
            return -2;
        }
        else
            return 0;
    }
    
    return 0;
    
    
    
    
    

    
    
    
    
    
    
}