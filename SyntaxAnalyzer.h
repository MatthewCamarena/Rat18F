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
#include <stdlib.h>

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
        
        if (token[currentIndex] == "$$") {
            currentIndex++;
            OptDeclList();
            StatementList();
            
            cout << "Completed" << endl;
        }
        else {
            cout << "ERROR: expected '$$' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
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
            cout << "ERROR: Expected 'function' or '$$' on line: " <<tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
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
                    currentIndex++;
                    OptDeclList();
                    Body();
                }
            }
            else
            {
                cout << "ERROR: Expected '(' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                exit(1);
            }
        }
        else
        {
            cout << "ERROR: Expected ')' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
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
            cout << "ERROR: Expected an <identifier> or ')' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
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
            cout << "ERROR: Expected ':' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
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
            cout << "ERROR: Expected 'int', 'real' or 'boolean' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
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
                cout << "ERROR, expected '{' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                exit(1);
            }
        }
        else{
            cout << "ERROR, expected '}' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
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
            cout << "ERROR: Expected 'int', 'real' or 'boolean' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
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
            cout << "ERROR: Expected ';' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
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
            cout << "ERROR: Expected an <identifier> on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
    }
    
    void StatementList()
    {
        if(seeSyntax)
        {
            cout << "\t<Statement List> ::= <Statement> | <Statement> <Statement List>" << endl;
        }
        
        while(token[currentIndex] == "if" || token[currentIndex] == "put" || token[currentIndex] == "while" || token[currentIndex] == "return" || tokenType[currentIndex] == "identifier" || token[currentIndex] == "get")
        {
            Statement();
        }
        
    }
    
    void Statement()
    {
        if(seeSyntax){
            cout << "\t<Statement> ::= <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>" << endl;
        }
        if (token[currentIndex] == "{") {
            Compound();
        }
        
        else if (tokenType[currentIndex] == "identifier") {
            Assign();
        }
        
        else if (token[currentIndex] == "if") {
            If();
        }
        
        else if (token[currentIndex] == "return") {
            Return();
        }
        
        else if (token[currentIndex] == "put") {
            Print();
        }
        
        else if (token[currentIndex] == "get") {
            Scan();
        }
        
        else if (token[currentIndex] == "while") {
            While();
        }
        
        else {
            cout << "ERROR: expected <Statement> on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
        
    }
    
    void Compound()
    {
        if (seeSyntax) {
            cout << "\t<Compound> ::= { <Statement List> } \n ";
        }
        
        if (token[currentIndex] == "{") {
            currentIndex++;
            StatementList();
            
            if (token[currentIndex] == "}") {
                currentIndex++;
            }
            
            else {
                cout << "ERROR: Expected '}' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                exit(1);
            }
        }
    }
    
    void Assign()
    {
        if (seeSyntax) {
            cout << "\t<Assign> ::= <Identifier> = <Expression>;\n";
        }
        
        if (tokenType[currentIndex] == "identifier") {
            currentIndex++;
            
            if (token[currentIndex] == "=") {
                currentIndex++;
                Expression();

                
                if (token[currentIndex] == ";") {
                    currentIndex++;
                }
                
                else {
                    cout << "ERROR: Expected ';' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                    exit(1);
                }
            }
            
            else {
                cout << "ERROR: Expected '=' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                exit(1);
            }
        }
        
        else {
            cout << "ERROR: Expected <identifier> on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
    }
    
    void If()
    {
        if (seeSyntax) {
            cout << "\t<If> ::= if ( <Condition> ) <Statement> ifend | if ( <Condition> ) <Statement> else <Statement> ifend\n";
        }
        
        if (token[currentIndex] == "if") {
            currentIndex++;
            if (token[currentIndex] == "(") {
                currentIndex++;
                Condition();
                if (token[currentIndex] == ")") {
                    currentIndex++;
                    Statement();
                    if (token[currentIndex] == "ifend") {
                        currentIndex++;
                    }
                    else if (token[currentIndex] == "else") {
                        currentIndex++;
                        Statement();
                        
                        if (token[currentIndex] == "ifend") {
                            currentIndex++;
                        }
                        else {
                            cout << "ERROR: Expected 'ifend' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                            exit(1);
                        }
                    }
                    else {
                        cout << "ERROR: Expected 'end if' or 'else' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                        exit(1);
                    }
                }
                else {
                    cout << "ERROR: Expected ')' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                    exit(1);
                }
            }
            else {
                cout << "ERROR: Expected '(' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                exit(1);
            }
            
        }
        
    }
    
    void Return()
    {
        if (seeSyntax) {
            cout << "\t<Return> ::= return; |  return <Expression>;\n";
        }
        
        currentIndex++;
        
        if (token[currentIndex] == ";") {
            currentIndex++;
        }
        
        else {
            Expression();
            if (token[currentIndex] == ";") {
                currentIndex++;
            }
            else {
                cout << "ERROR: Expected ';' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                exit(1);
            }
        }
    }
    
    void Print()
    {
        if (seeSyntax) {
            cout << "\t<Print> ::= put ( <Expression> );\n";
        }
        
            currentIndex++;
            
            
            if (token[currentIndex] == "(") {
                currentIndex++;
                Expression();
                if (token[currentIndex] == ")") {
                    currentIndex++;
                    
                    if (token[currentIndex] == ";") {
                        currentIndex++;
                    }
                    else {
                        cout << "ERROR: expected ';' on line: "<<  tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                        exit(1);
                    }
                }
                else {
                    cout << "ERROR: Expected ')' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                    exit(1);
                }
            }
            else {
                cout << "ERROR: Expected '(' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                exit(1);
            }
    }
    
    void Scan()
    {
        if (seeSyntax) {
            cout << "\t<Scan> ::= get ( <IDs> );\n";
        }
        
        currentIndex++;
        
        if (token[currentIndex] == "(") {
            currentIndex++;
            IDs();
            
            if (token[currentIndex] == ")") {
                currentIndex++;
                
                if (token[currentIndex] == ";") {
                    currentIndex++;
                }
                else {
                    cout << "ERROR: Expected ';' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                    exit(1);
                }
            }
            else {
                cout << "ERROR: Expected ')' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                exit(1);
            }
        }
        
        else {
            cout << "ERROR: Expected '(' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
    }
    
    void While()
    {
        if (seeSyntax) {
            cout << "\t<While> ::= while ( <Condition> )  <Statement>\n";
        }
        
        if (token[currentIndex] == "while") {

            currentIndex++;
            
            if (token[currentIndex] == "(") {
                currentIndex++;
                Condition();
                if (token[currentIndex] == ")") {
                    currentIndex++;
                    Statement();
                    if (token[currentIndex] == "whileend") {
                        currentIndex++;
                    }
                }
                else {
                    cout << "ERROR: Expected ')' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                    exit(1);
                }
            }
            else {
                cout << "ERROR: Expected '(' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                exit(1);
            }
        }
        
        else {
            cout << "ERROR: Expected '(' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
    }
    
    void Condition()
    {
        if (seeSyntax) {
            cout << "\t<Condition> ::= <Expression>  <Relop>  <Expression>\n";
        }
        
        Expression();
        Relop();
        Expression();
        

    
    }
    
    void Relop()
    {
  
        if (token[currentIndex] == "==") {
            if (seeSyntax) {
                cout << "\t<Relop> ::= ==" << endl;
            }
        }
        else if (token[currentIndex] == "^=") {
            if (seeSyntax) {
                cout << "\t<Relop> ::= ^=" << endl;
            }
        }
        else if (token[currentIndex] == ">") {
            if (seeSyntax) {
                cout << "\t<Relop> ::= >" << endl;
            }
        }
        else if (token[currentIndex] == "<") {
            if (seeSyntax) {
                cout << "\t<Relop> ::= <" << endl;
            }
        }
        else if (token[currentIndex] == ">=") {
            if (seeSyntax) {
                cout << "\t<Relop> ::= >=" << endl;
            }
        }
        else if (token[currentIndex] == "<=") {
            if (seeSyntax) {
                cout << "\t<Relop> ::= <=" << endl;
            }
        }
        else {
            cout << "ERROR: Expected '==','>','<','^=','>=','<=' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
        
        currentIndex++;
    }
    
    void Expression()
    {
        if (seeSyntax) {
            cout << "\t<Expression> ::= <Term> <ExpressionPrime>\n";
        }
        
        Term();
        ExpressionPrime();
    }
    
    void ExpressionPrime()
    {
        if (seeSyntax) {
            cout << "\t<ExpressionPrime> ::= + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | <Empty>" << endl;
        }
        
        if (token[currentIndex] == "+" || token[currentIndex] == "-") {
            currentIndex++;
            Term();
            ExpressionPrime();
        }
        else if(tokenType[currentIndex] == "identifer" || tokenType[currentIndex] == "keyword")
        {
            cout << "ERROR: expected a token on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
        
        else {
            Empty();
        }
    }
    
    void Term()
    {
        if (seeSyntax) {
            cout << "\t<Term> ::= <Factor> <TermPrime>" << endl;
        }
        
        Factor();
        TermPrime();
    }
    
    void TermPrime()
    {
        if (seeSyntax) {
            cout << "\t<TermPrime> ::= * <Factor> <TermPrime> | / <Factor> <TermPrime> | <Empty>" << endl;
        }
        
        if (token[currentIndex] == "*" || token[currentIndex] == "/") {
            currentIndex++;
            Factor();
            TermPrime();
        }
        else if(tokenType[currentIndex] == "identifer" || tokenType[currentIndex] == "keyword")
        {
            cout << "ERROR: expected a token on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
        else {
            Empty();
        }
    }
    
    void Factor()
    {
        if (seeSyntax) {
            cout << "\t<Factor> ::= - <Primary> | <Primary>" << endl;
        }
        
        if (token[currentIndex] == "-") {
            currentIndex++;
            Primary();
        }
        else if (tokenType[currentIndex] == "identifier" || tokenType[currentIndex] == "keyword" || tokenType[currentIndex] == "integer" || tokenType[currentIndex] == "real" || tokenType[currentIndex] == "separator" || tokenType[currentIndex] == "operator")
        {
            Primary();
        }
        else {
            cout << "ERROR: invalid input on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
    }
    
    void Primary()
    {
        if (seeSyntax) {
            cout << "\t<Primary> ::= <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false" << endl;
        }
        
        if(tokenType[currentIndex] == "identifier") {
            currentIndex++;
        
            if (token[currentIndex] == "(") {
            currentIndex++;
            IDs();
            
                if (token[currentIndex] == ")") {
                    currentIndex++;
                }
            
                else {
                    cout << "ERROR: Expected ')' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                    exit(1);
                }
            }
            else {
                Empty();
            }
        }
        else if (tokenType[currentIndex] == "integer") {
            currentIndex++;
        }
        else if (token[currentIndex] == "(") {
            currentIndex++;
            Expression();
            
            if (token[currentIndex] == ")") {
                currentIndex++;
                
            }
            else {
                cout << "ERROR: Expected ')' on line " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                exit(1);
            }
        }
        
        else if (tokenType[currentIndex] == "real") {
            currentIndex++;
            
        }
        
        else if (token[currentIndex] == "true") {
            currentIndex++;
            
        }
        
        else if (token[currentIndex] == "false") {
            currentIndex++;
            
        }
        
        else {
            cout << "ERROR: missing '<Identifer>', 'Integer', '<Expression>', or '<Qualifier>' on line: " << tokenLineNum[currentIndex] << "Token: " << token[currentIndex] << "Lexeme: " << tokenType[currentIndex];
            exit(1);
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
