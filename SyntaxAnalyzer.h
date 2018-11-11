//
//  SyntaxAnalyzer.h
//  lexican
//
//  Created by Matthew Camarena on 11/10/18.
//  Copyright © 2018 Matthew Camarena. All rights reserved.
//

#ifndef SyntaxAnalyzer_h
#define SyntaxAnalyzer_h

#pragma once
#include <string>
#include <cstring>
#include <stdexcept>
#include <vector>

using namespace std;

class SyntaxAnalyzer
{
public:
    // default constructor
    SyntaxAnalyzer()
    {
        seeSyntax = true;
        currentIndex = 0;
        state = 0;
    }
    
    // destructor
    ~SyntaxAnalyzer()
    {
        //Deletes allocated memory for the arrays
  
        
    }
    
    void run(string fileName)
    {
        LexiconReader LR;
        LR.buildLexiconRecordfromDatafile(fileName);
        tokenCount = LR.getNumTokens();
        token = LR.getTokens();
        tokenType = LR.getTokenType();
        tokenLineNum = LR.getTokenLineNum();
        Rat18F();   //first rule of production
    }
    
    void Rat18F()
    {
        if(seeSyntax) {cout << "\t<Rat18F> ::= <Opt Function Definitions> $$ <Opt Declaration List> <Statement List> $$" << endl;}
        //First call optFunc
        OptFuncDef();
    }
    
    void OptFuncDef()
    {
        if(seeSyntax) { cout << "\t<Opt Function Definitions> ::= <Function Definitions> | <Empty>" << endl;}
        
        //check to see if first token name is called function
        if(token[currentIndex] == "function")
        {
            FuncDef();
        }
        else if (token[currentIndex] == "$$") // otherwise there are no functions
        {
            Empty();
        }
        else
        {
            cout << "ERROR: Expected function or $$ on line: " << endl;
            errorHandler(1);
        }
    }
    
    void FuncDef()
    {
        if(seeSyntax)
        {
            cout << "\t<Function Definitions> ::= <Function> | <Function><Function Definitions>" << endl;
        }
        // Keep calling while the next one is  a function to register all functions before main
        while(token[currentIndex] == "function") Function();
    }
    
    void Function()
    {
        if(seeSyntax)
        {
            cout << "\t<Function> ::= function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>" << endl;
        }
        
        currentIndex++; // Move on to the next token
        
        if(tokenType[currentIndex] == "identifier")
        {
            currentIndex++;
            if(token[currentIndex] == "(")
            {
                currentIndex++;
                OptParaList();
                if(token[currentIndex] == ")")
                {
                    OptDeclList();
                    Body();
                }
            }
            else
            {
                cout << "ERROR";
            }
        }
        else
        {
            cout << "ERROR: ";
        }
        
    }

    void OptParaList()
    {
        if(seeSyntax)
        {
            cout << "\t<Opt Parameter List> ::= <Parameter List> | <Empty>" << endl;
        }
        
        if(tokenType[currentIndex] == "identifier") // there is a parameter in the function
        {
            ParaList();
        }
        else if (token[currentIndex] == ")")    // function can be empty
        {
            Empty();
        }
        else
        {
            cout << "ERROR";
        }
        
    }
    
    void ParaList()
    {
        if(seeSyntax)
        {
            cout << "\t<Parameter List> ::= <Parameter List> | <Empty>" << endl;
        }
        
        if(tokenType[currentIndex] == "identifier")
        {
            Parameter();
            if(token[currentIndex] == ",")
            {
                currentIndex++;
                ParaList();
            }
        }
        
    }
    
    void Parameter()
    {
        if(seeSyntax)
        {
            cout << "\t<Parameter> ::= <IDs> : <Qualifier>" << endl;
        }
        IDs();
        
        if(token[currentIndex] == ":")
        {
            currentIndex++;
            Qualifier();
        }
        else
        {
            cout << "ERROR";
        }
    }
    
