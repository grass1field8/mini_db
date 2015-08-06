//
//  catalogManager.cpp
//  catalogManager
//
//  Created by 黄佳睿 on 14-10-30.
//  Copyright (c) 2014年 黄佳睿. All rights reserved.
//


#include "catalogManager.h"
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int catalogManager::chooseTable(string tableName){
    int fd, i;
    int ibuffer[7];
    char buffer[140];
    fd = open("/Users/hjr/code/test/relation", O_RDWR);
    lseek(fd, 0, SEEK_SET);
    i = 1;
    while(i != 0){
        i = (int)read(fd, buffer, 140);
        if (i == 0) {
            break;
        }
        if(strcmp(buffer, tableName.c_str()) == 0 && *(buffer+100+7*sizeof(int)) == '0'){
            break;
        }
    }
    if(i == 0)
        return 1;
    choosedTable.tableName = buffer;
    choosedTable.primaryKeyName = buffer+50;
    memcpy(ibuffer, buffer+100, 7*sizeof(int));
    choosedTable.attributeNum = ibuffer[0];
    choosedTable.recordSize = ibuffer[1];
    choosedTable.location = ibuffer[2];
    choosedTable.aLocation = ibuffer[3];
    choosedTable.insertH = ibuffer[4];
    choosedTable.deleteH = ibuffer[5];
    choosedTable.recordNum = ibuffer[6];
    read(fd, buffer, sizeof(char));
    if (buffer[0] == '1') {
        return 1;
    }
    close(fd);
    return 0;
}


int catalogManager::createTable(table &tempTable, attributeNode &head){
    char buffer[140];
    char buffer1[140];
    int fd;
    int i;
    int j[1];
    //judge if is exist
    fd = open("/Users/hjr/code/test/relation", O_RDWR);
    lseek(fd, 0, SEEK_SET);
    i = 1;
    while(i != 0){
        i = (int)read(fd, buffer, 140);
        if (i == 0) {
            break;
        }
        if(strcmp(buffer, tempTable.tableName.c_str()) == 0 && *(buffer+100+7*sizeof(int)) == '0'){
            return 1;
        }
    }
    // 140
    //(tableNmae, primaryKeyName,attributeNum, recordSize, Location, alocation, insertHead, deleteHead, recordNum,  isdelete)
    //(50       , 50            ,int         ,int        , int     ,int       , int       ,int        , int      , char  )

    strcpy(buffer, (tempTable.tableName).c_str());
    strcpy(buffer+50, (tempTable.primaryKeyName).c_str());
    memcpy(buffer+100, &(tempTable.attributeNum), sizeof(int));
    memcpy(buffer+100+sizeof(int), &(tempTable.recordSize), sizeof(int));
    
    fd = open("/Users/hjr/code/test/info", O_RDWR);
    lseek(fd, 0, SEEK_SET);
    read(fd, j, sizeof(int));
    memcpy(buffer+100+2*sizeof(int), j, sizeof(int));
    j[0]++;
    lseek(fd, 0, SEEK_SET);
    write(fd, j, sizeof(int));
    close(fd);
    
    fd = open("/Users/hjr/code/test/attribute", O_RDWR);
    i = (int)lseek(fd, 0, SEEK_END);
    memcpy(buffer+100+3*sizeof(int), &i, sizeof(int));
    close(fd);
    i = -1;
    memcpy(buffer+100+4*sizeof(int), &i, sizeof(int));
    memcpy(buffer+100+5*sizeof(int), &i, sizeof(int));
    i = 0;
    memcpy(buffer+100+6*sizeof(int), &i, sizeof(int));
    *(buffer+100+7*sizeof(int)) = '0';
    
    
    
    // 120
    //(tableName, attributeName, type, isUnque, length, offset)
    //(50       , 50           , char, char   , int   , int   )
    attributeNode *temp;
    int length;
    int offset = 0;
    temp = &head;
    fd = open("/Users/hjr/code/test/attribute", O_RDWR);
    lseek(fd, 0, SEEK_END);
    int num = 0;
    while(temp != NULL){
        strcpy(buffer1, (tempTable.tableName).c_str());
        strcpy(buffer1+50, (temp->attributeName).c_str());
        *(buffer1+100) = temp->type;
        *(buffer1+101) = temp->isUnique;
        if(temp->type == 0)
            length = sizeof(int);
        else if(temp->type == 1)
            length = sizeof(char)*(temp->length) + 1;
        else
            length = sizeof(float);
        
        memcpy(buffer1+102, &length, sizeof(int));
        memcpy(buffer1+102+sizeof(int), &offset, sizeof(int));
        write(fd, buffer1, 120);
        offset += length;
        temp = temp->next;
        num++;
    }
    close(fd);
    offset = offset + 1 +sizeof(int);
    memcpy(buffer+100, &num, sizeof(int));
    memcpy(buffer+100+sizeof(int), &offset, sizeof(int));
    fd = open("/Users/hjr/code/test/relation", O_RDWR);
    lseek(fd, 0, SEEK_END);
    write(fd, buffer, 140);
    close(fd);
    return 0;
}


