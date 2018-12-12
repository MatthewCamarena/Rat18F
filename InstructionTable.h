//
//  InstructionTable.hpp
//  Assignment 3 CPSC 323
//
//  Created by Alex Rasho on 12/10/18.
//  Copyright © 2018 Alex Rasho. All rights reserved.
//

#ifndef InstructionTable_hpp
#define InstructionTable_hpp

#pragma once

#include <stdio.h>
#include <string>
#include <cstring>
#include <stdexcept>
#include <vector>

class InstructionTable {
    
public:
    int address = 0;
    int operand = 0;
    string op = "";
};
#endif /* InstructionTable_hpp */
