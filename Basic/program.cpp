/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include <utility>
#include<list>
#include "statement.h"
using namespace std;

Program::Program(){
   // Replace this stub with your own code
}

Program::~Program() {
   // Replace this stub with your own code
   if(!v_line.empty()){
       for(auto iter=v_line.begin();iter!=v_line.end();++iter)
       {
           delete iter->Paresed;
       }
   }

}

bool Program::find(list<LINE>::iterator& iter,int line_Number) {
    for(iter=v_line.begin();iter!=v_line.end();++iter)
    {
        if(iter->line_number==line_Number)return true;
    }
    return false;
}

void Program::clear() {
   // Replace this stub with your own code
   if(v_line.empty())return;
   for(auto iter=v_line.begin();iter!=v_line.end();++iter)
   {
       delete iter->Paresed;
   }
   v_line.clear();
}

void Program::addSourceLine(int lineNumber, string line) {
   // Replace this stub with your own code
   LINE l(lineNumber,line);
   if(v_line.empty()){v_line.push_back(l);return;}//空program
   auto iter=v_line.begin();
   if(find(iter,lineNumber))//原program中存在
   {
       v_line.erase(iter);
       if(v_line.empty()){v_line.push_back(l);return;}
   }
   iter=v_line.begin();
   while(iter!=v_line.end()&&iter->line_number<lineNumber)++iter;
   v_line.insert(iter,l);
}

void Program::removeSourceLine(int lineNumber) {
   // Replace this stub with your own code
   auto iter=v_line.begin();
   if(find(iter,lineNumber))
   {
       delete iter->Paresed;
       v_line.erase(iter);
   }
}

string Program::getSourceLine(int lineNumber) {
   auto iter=v_line.begin();
   if(find(iter,lineNumber))
   {
       return iter->source;
   }
   return "";
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
   // Replace this stub with your own code
   auto iter=v_line.begin();
   if(find(iter,lineNumber))
   {
       if(iter->Paresed)
       {
           delete iter->Paresed;
           iter->Paresed=stmt;
       }
       else
       {
           iter->Paresed=stmt;
       }
       return;
   }
   ///此处尚未throw！
}

Statement *Program::getParsedStatement(int lineNumber) {
   auto iter=v_line.begin();
   if(find(iter,lineNumber))
   {
       return iter->Paresed;
   }
    return NULL;
}

int Program::getFirstLineNumber() {
   if(v_line.empty())return -1;
   return v_line.begin()->line_number;
}

int Program::getNextLineNumber(int lineNumber) {
   auto iter=v_line.begin();
   if(find(iter,lineNumber))
   {
       ++iter;
       if(iter==v_line.end())return -1;
       return iter->line_number;
   }
   return -1;
}
//从start开始运行到结束
void Program::RUN(int start,EvalState& state){
    if(v_line.empty())return;//程序为空
    auto iter=v_line.begin();
    if(find(iter,start))
    {
        while(iter!=v_line.end())//运行到结束
        {
            iter->Paresed->execute(state);//执行程序
            if(state.isDefined("END")){
                state.Delete("END");
                break;
            }//停止程序
            if(state.isDefined("GOTO"))
            {
                if(iter->line_number==state.getValue("is_GOTO"))
                {
                    if(!find(iter,state.getValue("GOTO")))
                    {
                        error("LINE NUMBER ERROR");
                    }//修改指针位置
                    continue;
                }
            }
            if(state.isDefined("IF_THEN"))
            {
                if(iter->line_number==state.getValue("is_IF_THEN"))
                {
                    if(!find(iter, state.getValue("IF_THEN")))
                        error("LINE NUMBER ERROR");
                    state.Delete("is_IF_THEN");
                    state.Delete("IF_THEN");
                    continue;
                }
            }
            ++iter;
        }
    }
}
void Program::LIST(EvalState &state) {
    if(v_line.empty())return;//程序为空
    for(auto & iter : v_line)
    {
        cout<<iter.source<<"\n";
    }
}

void Program::CLEAR(EvalState &state) {
    clear();
}

void Program::QUIT(EvalState& state){}