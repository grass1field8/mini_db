//
//  bufferManager.h
//  bufferManager
//
//  Created by 黄佳睿 on 14-10-28.
//  Copyright (c) 2014年 黄佳睿. All rights reserved.
//

#ifndef __bufferManager__bufferManager__
#define __bufferManager__bufferManager__
const int MAX_BUFFER_SIZE = 20;
const int BLOCKSIZE = 1024*4;
#include <iostream>
using namespace std;

class bufferNode{
public:
    int isLock;
    int isDirty;
    int offset;
    string fileName;
    void *buffer;
    struct bufferNode *next, *previous;
    bufferNode(){
        buffer = (void*)malloc(BLOCKSIZE);
        isLock = 0;
        isDirty = 0;
        next = NULL;
        previous = NULL;
    }
    ~bufferNode(){
        free(buffer);
    }
};

class bufferManager{
private:
    bufferNode *head, *rear;
    int size;
public:
    bufferManager(){
        size = 0;
        head = NULL;
        rear = NULL;
    }
    void print(void);//for test
    void *readBlock(string fileName, int offset);
    int writeBlock(string fileName, int offset, void* sourceBuffer);
    //return 0 when write into buffer, return 1 when write driectly into disk
    int pin(string fileName, int offset);
    int unpin(string fileName, int offset);
    //success return 0, return 1 when block can't be found
    ~bufferManager();
};



#endif /* defined(__bufferManager__bufferManager__) */
