/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface specified in Chapter 17, which is an
 * excellent model for the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include "evalstate.h"
#include "exp.h"

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

   Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

   virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

   virtual void execute(EvalState & state) = 0;

};

/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */
class REM:public Statement{
public:
    void execute(EvalState & state);//注释就是什么都不做
};

class LET:public Statement{
public:
    LET(Expression* _exp);
    void execute(EvalState & state);
private:
    Expression* exp;
};

class PRINT:public Statement{
public:
    PRINT(Expression* _exp);
    void execute(EvalState& state);
private:
    Expression* exp;
};

class INPUT:public Statement{
public:
    INPUT(string s);
    void execute(EvalState& state);
private:
    string var;
    int value;
};

class END:public Statement{
public:
    void execute(EvalState& state);
};

class GOTO:public Statement{
public:
    GOTO(int f_l,int t_l);
    void execute(EvalState& state);
private:
int to_line;
int from_line;
};

class IF_THEN:public Statement{
public:
    IF_THEN(Expression* l,string op,Expression* r,int number,int f_l);
    void execute(EvalState& state);
private:
    Expression* lh;
    Expression* rh;
    string cmp;
    int to_line;
    int from_line;
};
#endif
