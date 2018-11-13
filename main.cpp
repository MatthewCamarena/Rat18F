//
//  main.cpp
//  Compilermain.cpp
//
//  Created by Alex Rasho on 11/11/18.
//  Copyright © 2018 Alex Rasho. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
#include <string>
#include <cassert>
#include "LexiconReader.h"
#include "SyntaxAnalyzer.h"

using namespace std;

int main(int argc, const char * argv[]) {
    string fileName = "testLex.txt";
    //LexiconReader LR;
    //LR.buildLexiconRecordfromDatafile(fileName);
    SyntaxAnalyzer SA;
    SA.run(fileName);
    return 0;
}
