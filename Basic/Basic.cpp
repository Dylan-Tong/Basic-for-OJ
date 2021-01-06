/*
 * File: Basic.cpp
 * ---------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;

/* Function prototypes */

void processLine(string line, Program & program, EvalState & state);

/* Main program */

int main() {
   EvalState state;
   Program program;
   cout << "Stub implementation of BASIC" << endl;
   while (true) {
      try {
         processLine(getLine(), program, state);//evalstate相当于内存空间
      } catch (ErrorException & ex) {
         cerr << "Error: " << ex.getMessage() << endl;
      }
   }
   return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program & program, EvalState & state) {
   TokenScanner scanner;
   scanner.ignoreWhitespace();
   scanner.scanNumbers();
   scanner.setInput(line);
/*
   Expression *exp = parseExp(scanner);//通过从扫描仪读取令牌来解析表达式
   int value = exp->eval(state);
   cout << value << endl;
   delete exp;
*/
    string token = scanner.nextToken();
    TokenType type = scanner.getTokenType(token);
    if (type == NUMBER)
    {
        int line_number=stringToInteger(token);
        if(!scanner.hasMoreTokens()){
            program.removeSourceLine(line_number);
            return;
        }
        program.addSourceLine(line_number,line);//添加源line
        // 解析句子：
        token=scanner.nextToken();
        //type=scanner.getTokenType(token);
        //if(type!= WORD)
        if(token=="LET")
        {
            Expression* exp=parseExp(scanner);
            program.setParsedStatement(line_number,new LET(exp));
            exp= nullptr;
        }
        else if(token=="PRINT")
        {
            Expression* exp=parseExp(scanner);
            program.setParsedStatement(line_number,new PRINT(exp));
            exp= nullptr;
        }
        else if(token=="INPUT")
        {
            token=scanner.nextToken();
            program.setParsedStatement(line_number,new INPUT(token));
        }
        else if(token=="END")
        {
            program.setParsedStatement(line_number,new END());
        }
        else if(token=="GOTO")
        {
            token=scanner.nextToken();
            int from_line=line_number;
            int to_line=stringToInteger(token);
            program.setParsedStatement(line_number,new GOTO(from_line,to_line));
        }
        else if(token=="IF")
        {
            string l_string,r_string;
            token=scanner.nextToken();
            do{
              l_string+=token;
              token=scanner.nextToken();
              cout<<l_string<<"\n";
            }while(token!=">"&&token!="<"&&token!="=");

            string cmp=token;

            token=scanner.nextToken();
            do{
                r_string+=token;
                token=scanner.nextToken();
                cout<<r_string<<"\n";
            }while(token!="THEN");

            int l=stringToInteger(scanner.nextToken());

            scanner.setInput(l_string);
            Expression* l_exp=parseExp(scanner);
            scanner.setInput(r_string);
            Expression* r_exp=parseExp(scanner);

            program.setParsedStatement(line_number,new IF_THEN(l_exp,cmp,r_exp,l,line_number));
            l_exp= nullptr;r_exp= nullptr;
        }
        else if(token=="REM")
        {
         program.setParsedStatement(line_number,new REM());
        }
    }
    else if(type==WORD)
    {
        if(token=="RUN")program.RUN(program.getFirstLineNumber(),state);
        else if(token=="LIST")program.LIST(state);
        else if(token=="CLEAR")program.CLEAR(state);
        else if(token=="QUIT")exit(0);
        else if(token=="HELP")
        {
            cout<<"We have REM LET PRINT INPUT END\n";
        }
    }

}
