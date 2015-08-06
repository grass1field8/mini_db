//
//  main.cpp
//  recordManager
//
//  Created by 黄佳睿 on 14-11-5.
//  Copyright (c) 2014年 黄佳睿. All rights reserved.
//

#include <iostream>
#include "bufferManager.h"
#include "catalogManager.h"
#include "recordManager.h"
int printRes(response *r);
int main(int argc, const char * argv[]) {
    int res;
    int i;
    response *r;
    bufferManager buf;
    condition *c1, *c2;
    c1 = new condition;
    c2 = new condition;
    
    //c1
    //id >= 4
    c1->type = '2';
    c1->data = malloc(100);
    strcpy((char *)c1->data, "3.1");
    c1->attributeName = "float";
    c1->next = NULL;
    
    
    //c2
    //name = "bb"
    c2->type = '0';
    c2->data = malloc(3*sizeof(char));
    memcpy(c2->data, "bb", 3*sizeof(char));
    c2->attributeName = "char";
    c2->next = NULL;
    
    attributeNode *a1, *a2, *a3;
    a1 = new attributeNode;
    a2 = new attributeNode;
    a3 = new attributeNode;
    a1->index = 1;
    a1->data = malloc(100);
    strcpy((char *)a1->data, "11");
    a1->next =a2;
    
    a2->index = 2;
    a2->data = malloc(100);
    strcpy((char *)a2->data, "yiku!");
    a2->next = a3;
    
    a3->index = 3;
    a3->data = malloc(100);
    strcpy((char *)a3->data, "11.11");
    a3->next = NULL;
    
    request req;
    req.tableName = "testtable";
    req.attributeHead = a1;
    req.conditionHead = c1;
    
    recordManager rec;
    //r = rec.insertRecord(req, buf);
    //cout<<"insert record return:"<<r->error<<endl;
    //r = rec.deleteRecord(req, buf);
    //printRes(r);
    //rec.updateRecord(req, buf);
    req.conditionHead = NULL;
    r = rec.selectRecord(req, buf);
    printRes(r);
    return 0;
    
    
}

int printRes(response *r){
    if (r == NULL) {
        return 1;
    }
    cout<<"erro code:"<<r->error<<endl;
    tableRow *t;
    response *rr;
    rr = r;
    while (rr != NULL) {
        cout<<"haha"<<endl;
        t = rr->tableRowHead;
        while (t != NULL) {
            cout<<"hehe"<<endl;
            if (t->type == '0') {
                cout<<"int:";
                cout<<*((int *)(t->data))<<" ";
            }
            else if(t->type == '1'){
                cout<<"char:";
                cout<<(char *)(t->data)<<" ";
            }
            else{
                cout<<"float:";
                cout<<*((float *)(t->data))<<" ";
            }
            cout<<endl;
            t = t->next;
        }
        rr = rr->next;
    }
    return 0;

}
