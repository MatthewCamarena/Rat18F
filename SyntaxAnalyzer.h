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
        
    }
    
    void ParaList()
    {
        
    }
    
    void Parameter()
    {
        
    }
    
    void Qualifier()
    {
        
    }
    
    void Body()
    {
        
    }
    
    void OptDeclList()
    {
        
    }
    
    void DeclList()
    {
        
    }
    
    void Declaration()
    {
        
    }
    
    void IDs()
    {
        
    }
    
    void StatementList()
    {
        
        
    }
    
    void Statement()
    {
        
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
    
    void ExpressionPrime() {
        if (seeSyntax) {
            cout << "\t<ExpressionPrime> -> + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | <Empty>   \n";
        }
        
        if (currentToken.token == "+" || currentToken.token == "-") {
            currentIndex++;
            Term();
            ExpressionPrime();
        }
        
        else {
            Empty();
        }
    }
    
    void Term()
    {
        if (seeSyntax) {
            cout << "\t<Term> -> <Factor> <TermPrime>\n";
        }
        
        Factor();
        TermPrime();
    }
    
    void TermPrime() {
        
        if (seeSyntax) {
            cout << "\t<TermPrime> -> * <Factor> <TermPrime> | / <Factor> <TermPrime> | <Empty>\n";
        }
        
        if (token[currentIndex] == "*" || token[currentIndex] == "/") {
        currentIndex++;
        Factor();
        TermPrime();
        }
            
        else {
            Empty();
        }
    }
            
    
    void Factor()
    {
        if (seeSyntax) {
            cout << "\t<Factor> -> - <Primary> | <Primary>\n";
        }
    
        if (token[currentIndex] == "-") {
            currentIndex++;
            Primary();
        }
    
        else {
            cout << "ERROR";
        }
    }
    
    void Primary()
    {
        if (seeSyntax) {
            cout << "\t<Primary> -> <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false\n";
        }
        
        if (token[currentIndex] == "(") {
            currentIndex++;
            
            if (token[currentIndex] == ")") {
                currentIndex++;
            }
            
            else {
                cout << "ERROR";
            }
        }
        else {
            Empty();
        }
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
