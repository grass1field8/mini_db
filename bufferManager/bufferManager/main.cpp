//
//  main.cpp
//  bufferManager
//
//  Created by 黄佳睿 on 14-10-28.
//  Copyright (c) 2014年 黄佳睿. All rights reserved.
//

#include <iostream>
#include "bufferManager.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(int argc, const char * argv[]) {
    bufferManager manager;
    char *buffer;
    
//    manager.writeBlock("/Users/hjr/code/test/testdb", 0, buffer);
//    for (int i = 0; i < BLOCKSIZE; i++) {
//        *(buffer+i) = 'b';
//    }
//    manager.writeBlock("/Users/hjr/code/test/testdb", 1, buffer);
//    for (int i = 0; i < BLOCKSIZE; i++) {
//        *(buffer+i) = 'c';
//    }
    
    buffer = (char *)manager.readBlock("/Users/hjr/code/test/testdb", 0);
    buffer = (char *)manager.readBlock("/Users/hjr/code/test/testdb", 1);
    buffer = (char *)malloc(BLOCKSIZE);
    for (int i = 0; i < BLOCKSIZE; i++) {
        *(buffer+i) = 'd';
    }
    manager.writeBlock("/Users/hjr/code/test/testdb", 0, buffer);
    //buffer = (char *)manager.readBlock("/Users/hjr/code/test/testdb", 2);
//    manager.pin("/Users/hjr/code/test/testdb", 0);
//    manager.unpin("/Users/hjr/code/test/testdb", 1);
//    buffer = (char *)manager.readBlock("/Users/hjr/code/test/testdb", 1);
//    buffer = (char *)manager.readBlock("/Users/hjr/code/test/testdb", 3);
    int i;
    cin>>i;
    manager.print();
    cout<<"content"<<endl;
    cout<<(char *)buffer<<endl;
    return 0;
}
