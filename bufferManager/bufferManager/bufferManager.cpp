//
//  bufferManager.cpp
//  bufferManager
//
//  Created by 黄佳睿 on 14-10-28.
//  Copyright (c) 2014年 黄佳睿. All rights reserved.
//



#include "bufferManager.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void *bufferManager::readBlock(string fileName, int offset){
    int fd;
    if (size == 0) {         //zero node
        size++;
        head = new bufferNode;
        head->offset = offset;
        head->fileName = fileName;
        
        fd = open(fileName.c_str(), O_RDWR);
        lseek(fd, BLOCKSIZE * offset, SEEK_SET);
        read(fd, head->buffer, BLOCKSIZE);
        close(fd);
        rear = head;
        return head->buffer;
    }
    bufferNode *temp;
    temp = head;
    while (temp != NULL) {          //node exsist
        if (temp->fileName == fileName && temp->offset == offset) {
            if (temp != head) {
                if (temp->previous != NULL) {
                temp->previous->next = temp->next;
                }
                if (temp->next != NULL) {
                temp->next->previous = temp->previous;
                }
                if (temp == rear && temp != head) {
                rear = temp->previous;
                }
            
                temp->previous = NULL;
                temp->next = head;
                head->previous = temp;
                head = temp;
            }
            return head->buffer;
        }
        temp = temp->next;
    }
    if (size < MAX_BUFFER_SIZE){
        size++;                  //not exeist, and there is space
        temp = head;
        head = new bufferNode;
        head->offset = offset;
        head->fileName = fileName;
        head->next = temp;
        temp->previous = head;
        fd = open(fileName.c_str(), O_RDWR);
        lseek(fd, BLOCKSIZE * offset, SEEK_SET);
        read(fd, head->buffer, BLOCKSIZE);
        close(fd);
        return head->buffer;
    }
    else {                        //node full, using LRU
        temp = rear;
        while (temp->isLock == 1 && temp->previous != NULL) {
            temp = temp->previous;
        }
        if(temp == head && temp->isLock == 1){
            cout<<"All locked"<<endl;
            return NULL;
        }
        
        if (temp->isDirty == 1) {
            fd = open((temp->fileName).c_str(), O_RDWR);
            lseek(fd, BLOCKSIZE * temp->offset, SEEK_SET);
            write(fd, temp->buffer, BLOCKSIZE);
            close(fd);
            temp->isDirty = 0;
        }
        if (temp != head) {
            if (temp->previous != NULL) {
                temp->previous->next = temp->next;
            }
            if (temp->next != NULL) {
                temp->next->previous = temp->previous;
            }
            if (temp == rear && temp != head) {
                rear = temp->previous;
            }
            temp->previous = NULL;
            temp->next = head;
            head->previous = temp;
        }
        temp->offset = offset;
        temp->fileName = fileName;
        head = temp;
        fd = open(fileName.c_str(), O_RDWR);
        lseek(fd, BLOCKSIZE * offset, SEEK_SET);
        read(fd, head->buffer, BLOCKSIZE);
        close(fd);
        return head->buffer;
    }
}

int bufferManager::writeBlock(string fileName, int offset, void* sourceBuffer){
    bufferNode *temp;
    temp = head;
    while (temp != NULL) {
        if (temp->fileName == fileName && temp->offset == offset) {
            memcpy(temp->buffer, sourceBuffer, BLOCKSIZE);
            temp->isDirty = 1;
            return 0;
        }
        temp = temp->next;
    }
    int fd;
    fd = open(fileName.c_str(), O_RDWR);
    lseek(fd, BLOCKSIZE * offset, SEEK_SET);
    write(fd, sourceBuffer, BLOCKSIZE);
    close(fd);
    return 0;
}

int bufferManager::pin(string fileName,int offset){
    bufferNode *temp;
    temp = head;
    while (temp != NULL) {
        if (temp->fileName == fileName && temp->offset == offset) {
            temp->isLock = 1;
            return 0;
        }
        temp = temp->next;
    }
    return 1;
}

int bufferManager::unpin(string fileName, int offset){
    bufferNode *temp;
    temp = head;
    while (temp != NULL) {
        if (temp->fileName == fileName && temp->offset == offset) {
            temp->isLock = 0;
            if (temp != head) {
                if (temp->previous != NULL) {
                    temp->previous->next = temp->next;
                }
                if (temp->next != NULL) {
                    temp->next->previous = temp->previous;
                }
                if (temp == rear && temp != head) {
                    rear = temp->previous;
                }
                temp->previous = NULL;
                temp->next = head;
                head->previous = temp;
                head = temp;
                
            }
            return 0;
        }
        temp = temp->next;
    }
    return 1;
}

void bufferManager::print(void){
    bufferNode *temp;
    temp = head;
    if(size != 0){
        cout<<"print begin:"<<endl;
        cout<<"head is: "<<head<<"::"<<head->fileName<<"::"<<head->offset<<"::"<<head->isLock<<"::"<<head->previous<<"::"<<head->next<<endl;
        cout<<"rear is: "<<rear<<"::"<<rear->fileName<<"::"<<rear->offset<<"::"<<rear->isLock<<"::"<<rear->previous<<"::"<<rear->next<<endl;
        cout<<"size:"<<size<<endl;
    }
    while (temp != NULL) {
        cout<<temp<<"::"<<temp->fileName<<"::"<<temp->offset<<"::"<<temp->isLock<<"::"<<temp->previous<<"::"<<temp->next<<endl;
        temp = temp->next;
    }
    cout<<"print end"<<endl;
    return;
}

bufferManager::~bufferManager(){
    bufferNode *temp;
    int fd;
    while (head != NULL) {
        if (head->isDirty) {
            fd = open((head->fileName).c_str(), O_RDWR);
            lseek(fd, BLOCKSIZE * head->offset, SEEK_SET);
            write(fd, head->buffer, BLOCKSIZE);
            close(fd);
        }
        temp = head;
        head = head->next;
        delete temp;
    }
}

