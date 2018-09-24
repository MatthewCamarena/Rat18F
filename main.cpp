#include <iostream>
#include <string>
#include <fstream>
#include <string>
#include <cassert>
#include "LexiconReader.h"
using namespace std;

int main(int argc, const char * argv[]) {
    string fileName = "testLex.txt";
    LexiconReader LR;
    LR.buildLexiconRecordfromDatafile(fileName);
    return 0;
}
