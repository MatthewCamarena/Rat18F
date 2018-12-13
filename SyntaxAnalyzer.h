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
#include "InstructionTable.h"
//#include "SymbolTable.h"
#include <stack>

ofstream myfile;
InstructionTable instrTable[1000]; // creates instruction table
int instrAddress = 1; // index for instrTable
int addr = 0; // global
int memAddr = 5000;

stack <int> jumpstack;

string symbolTable[1000];
string symbolType[1000];


using namespace std;

class SyntaxAnalyzer

{
public:
    // default constructor
    SyntaxAnalyzer()
    {
        seeSyntax =false;
        currentIndex = 0;
        state = 0;
        symbCount = 0;
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
        myfile.open("Output.txt");
        Rat18F();   //first rule of production
        printInstr();
        myfile.close();
    }
    
    void Rat18F()
    {
        if (seeSyntax) {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << "tokenType[currentIndex]" << endl << "\t<Rat18F> ::= <Opt Function Definitions> $$ <Opt Declaration List> <Statement List> $$" << endl;
        }
        
        //First call optFunc
        OptFuncDef();
        
        if (token[currentIndex] == "$$") {
            currentIndex++;
            OptDeclList();
            StatementList();
            
            int i = 0;
            myfile << "Symbol Table:";
            myfile << endl << "Identifier" << "         " << "Memory Location" << "       " << "Type" << endl;
            while (i < symbCount) {
                myfile << symbolTable[i] << "                  " << i + memAddr << "                    " << "Integer" << endl;
                i++;
            }
            myfile << endl;
            myfile << "Instruction Table:" << endl;
            
        }
        else {
            myfile << "ERROR: expected '$$' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
    }
    
    void OptFuncDef()
    {
        if (seeSyntax) { myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Opt Function Definitions> ::= <Function Definitions> | <Empty>" << endl; }
        
        //check to see if first token name is called function
        if (token[currentIndex] == "function")
        {
            FuncDef();
        }
        else if (token[currentIndex] == "$$") // otherwise there are no functions
        {
            Empty();
        }
        else
        {
            myfile << "ERROR: Expected 'function' or '$$' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
    }
    
    void FuncDef()
    {
        if (seeSyntax)
        {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Function Definitions> ::= <Function> | <Function><Function Definitions>" << endl;
        }
        // Keep calling while the next one is  a function to register all functions before main
        while (token[currentIndex] == "function") Function();
    }
    
    void Function()
    {
        if (seeSyntax)
        {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Function> ::= function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>" << endl;
        }
        
        currentIndex++; // Move on to the next token
        
        if (tokenType[currentIndex] == "identifier")
        {
            currentIndex++;
            if (token[currentIndex] == "(")
            {
                currentIndex++;
                OptParaList();
                if (token[currentIndex] == ")")
                {
                    currentIndex++;
                    OptDeclList();
                    Body();
                }
            }
            else
            {
                myfile << "ERROR: Expected '(' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                exit(1);
            }
        }
        else
        {
            myfile << "ERROR: Expected ')' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
        
    }
    
    void OptParaList()
    {
        if (seeSyntax)
        {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Opt Parameter List> ::= <Parameter List> | <Empty>" << endl;
        }
        
        if (tokenType[currentIndex] == "identifier") // there is a parameter in the function
        {
            ParaList();
        }
        else if (token[currentIndex] == ")")    // function can be empty
        {
            Empty();
        }
        else
        {
            myfile << "ERROR: Expected an <identifier> or ')' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
        
    }
    
    void ParaList()
    {
        if (seeSyntax)
        {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Parameter List> ::= <Parameter List> | <Empty>" << endl;
        }
        
        if (tokenType[currentIndex] == "identifier")
        {
            Parameter();
            if (token[currentIndex] == ",")
            {
                currentIndex++;
                ParaList();
            }
        }
        
    }
    
    void Parameter()
    {
        if (seeSyntax)
        {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Parameter> ::= <IDs> : <Qualifier>" << endl;
        }
        IDs();
        
        if (token[currentIndex] == ":")
        {
            currentIndex++;
            Qualifier();
        }
        else
        {
            myfile << "ERROR: Expected ':' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
    }
    
    void Qualifier()
    {
        if (seeSyntax)
        {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Qualifier> ::= int | boolean | real" << endl;
        }
        
        if (token[currentIndex] == "int") //see what the Qualifier is
        {
            if (seeSyntax)
            {
                myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Qualifier> ::= int" << endl;
            }
        }
        else if (token[currentIndex] == "boolean")
        {
            if (seeSyntax) { myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Qualifier> ::= boolean" << endl; }
        }
        else if (token[currentIndex] == "real")
        {
            if (seeSyntax) { myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Qualifier> ::= real" << endl; }
        }
        else
        {
            myfile << "ERROR: Expected 'int', 'real' or 'boolean' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
        currentIndex++;
        
    }
    
    void Body()
    {
        if (seeSyntax)
        {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Body> ::= { <Statement List> }" << endl;
        }
        
        if (token[currentIndex] == "{")
        {
            currentIndex++;
            StatementList();
            if (token[currentIndex] == "}")
            {
                currentIndex++;
            }
            else
            {
                myfile << "ERROR, expected '{' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                exit(1);
            }
        }
        else {
            myfile << "ERROR, expected '}' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
    }
    
    void OptDeclList()
    {
        if (seeSyntax)
        {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Opt Declaration List> ::= <Declaration List> | <Empty>" << endl;
        }
        if (token[currentIndex] == "{")
        {
            Empty();
        }
        else if (token[currentIndex] == "int" || token[currentIndex] == "boolean" || token[currentIndex] == "real")
        {
            DeclList();
        }
        else
        {
            myfile << "ERROR: Expected 'int', 'real' or 'boolean' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
    }
    
    void DeclList()
    {
        if (seeSyntax)
        {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Declaration List> ::= <Declaration>; | <Declaration>;<Declaration List>" << endl;
        }
        Declaration();
        if (token[currentIndex] == ";")
        {
            currentIndex++;
            
            if (token[currentIndex] == "int" || token[currentIndex] == "boolean" || token[currentIndex] == "real")
            {
                DeclList();
            }
        }
        else
        {
            myfile << "ERROR: Expected ';' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
    }
    
    void Declaration()
    {
        if (seeSyntax)
        {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Declaration> ::= <Qualifier><IDs>" << endl;
        }
        
        Qualifier();
        IDs();
    }
    
    void IDs()
    {
        if (tokenType[currentIndex] == "identifier")
        {
            if (seeSyntax)
            {
                myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<IDs> ::= <Identifier> | <Identifier>,<IDs>" << endl;
            }
            
            //check to see if id is in the table
            
            if (!dupli(token[currentIndex]))
            {
                symbolTable[symbCount] = token[currentIndex];
                symbolType[symbCount] = tokenType[currentIndex];
                symbCount++;
            }
            
            
            
            
            
            currentIndex++;
            if (token[currentIndex] == ",")
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
            myfile << "ERROR: Expected an <identifier> on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
    }
    
    void StatementList()
    {
        if (seeSyntax)
        {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Statement List> ::= <Statement> | <Statement> <Statement List>" << endl;
        }
        
        while (token[currentIndex] == "if" || token[currentIndex] == "put" || token[currentIndex] == "while" || token[currentIndex] == "return" || tokenType[currentIndex] == "identifier" || token[currentIndex] == "get")
        {
            Statement();
        }
        
    }
    
    void Statement()
    {
        if (seeSyntax) {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Statement> ::= <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>" << endl;
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
            myfile << "ERROR: expected <Statement> on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
        
    }
    
    void Compound()
    {
        if (seeSyntax) {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Compound> ::= { <Statement List> } \n ";
        }
        
        if (token[currentIndex] == "{") {
            currentIndex++;
            StatementList();
            
            if (token[currentIndex] == "}") {
                currentIndex++;
            }
            
            else {
                myfile << "ERROR: Expected '}' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                exit(1);
            }
        }
    }
    
    void Assign()
    {
        if (seeSyntax) {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Assign> ::= <Identifier> = <Expression>;\n";
        }
        
        if (tokenType[currentIndex] == "identifier") {
            currentIndex++;
            
            if (token[currentIndex] == "=") {
                currentIndex++;
                Expression();
                
                gen_instr("POPM", get_addr(token[currentIndex - 3]));
                
                
                if (token[currentIndex] == ";") {
                    currentIndex++;
                }
                
                else {
                    myfile << "ERROR: Expected ';' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                    exit(1);
                }
            }
            
            else {
                myfile << "ERROR: Expected '=' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                exit(1);
            }
        }
        
        else {
            myfile << "ERROR: Expected <identifier> on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
    }
    
    void If()
    {
        if (seeSyntax) {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<If> ::= if ( <Condition> ) <Statement> ifend | if ( <Condition> ) <Statement> else <Statement> ifend\n";
        }
        
        if (token[currentIndex] == "if") {
            addr = instrAddress;
            currentIndex++;
            if (token[currentIndex] == "(") {
                currentIndex++;
                Condition();
                if (token[currentIndex] == ")") {
                    currentIndex++;
                    Statement();
                    back_patch(instrAddress);
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
                            myfile << "ERROR: Expected 'ifend' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                            exit(1);
                        }
                    }
                    else {
                        myfile << "ERROR: Expected 'ifend' or 'else' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                        exit(1);
                    }
                }
                else {
                    myfile << "ERROR: Expected ')' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                    exit(1);
                }
            }
            else {
                myfile << "ERROR: Expected '(' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                exit(1);
            }
            
        }
        
    }
    
    void Return()
    {
        if (seeSyntax) {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Return> ::= return; |  return <Expression>;\n";
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
                myfile << "ERROR: Expected ';' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                exit(1);
            }
        }
    }
    
    void Print()
    {
        if (seeSyntax) {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Print> ::= put ( <Expression> );\n";
        }
        
        currentIndex++;
        
        
        if (token[currentIndex] == "(") {
            currentIndex++;
            Expression();
            //cout << "HELLO" << endl << "STDOUT?";
            //system("PAUSE");
            gen_instr("STDOUT", -99999);
            if (token[currentIndex] == ")") {
                currentIndex++;
                
                if (token[currentIndex] == ";") {
                    currentIndex++;
                }
                else {
                    myfile << "ERROR: expected ';' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                    exit(1);
                }
            }
            else {
                myfile << "ERROR: Expected ')' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                exit(1);
            }
        }
        else {
            myfile << "ERROR: Expected '(' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
    }
    
    void Scan()
    {
        if (seeSyntax) {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Scan> ::= get ( <IDs> );\n";
        }
        
        currentIndex++;
        
        if (token[currentIndex] == "(") {
            currentIndex++;
            IDs();
            gen_instr("STDIN", -99999);
            gen_instr("POPM", get_addr(token[currentIndex - 1]));
            
            if (token[currentIndex] == ")") {
                currentIndex++;
                
                if (token[currentIndex] == ";") {
                    currentIndex++;
                }
                else {
                    myfile << "ERROR: Expected ';' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                    exit(1);
                }
            }
            else {
                myfile << "ERROR: Expected ')' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                exit(1);
            }
        }
        
        else {
            myfile << "ERROR: Expected '(' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
    }
    
    void While()
    {
        if (seeSyntax) {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<While> ::= while ( <Condition> )  <Statement> whileend\n";
        }
        
        if (token[currentIndex] == "while") {
            
            addr = instrAddress;
            gen_instr("LABEL", -99999);
            currentIndex++;
            
            if (token[currentIndex] == "(") {
                currentIndex++;
                Condition();
                if (token[currentIndex] == ")") {
                    currentIndex++;
                    Statement();
                    gen_instr("JUMP", addr);
                    back_patch(instrAddress);
                    if (token[currentIndex] == "whileend") {
                        currentIndex++;
                    }
                    else
                    {
                        myfile << "ERROR: Expected 'whileend' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                        exit(1);
                    }
                }
                else {
                    myfile << "ERROR: Expected ')' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                    exit(1);
                }
            }
            else {
                myfile << "ERROR: Expected '(' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                exit(1);
            }
        }
        
        else {
            myfile << "ERROR: Expected '(' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
    }
    
    void Condition()
    {
        if (seeSyntax) {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Condition> ::= <Expression>  <Relop>  <Expression>\n";
        }
        
        Expression();
        Relop();
        Expression();
        
        if (token[currentIndex-2] == "<") { //-99999 to not display address
            gen_instr("LES", -99999);
        }
        else if (token[currentIndex-2] == ">") {
            gen_instr("GRE", -99999);
        }
        else if (token[currentIndex-2] == "==") {
            gen_instr("EQU", -99999);
        }
        else if (token[currentIndex-2] == "^=") {
            gen_instr("NEQ", -99999);
        }
        else if (token[currentIndex-2] == "=>") {
            gen_instr("GEQ", -99999);
        }
        else if (token[currentIndex-2] == "=<") {
            gen_instr("LEQ", -99999);
        }
        
        jumpstack.push(instrAddress);
        gen_instr ("JUMPZ", -99999);
        
    }
    
    void Relop()
    {
        if (seeSyntax) {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Relop> ::= == | ^= | > | < | => | =<";
        }
        
        if (token[currentIndex] == "==") {
            if (seeSyntax) {
                myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Relop> ::= ==" << endl;
            }
        }
        else if (token[currentIndex] == "^=") {
            if (seeSyntax) {
                myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Relop> ::= ^=" << endl;
            }
        }
        else if (token[currentIndex] == ">") {
            if (seeSyntax) {
                myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Relop> ::= >" << endl;
            }
        }
        else if (token[currentIndex] == "<") {
            if (seeSyntax) {
                myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Relop> ::= <" << endl;
            }
        }
        else if (token[currentIndex] == "=>") {
            if (seeSyntax) {
                myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Relop> ::= =>" << endl;
            }
        }
        else if (token[currentIndex] == "=<") {
            if (seeSyntax) {
                myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Relop> ::= =<" << endl;
            }
        }
        else {
            myfile << "ERROR: Expected '==','>','<','^=','=>','=<' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
        
        
        currentIndex++;
    }
    
    void Expression()
    {
        if (seeSyntax) {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Expression> ::= <Term> <ExpressionPrime>\n";
        }
        
        Term();
        ExpressionPrime();
    }
    
    void ExpressionPrime()
    {
        if (seeSyntax) {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<ExpressionPrime> ::= + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | <Empty>" << endl;
        }
        
        if (token[currentIndex] == "+") {
            currentIndex++;
            Term();
            gen_instr("ADD", -99999);
            ExpressionPrime();
        }
        else if (token[currentIndex] == "-") {
            currentIndex++;
            Term();
            ExpressionPrime();
        }
        else if (tokenType[currentIndex] == "identifer" || tokenType[currentIndex] == "keyword")
        {
            myfile << "ERROR: expected a token on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
        
        else {
            Empty();
        }
    }
    
    void Term()
    {
        if (seeSyntax) {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Term> ::= <Factor> <TermPrime>" << endl;
        }
        
        Factor();
        TermPrime();
    }
    
    void TermPrime()
    {
        if (seeSyntax) {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<TermPrime> ::= * <Factor> <TermPrime> | / <Factor> <TermPrime> | <Empty>" << endl;
        }
        
        if (token[currentIndex] == "*") {
            currentIndex++;
            Factor();
            gen_instr("MUL", -99999);
            TermPrime();
        }
        else if (token[currentIndex] == "/") {
            currentIndex++;
            Factor();
            TermPrime();
        }
        else if (tokenType[currentIndex] == "identifer" || tokenType[currentIndex] == "keyword")
        {
            myfile << "ERROR: expected a token on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
        else {
            Empty();
        }
    }
    
    void Factor()
    {
        if (seeSyntax) {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Factor> ::= - <Primary> | <Primary>" << endl;
        }
        
        if (token[currentIndex] == "-") {
            currentIndex++;
            Primary();
        }
        else if (tokenType[currentIndex] == "identifier") {
            Primary();
        }
        else if (tokenType[currentIndex] == "keyword" || tokenType[currentIndex] == "integer" || tokenType[currentIndex] == "real" || tokenType[currentIndex] == "separator" || tokenType[currentIndex] == "operator")
        {
            Primary();
        }
        else {
            myfile << "ERROR: invalid input on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
    }
    
    void Primary()
    {
        if (seeSyntax) {
            myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Primary> ::= <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false" << endl;
        }
        
        if (tokenType[currentIndex] == "identifier") {
            
            gen_instr("PUSHM", get_addr(token[currentIndex]));
            currentIndex++;
            
            if (token[currentIndex] == "(") {
                currentIndex++;
                IDs();
                
                if (token[currentIndex] == ")") {
                    currentIndex++;
                }
                
                else {
                    myfile << "ERROR: Expected ')' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                    exit(1);
                }
            }
            else {
                Empty();
            }
        }
        else if (tokenType[currentIndex] == "integer") {
            gen_instr("PUSHI", stoi(token[currentIndex]));
            currentIndex++;
        }
        else if (token[currentIndex] == "(") {
            currentIndex++;
            Expression();
            
            if (token[currentIndex] == ")") {
                currentIndex++;
                
            }
            else {
                myfile << "ERROR: Expected ')' on line " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
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
            myfile << "ERROR: missing '<Identifer>', 'Integer', '<Expression>', or '<Qualifier>' on line: " << tokenLineNum[currentIndex] << "Token: " << token[currentIndex] << "Lexeme: " << tokenType[currentIndex];
            exit(1);
        }
    }
    
    void Empty()
    {
        if (seeSyntax) { myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<Empty> ::= Epsilon" << endl; }
    }
    
    void errorHandler(int type)
    {
        
    }
    
    void gen_instr(string op, int operand) {  //create instruction for instruction table
        instrTable[instrAddress].address = instrAddress;
            instrTable[instrAddress].operand = operand;
        
        instrTable[instrAddress].op = op;
        
        //printInstr(instrAddress);
        
        instrAddress++;
        
    };
    
    
    
    void back_patch(int jump_addr) {
        
        
        if (!jumpstack.empty()) {
            addr = jumpstack.top();
            jumpstack.pop();
            instrTable[addr].operand = jump_addr;
        }
        else {
            cout << "Error: Stack came up empty." << endl;
            
            
        }
    }
    
    int get_addr(string ok) {            //takes a string and returns the address from the symbol table
        int addr = 0;
        int i = 0;
        
        while (i < symbCount+1) {
            
            if (symbolTable[i] == ok) {
                addr = i + memAddr;
            }
            i++;
        }
        
        
        return addr;
    }
    
    void printInstr() {
        for(int i = 1; i < instrAddress;i++){
            myfile << i << ". " << instrTable[i].op << " ";
            if(instrTable[i].operand != -99999) {
                myfile << instrTable[i].operand << endl;
            }
            else {
                myfile << endl;
            }
        }
    }
    
    bool dupli(string lex)
    {
        bool in = 0;
        int i = 0;
        while (i < symbCount + 1)
        {
            if (symbolTable[i] == lex)
                in = 1;
            i++;
        }
        return in;
    }
    
private:
    int tokenCount;
    string *token;
    string *tokenType;
    int *tokenLineNum;
    int currentIndex;
    int state;
    bool seeSyntax;
    int symbCount;
};

#endif /* SyntaxAnalyzer_h */
