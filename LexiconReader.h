#ifndef LexiconReader_h
#define LexiconReader_h


#pragma once
#include <string>
#include <cstring>
#include <stdexcept>
#include <vector>

using namespace std;

class LexiconReader
{
public:
    // default constructor
    LexiconReader()
    {
        cSTATE = 0;
        lexSTATE = 0;
        token = new string[1000000];
        tokenType = new string[1000000];
        index = 0;
    }
    
    // destructor
    ~LexiconReader()
    {
        //Deletes allocated memory for the arrays
        delete []token;
        delete []tokenType;
     
    }
    
        void updateCurrentState(char s)
    {
        lexSTATE = cSTATE;
        lastToken = s;
        
    if(lexSTATE == 0)
    {
        if(isalpha(s))
        {
            addToLex(s);
            cSTATE = 1;
        }
        if(isnumber(s))
        {
            addToLex(s);
            cSTATE = 3;
        }
        if(!isalnum(s))
        {
            // check ' ' or functions
            if(s == ' ')
                cSTATE = 0;
            else
            {
                addToLex(s);
                cSTATE = 5;
            }
        }
    }
    else if(lexSTATE == 1)
    {
        if(isalpha(s))
        {
            addToLex(s);
            cSTATE = 1;
        }
        if(isnumber(s))
        {
            addToLex(s);
            cSTATE = 2;
        }
        if(!isalnum(s))
        {
           //create a token
            
            cSTATE = 0;
            if(s == ' '){updateIndex();}
            else
            {
                updateIndex();
                updateCurrentState(s);
            }
            
        }
    }
    else if(lexSTATE == 2)
    {
        if(isalpha(s))
        {
            addToLex(s);
            cSTATE = 1;
        }
        if(isnumber(s))
        {
            addToLex(s);
            cSTATE = 2;
        }
        if(!isalnum(s))
        {
            // not a valid token
            cSTATE = 0;
            updateCurrentState(s);
         
        }
    }
    else if(lexSTATE == 3)
    {
        if(isalpha(s))
        {
            cSTATE = 0;
            //not a valid token
            
        }
        if(isnumber(s))
        {
            addToLex(s);
            cSTATE = 3;
        }
        if(!isalnum(s))
        {
            // check ' ' or functions
            if(s == ' ')
            {
                cSTATE = 0;
                updateIndex();
            }
            else if (s == '.')
            {
                addToLex(s);
                cSTATE = 4;
            }
            else
            {
                updateIndex();
                cSTATE = 0;
                updateCurrentState(s);
            }
            
        }
    }
    else if(lexSTATE == 4)
    {
        if(isalpha(s))
        {
            cSTATE = 0;
            updateCurrentState(s);
        }
        if(isnumber(s))
        {
            cSTATE = 4;
            addToLex(s);
        }
        if(!isalnum(s))
        {
            // check ' ' or functions
            
            cSTATE = 0;
            updateIndex();
            if(s != ' ')
            updateCurrentState(s);
            
        }
    }
    else if(lexSTATE == 5)
    {
        if(isalpha(s))
        {
            //create token
            cSTATE = 1;
            updateIndex();
            updateCurrentState(s);
            
        }
        if(isnumber(s))
        {
            //create token
            cSTATE = 3;
            updateIndex();
            updateCurrentState(s);
            
        }
        if(!isalnum(s))
        {
            // create token check type if
            if(s == ' ')
            {
                updateIndex();
            }
            else
            {
                addToLex(s);
                updateIndex();
            }
            cSTATE = 0;
        }
    }
    
    }
    void updateIndex(){index++;}
    
    
    void addToLex(char s)
    {
        token[index] += s;
    }
    void resetState()
    {
        cSTATE = 0;
        lexSTATE = 0;
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
    int lexSTATE, cSTATE, index;
    string *token, lastToken;
    string *tokenType;
};

#endif /* LexiconReader_h */