table &catalogManager::getTableInfo(void){
    return choosedTable;
}

int catalogManager::getAttributeByName(string attributeName, attributeNode &tempAttribute){
    int fd, i;
    int ibuffer[2];
    char buffer[120];
    fd = open("/Users/hjr/code/test/attribute", O_RDWR);
    lseek(fd, choosedTable.aLocation, SEEK_SET);
    i = 1;
    while(i <= choosedTable.attributeNum){
        read(fd, buffer, 100);
        if(strcmp(buffer+50, attributeName.c_str()) == 0){
            tempAttribute.tableName = buffer;
            tempAttribute.attributeName = buffer+50;
            read(fd, buffer, 2);
            tempAttribute.type = buffer[0];
            tempAttribute.isUnique = buffer[1];
            read(fd, ibuffer, 2*sizeof(int));
            tempAttribute.length = ibuffer[0];
            tempAttribute.offset = ibuffer[1];
            return 0;
        }
        lseek(fd, 20, SEEK_CUR);
        i++;
    }
    return 1;
}

int catalogManager::getAttributeByIndex(int index, attributeNode &tempAttribute){
    int fd;
    int ibuffer[2];
    char buffer[120];
    if(index > choosedTable.attributeNum || index < 0)
        return 1;
    fd = open("/Users/hjr/code/test/attribute", O_RDWR);
    lseek(fd, choosedTable.aLocation+(index-1)*120, SEEK_SET);
    read(fd, buffer, 100);
    tempAttribute.tableName = buffer;
    tempAttribute.attributeName = buffer+50;
    read(fd, buffer, 2);
    tempAttribute.type = buffer[0];
    tempAttribute.isUnique = buffer[1];
    read(fd, ibuffer, 2*sizeof(int));
    tempAttribute.length = ibuffer[0];
    tempAttribute.offset = ibuffer[1];
    return 0;

}

int catalogManager::updateTable(void){
    int fd, i;
    int ibuffer[3];
    char buffer[140];
    fd = open("/Users/hjr/code/test/relation", O_RDWR);
    lseek(fd, 0, SEEK_SET);
    i = 1;
    while(i != 0){
        i = (int)read(fd, buffer, 140);
        if (i == 0) {
            break;
        }
        if(strcmp(buffer, choosedTable.tableName.c_str()) == 0){
            break;
        }
    }
    if(i == 0)
        return 1;
//    cout<<choosedTable.insertH<<choosedTable.deleteH<<choosedTable.recordNum<<endl;
    ibuffer[0] = choosedTable.insertH;
    ibuffer[1] = choosedTable.deleteH;
    ibuffer[2] = choosedTable.recordNum;
    lseek(fd, -40+4*sizeof(int), SEEK_CUR);
    write(fd, ibuffer, 3*sizeof(int));
    close(fd);
    return 0;
}