    void Qualifier()
    {
        if(seeSyntax)
        {
            cout << "\t<Qualifier> ::= int | boolean | real" << endl;
        }
        
        if(token[currentIndex] == "int")
        {
            if(seeSyntax)
            {
                cout << "\t<Qualifier> ::= int" << endl;
            }
        }
        else if (token[currentIndex] == "boolean")
        {
            cout << "\t<Qualifier> ::= boolean" << endl;
        }
        else if(token[currentIndex] == "real")
        {
            cout << "\t<Qualifier> ::= real" << endl;
        }
        else
        {
            cout << "ERROR";
        }
        currentIndex++;
        
    }
    
    void Body()
    {
        if(seeSyntax)
        {
            cout << "\t<Body> ::= { <Statement List> }" << endl;
        }
        
        if(token[currentIndex] == "{")
        {
            currentIndex++;
            StatementList();
            if(token[currentIndex] == "}")
            {
                currentIndex++;
            }
            else
            {
                cout << "ERROR";
            }
        }
        else{
            cout << "ERROR";
        }
    }
    
    void OptDeclList()
    {
        if(seeSyntax)
        {
            cout << "\t<Opt Declaration List> ::= <Declaration List> | <Empty>" << endl;
        }
        if(token[currentIndex] == "{")
        {
            Empty();
        }
        else if(token[currentIndex] == "int" || token[currentIndex] == "boolean" || token[currentIndex] == "real")
        {
            DeclList();
        }
        else
        {
            cout << "ERROR";
        }
    }
    
    void DeclList()
    {
        if(seeSyntax)
        {
            cout << "\t<Declaration List> ::= <Declaration>; | <Declaration>;<Declaration List>" << endl;
        }
        Declaration();
        if(token[currentIndex] == ";")
        {
            currentIndex++;
            
            if(token[currentIndex] == "int" || token[currentIndex] == "boolean" || token[currentIndex] == "real")
            {
                DeclList();
            }
        }
        else
        {
            cout << "ERROR";
        }
    }
    
    void Declaration()
    {
        if(seeSyntax)
        {
            cout << "\t<Declaration> ::= <Qualifier><IDs>" << endl;
        }
        
        Qualifier();
        IDs();
    }
    
    void IDs()
    {
        if(tokenType[currentIndex] == "identifier" )
        {
            if(seeSyntax)
            {
                cout << "\t<IDs> ::= <Identifier> | <Identifier>,<IDs>" << endl;
            }
            currentIndex++;
            if(token[currentIndex] == ",")
            {
                currentIndex++;
                IDs();
            }
            else
            {
                Empty();
            }
        }
        else
        {
            cout << "ERROR";
        }
    }
    
    void StatementList()
    {
        if(seeSyntax)
        {
            cout << "\t<Statement List> ::= <Statement> | <Statement> <Statement List>" << endl;
        }
        
        while(token[currentIndex] == "if" || token[currentIndex] == "put" || token[currentIndex] == "while" || token[currentIndex] == "return" || tokenType[currentIndex] == "identifier")
        {
            Statement();
        }
        
    }
    
    void Statement()
    {
        if(seeSyntax){
            cout << "\t<Statement> ::= <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>" << endl;
        }
        
        
    }
    
    void Compound()
    {
        
    }
    
    void Assign()
    {
        
    }
    
    void If()
    {
        
    }
    
    void Return()
    {
        
    }
    
    void Print()
    {
        
    }
    
    void Scan()
    {
        
    }
    
    void While()
    {
        
    }
    
    void Condition()
    {
        
    }
    
    void Relop()
    {
        
    }
    
    void Expression()
    {
        
    }
    
    void Term()
    {
        
    }
    
    void Factor()
    {
        
    }
    
    void Primary()
    {
        
    }
    
    void Empty()
    {
        if(seeSyntax) {cout << "\t<Empty> ::= Epsilon" << endl; }
    }
    
    void errorHandler(int type)
    {
        
    }
    
    
    
private:
    int tokenCount;
    string *token;
    string *tokenType;
    int *tokenLineNum;
    int currentIndex;
    int state;
    bool seeSyntax;
};

#endif /* SyntaxAnalyzer_h */
