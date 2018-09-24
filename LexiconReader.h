#ifndef LexiconReader_h
#define LexiconReader_h


#pragma once
#include <string>
#include <stdexcept>

using namespace std;

class LexiconReader
{
public:
    // default constructor
    LexiconReader()
    {
     
    }
    
    // destructor
    ~LexiconReader()
    {
        //Deletes allocated memory for the arrays
     
    }
    
    // Load information from a text file with the given filename.
    void buildLexiconRecordfromDatafile(string filename)
    {
        fstream myfile(filename);
        
        if (myfile.is_open())
        {
            cout << "Successfully opened file " << filename << endl;
            string line;
            
            while (getline(myfile,line))
            {
                cout << line << endl;
     
            }
            myfile.close();
        }
        else
            throw invalid_argument("Could not open file " + filename);
    }
 
private:

};

#endif /* LexiconReader_h */