int catalogManager::dropTable(string tableName){
    int fd, i;
    char buffer[140];
    fd = open("/Users/hjr/code/test/relation", O_RDWR);
    lseek(fd, 0, SEEK_SET);
    i = 1;
    while(i != 0){
        i = (int)read(fd, buffer, 140);
        if (i == 0) {
            break;
        }
        if(strcmp(buffer, tableName.c_str()) == 0 && *(buffer+100+7*sizeof(int)) == '0'){
            lseek(fd, -140+100+7*sizeof(int), SEEK_CUR);
            char j = '1';
            write(fd, &j, sizeof(char));
            close(fd);
            return 0;
        }
    }
    close(fd);
    return 1;
}


int catalogManager::createIndex(indexx &tempIndex){
    //120
    //(tableName, attribute, head, left, isDelete)
    //(50       , 50       , int , int , char)
    int fd, i;
    char buffer[120];
    
    fd = open("/Users/hjr/code/test/attribute", O_RDWR);
    lseek(fd, choosedTable.aLocation, SEEK_SET);
    i = 1;
    while(i <= choosedTable.attributeNum){
        read(fd, buffer, 100);
        if(strcmp(buffer+50, tempIndex.attributeName.c_str()) == 0){
            break;
        }
        lseek(fd, 20, SEEK_CUR);
        i++;
    }
    if (i > choosedTable.attributeNum) {
        return 1;
    }
    
    i = 1;
    fd = open("/Users/hjr/code/test/index", O_RDWR);
    while (i != 0) {
        i = (int)read(fd, buffer, 120);
        if (i == 0) {
            break;
        }
        if (strcmp(buffer, tempIndex.tableName.c_str()) == 0 || strcmp(buffer+50, tempIndex.attributeName.c_str()) == 0) {
            if (*(buffer+100+2*sizeof(int)) == '0') {
                return 2;
            }
        }
    }

    i = -1;
    lseek(fd, 0, SEEK_END);
    memcpy(buffer, tempIndex.tableName.c_str(), 50);
    memcpy(buffer+50, tempIndex.attributeName.c_str(), 50);
    memcpy(buffer+100, &i, sizeof(int));
    memcpy(buffer+100+sizeof(int), &i, sizeof(int));
    *(buffer+100+2*sizeof(int)) = '0';
    write(fd, buffer, 120);
    close(fd);
    return 0;
}

int catalogManager::dropIndex(string tableName, string attributeName){
    int fd, i;
    char buffer[120];
    i = 1;
    fd = open("/Users/hjr/code/test/index", O_RDWR);
    while (i != 0) {
        i = (int)read(fd, buffer, 120);
        if (i == 0) {
            break;
        }
        if (strcmp(buffer, tableName.c_str()) == 0 || strcmp(buffer+50, attributeName.c_str()) == 0) {
            if (*(buffer+100+2*sizeof(int)) == '0') {
                lseek(fd, -20+2*sizeof(int), SEEK_CUR);
                char j = '1';
                write(fd, &j, sizeof(char));
                close(fd);
                return 0;
            }
        }
    }
    close(fd);
    return 1;
}

int catalogManager::getIndex(string tableName, string attributeName, indexx &tempIndex){
    int fd, i;
    int ibuffer[2];
    char buffer[120];
    i = 1;
    fd = open("/Users/hjr/code/test/index", O_RDWR);
    while (i != 0) {
        i = (int)read(fd, buffer, 120);
        if (i == 0) {
            break;
        }
        if (strcmp(buffer, tableName.c_str()) == 0 || strcmp(buffer+50, attributeName.c_str()) == 0) {
            if (*(buffer+100+2*sizeof(int)) == '0'){
                tempIndex.tableName = buffer;
                tempIndex.attributeName = buffer + 50;
                memcpy(ibuffer, buffer+100, 2*sizeof(int));
                tempIndex.head = ibuffer[0];
                tempIndex.left = ibuffer[1];
                return 0;
            }
        }
    }
    return 1;
}
    





