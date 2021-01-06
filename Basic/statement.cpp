/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
using namespace std;

/* Implementation of the Statement class */

Statement::Statement() {
   /* Empty */
}

Statement::~Statement() {
   /* Empty */
}


void REM::execute(EvalState & state){};


LET::LET(Expression* _exp):exp(_exp){}
//exp要在解析的时候调用prased!!
void LET::execute(EvalState &state){
    exp->eval(state);
}


PRINT::PRINT(Expression* _exp):exp(_exp){}

void PRINT::execute(EvalState& state){
    if(exp->getType()==IDENTIFIER){
        if(!state.isDefined(exp->toString()))
            error("User types wrong value to answer INPUT statement.");
    }
    cout<<exp->eval(state)<<endl;
}

INPUT::INPUT(string s):var(s),value(0){};

void INPUT::execute(EvalState &state) {
    cout<<"?";
   string token;
   cin>>token;
    for(char c:token)
    {
        if(c<'0'||c>'9')error("INVALID NUMBER");
    }
    value=stringToInteger(token);
    state.setValue(var,value);
}


void END::execute(EvalState& state){
    state.setValue("END",1);
}


GOTO::GOTO(int f_l,int t_l):from_line(f_l),to_line(t_l){};

void GOTO::execute(EvalState& state){
     //state.setValue("GOTO",line_number);
     state["GOTO"]=to_line;
     state["is_GOTO"]=from_line;
}


IF_THEN::IF_THEN(Expression* l,string op,Expression* r,int number,int f_l)
:lh(l),cmp(op),rh(r),to_line(number),from_line(f_l){}

void IF_THEN::execute(EvalState& state){
    int left=lh->eval(state);
    int right=rh->eval(state);
    bool judge=true;
    if(cmp=="=")judge=(left==right);
    else if(cmp==">")judge=(left>right);
    else if(cmp=="<")judge=(left<right);
    else error("SYNTAX ERROR");
    if(judge)//state.setValue("IF_THEN",line_number);
    {
        state["is_IF_THEN"]=from_line;
        state["IF_THEN"]=to_line;
    }
}