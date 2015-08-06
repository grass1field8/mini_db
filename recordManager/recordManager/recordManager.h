//
//  recordManager.h
//  recordManager
//
//  Created by 黄佳睿 on 14-11-5.
//  Copyright (c) 2014年 黄佳睿. All rights reserved.
//

#ifndef __recordManager__recordManager__
#define __recordManager__recordManager__

#include <stdio.h>
#include <sstream>
#include <string.h>
#include "table&index.h"
#include "request&response.h"
#include "bufferManager.h"


string num2str(double i);
string num2str(int i);
double str2num(string s);
class recordManager{
public:
    response *insertRecord(request &req, bufferManager &buf);
    response *deleteRecord(request &req, bufferManager &buf);
    response *selectRecord(request &req, bufferManager &buf);
    response *updateRecord(request &req, bufferManager &buf);
};










#endif /* defined(__recordManager__recordManager__) */
