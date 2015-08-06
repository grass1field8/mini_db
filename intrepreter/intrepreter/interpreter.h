//
//  interpreter.h
//  intrepreter
//
//  Created by 黄佳睿 on 14-11-11.
//  Copyright (c) 2014年 黄佳睿. All rights reserved.
//

#ifndef __intrepreter__interpreter__
#define __intrepreter__interpreter__

#include <stdio.h>
#include <sstream>
#include <fstream>
#include <string.h>
#include <iostream>
#include "bufferManager.h"
#include "recordManager.h"
#include "catalogManager.h"
using namespace std;

string num2str(double i);
string num2str(int i);
double str2num(string s);
int phraser(bufferManager &buf, recordManager &rec, catalogManager &cat, int &flag, char *buffer, int &cur, int &end);

#endif /* defined(__intrepreter__interpreter__) */
