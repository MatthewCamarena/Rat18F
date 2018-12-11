//
//  SymbolTable.hpp
//  Assignment 3 CPSC 323
//
//  Created by Alex Rasho on 12/10/18.
//  Copyright © 2018 Alex Rasho. All rights reserved.
//

#ifndef SymbolTable_hpp
#define SymbolTable_hpp

#pragma once

#include <stdio.h>
#include <string>
#include <cstring>
#include <stdexcept>
#include <vector>

class SymbolTable {
    
public:
    string identifier = "";
    int memlocation = 0;
    string type = "";
};

#endif /* SymbolTable_hpp */
