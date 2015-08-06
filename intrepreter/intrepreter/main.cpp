//
//  main.cpp
//  intrepreter
//
//  Created by 黄佳睿 on 14-11-11.
//  Copyright (c) 2014年 黄佳睿. All rights reserved.
//

#include <iostream>
#include "interpreter.h"
using namespace std ;

int main(int argc, const char * argv[]) {
    bufferManager buf;
    recordManager rec;
    catalogManager cat;
    char buffer[10000];
    int cur;
    int end;
    int i = 0;
    int flag = 0;
    while (i != 1){
        cout<<"sql> ";
        i = phraser(buf, rec, cat, flag, buffer, cur, end);
    }
    return 0;
}

