//
//  LexiconReader.h
//  lexican
//
//  Created by Matthew Camarena on 9/24/18.
//  Copyright © 2018 Matthew Camarena. All rights reserved.
//

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
        keywords = new string[18]  {"function", "if", "ifend", "while", "whileend", "return", "int","real", "boolean", "else", "get", "put", "true", "False", "and", "or", "xor", "not"};
        separators = new string[7] {"{", "}","(",")", "]", ",", "$$"};
        operators = new string[5] {"-", "+" ,"*", "/",  ";"};
    }
    
    // destructor
    ~LexiconReader()
    {
        //Deletes allocated memory for the arrays
        delete []token;
        delete []tokenType;
        delete []separators;
        delete []operators;
    }
    
    void updateCurrentState(char s)
    {
        lexSTATE = cSTATE;  // update last state
        
    if(lexSTATE == 0)       // DEFAULT STARTING STATE
    {
        if(isalpha(s))      // ALPHA
        {
            addToLex(s);
            cSTATE = 1;
        }
        if(isnumber(s))     //NUMBERS
        {
            addToLex(s);
            cSTATE = 3;
        }
        if(!isalnum(s))     //EVERYTHING ELSE
        {
            // check ' ' or functions TO SKIP
            if(s == ' ')
                cSTATE = 0;
            else if(s == '[') //potential comment out section
            {
               
                cSTATE = 6;
            }
            else
            {
                addToLex(s);
                if(isSeparator()){tokenType[index] = "separator";
                    updateIndex();
                }
                else
                cSTATE = 5;
            }
        }
    }
    else if(lexSTATE == 1)              // ALPHA INPUT
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
            if(isKeyword())
                tokenType[index] = "keyword";
            else
                tokenType[index] = "identifier";
            cSTATE = 0;
            if(s == ' '){updateIndex();}
            else
            {

                updateIndex();
                updateCurrentState(s);
            }
            
        }
    }
    else if(lexSTATE == 2)  // NUMERIC INPUT AFTER ALPHA
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
    else if(lexSTATE == 3)  // NUMERIC INPUT
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
                tokenType[index] = "integer";
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
                tokenType[index] = "integer";
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
            tokenType[index] = "real";
            cSTATE = 0;
            updateIndex();
            if(s != ' ')
            updateCurrentState(s);
            
        }
    }
    else if(lexSTATE == 5)  //NON ALPHA/NUM
    {
        if(isalpha(s))
        {
            
            //create token


            cSTATE = 1;
            if(isSeparator())
            {
                tokenType[index] = "separator";
            }
            else
            tokenType[index] = "operator";
            
            updateIndex();
            updateCurrentState(s);

            
        }
        if(isnumber(s))
        {
            
            //create token
            cSTATE = 3;
            if(isSeparator())
            {
                tokenType[index] = "separator";
            }
            else
            tokenType[index] = "operator";
            updateIndex();
            updateCurrentState(s);
            
        }
        if(!isalnum(s))
        {
            // create token check type if
            if(s == ' ')
            {
                if(s == '$' && lastToken == '$')
                {
                    tokenType[index] = "separator";
                }
                else
                tokenType[index] = "operator";
                updateIndex();
            }
            else
            {
                addToLex(s);
                
                if(isSeparator())
                {
                    tokenType[index] = "separator";
                    cout << "hi";
                }
                else
                {
                tokenType[index] = "operator";
                
                }
                updateIndex();
            }
            cSTATE = 0;
        }
    }
    else if(lexSTATE == 6) // LAST TOKEN WAS  [ NEED TO CHECK  FOR *
    {
       
        if(s == '*')
        {
            cSTATE = 7;
        }
        else
        {
            addToLex(lastToken);
            tokenType[index] = "separator";
            updateIndex();
            cSTATE = 0;
            updateCurrentState(s);
        }
    }
    else if(lexSTATE == 7)// comment mode
    {
      
        if(s == '*')
        {
            cSTATE = 8;
        }
    }
    else if(lexSTATE == 8)
    {
 
        if(s == ']')
        {
            cSTATE = 0;
        }
        else
            cSTATE = 7;
    }
            
    
    lastToken = s;
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
    
    bool isKeyword()
    {
        for(int i = 0; i < 18; i++)
        {
            if (token[index] == keywords[i]) {return 1;}
        }
        return 0;
    }
    bool isSeparator()
    {
        for(int i = 0; i < 7; i++)
        {
            if (token[index] == separators[i]) {return 1;}
        }
        return 0;
    }
    
    // Load information from a text file with the given filename.
    void buildLexiconRecordfromDatafile(string filename)
    {
        fstream myfile(filename);
        
        if (myfile.is_open())
        {
            cout << "Successfully opened file " << filename << endl;
            string line;

            char t;
       
            while (getline(myfile,line))
            {
                line += ' ';
          
                for(int i = 0; i < line.length(); i++)
                {
                
                    t = line[i];
                    updateCurrentState(t);
                   //cout << "letter: " << t << " state: " << cSTATE << endl;
                }
            
                
                
            }
            myfile.close();
            for(int i = 0; i < index + 1; i++)
            {
 
                cout << left << "{" << token[i]  << "} is a " <<  "["<<  tokenType[i]<< "]" << endl;
            }
         
        }
        else
            throw invalid_argument("Could not open file " + filename);
    }
 
private:
    string *keywords;
    string *separators;
    string *operators;
    int lexSTATE, cSTATE, index;
    string *token;
    string *tokenType;
    string temp;
    char lastToken;
    
};
#endif /* LexiconReader_h */
