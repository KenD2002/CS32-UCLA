#include "Set.h"  // with ItemType being a type alias for char
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

bool syntax(string infix); // check the syntax of infix string
void infixToPostfix(string infix, string& postfix);
bool evalPostfix(string postfix, const Set& trueValues);

int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result)
{
    if(!syntax(infix))
    {
        return 1;
    }
    
    for(int i = 0; i < infix.size(); i ++)
    {
        if(islower(infix.at(i)))
        {
            if((!trueValues.contains(infix.at(i))) && (!falseValues.contains(infix.at(i))))
            {
                return 2;
            }
            if(trueValues.contains(infix.at(i)) && falseValues.contains(infix.at(i)))
            {
                return 3;
            }
        }
    }
    
    infixToPostfix(infix, postfix);
    
    result = evalPostfix(postfix, trueValues);
    
    return 0;
}

bool syntax(string infix)  // check the syntax of infix string
{
    std::string infixNoSpace;
    for(int i = 0; i < infix.size(); i ++)
    {
        if(infix.at(i) != ' ')
        {
            infixNoSpace += infix.at(i);
        }
    }
    
    // infix cant be empty
    if(infixNoSpace.size() == 0)
    {
        return false;
    }
    
    // infix can only contain specific characters
    char ch;
    for(int i = 0; i < infixNoSpace.size(); i ++)
    {
        ch = infixNoSpace.at(i);
        if(!(ch == '!' || ch == '&' || ch == '|' || ch == '(' || ch == ')' || islower(ch)))
        {
            return false;
        }
    }
    
    // only '(' and operands and '!' can be at the beginning
    if(infixNoSpace.at(0) != '(' && !islower(infixNoSpace.at(0)) && infixNoSpace.at(0) != '!')
    {
        return false;
    }
    
    // only ')' and operands can be at the end
    if(infixNoSpace.at(infixNoSpace.size() - 1) != ')' && !islower(infixNoSpace.at(infixNoSpace.size() - 1)))
    {
        return false;
    }
    
    // brackets must be in pairs
    int bracketsDiffNum = 0;
    for(int i = 0; i < infixNoSpace.size(); i ++)
    {
        ch = infixNoSpace.at(i);
        if(ch == '(')
        {
            bracketsDiffNum ++;
        }
        if(ch == ')')
        {
            bracketsDiffNum --;
        }
        if(bracketsDiffNum < 0)
        {
            return false;
        }
    }
    if(bracketsDiffNum != 0)
    {
        return false;
    }
    
    // there must be a '&' or '|' or ')' after a ')'
    for(int i = 0; i < infixNoSpace.size() - 1; i ++)
    {
        ch = infixNoSpace.at(i);
        if(ch == ')')
        {
            if(infixNoSpace.at(i + 1) != '&' && infixNoSpace.at(i + 1) != '|' && infixNoSpace.at(i + 1) != ')')
            {
                return false;
            }
        }
    }
    
    // there must be a ')' or '&' or '|' after an operand
    for(int i = 0; i < infixNoSpace.size() - 1; i ++)
    {
        ch = infixNoSpace.at(i);
        if(islower(ch))
        {
            if(infixNoSpace.at(i + 1) != ')' && infixNoSpace.at(i + 1) != '&' && infixNoSpace.at(i + 1) != '|')
            {
                return false;
            }
        }
    }
    // there must be a '!' or '(' or operand after a '!' or '(' or '&' or '|'
    for(int i = 0; i < infixNoSpace.size() - 1; i ++)
    {
        ch = infixNoSpace.at(i);
        if(ch == '!' || ch == '(' || ch == '&' || ch == '|')
        {
            if(infixNoSpace.at(i + 1) != '!' && infixNoSpace.at(i + 1) != '(' && (!islower(infixNoSpace.at(i + 1))))
            {
                return false;
            }
        }
    }
    
    return true;
}

void infixToPostfix(string infix, string& postfix)
{
    postfix = "";
    std::stack <char> operatorStack;
    for(int i = 0; i < infix.size(); i ++)
    {
        char ch = infix.at(i);
        switch (ch) {
            case '(':
                operatorStack.push(ch);
                break;
            case ')':
                while(operatorStack.top() != '(')
                {
                    postfix += operatorStack.top();
                    operatorStack.pop();
                }
                operatorStack.pop();
                break;
            case '!':
                operatorStack.push(ch);
                break;
                
            case '&':
                while(!operatorStack.empty() && operatorStack.top() != '(' && operatorStack.top() != '|')
                {
                    postfix += operatorStack.top();
                    operatorStack.pop();
                }
                operatorStack.push(ch);
                break;
            case '|':
                while(!operatorStack.empty() && operatorStack.top() != '(')
                {
                    postfix += operatorStack.top();
                    operatorStack.pop();
                }
                operatorStack.push(ch);
                break;
            case ' ': // if blank
                break;
            default: // if lower letter
                postfix += infix.at(i);
                break;
        }
    }
    while(!operatorStack.empty())
    {
        postfix += operatorStack.top();
        operatorStack.pop();
    }
}
       
bool evalPostfix(string postfix, const Set& trueValues)
{
    std::stack <char> operandStack;
    char operand1;
    char operand2;
    for(int i = 0; i < postfix.size(); i ++)
    {
        char ch = postfix.at(i);
        if(islower(ch))
        {
            if(trueValues.contains(ch))
            {
                operandStack.push('t');
            }
            else
            {
                operandStack.push('f');
            }
        }
        else if(ch == '!')
        {
            operand1 = operandStack.top();
            if(operand1 == '!')
            {
                operand1 = '\0';
            }
            else if(operand1 == 't')
            {
                operand1 = 'f';
            }
            else
            {
                operand1 = 't';
            }
            operandStack.pop();
            operandStack.push(operand1);
        }
        else if(ch == '&')
        {
            operand2 = operandStack.top();
            operandStack.pop();
            operand1 = operandStack.top();
            operandStack.pop();
            char result;
            if(operand2 == 't' && operand1 == 't')
            {
                result = 't';
            }
            else
            {
                result = 'f';
            }
            operandStack.push(result);
        }
        else
        {
            operand2 = operandStack.top();
            operandStack.pop();
            operand1 = operandStack.top();
            operandStack.pop();
            char result;
            if(operand2 == 'f' && operand1 == 'f')
            {
                result = 'f';
            }
            else
            {
                result = 't';
            }
            operandStack.push(result);
        }
    }
    if(operandStack.top() == 'f')
    {
        return false;
    }
    else
    {
        return true;
    }
}


int main()
       {
           string trueChars  = "tywz";
           string falseChars = "fnx";
           Set trues;
           Set falses;
           for (int k = 0; k < trueChars.size(); k++)
               trues.insert(trueChars[k]);
           for (int k = 0; k < falseChars.size(); k++)
               falses.insert(falseChars[k]);

           string pf;
           bool answer;
           assert(evaluate("w| f", trues, falses, pf, answer) == 0  &&  pf == "wf|" &&  answer);
           assert(evaluate("y|", trues, falses, pf, answer) == 1);
           assert(evaluate("n t", trues, falses, pf, answer) == 1);
           assert(evaluate("nt", trues, falses, pf, answer) == 1);
           assert(evaluate("()", trues, falses, pf, answer) == 1);
           assert(evaluate("()z", trues, falses, pf, answer) == 1);
           assert(evaluate("y(n|y)", trues, falses, pf, answer) == 1);
           assert(evaluate("t(&n)", trues, falses, pf, answer) == 1);
           assert(evaluate("(n&(t|y)", trues, falses, pf, answer) == 1);
           assert(evaluate("n+y", trues, falses, pf, answer) == 1);
           assert(evaluate("", trues, falses, pf, answer) == 1);
           assert(evaluate("f  |  !f & (t&n) ", trues, falses, pf, answer) == 0
                                  &&  pf == "ff!tn&&|"  &&  !answer);
           assert(evaluate(" x  ", trues, falses, pf, answer) == 0  &&  pf == "x"  &&  !answer);
           trues.insert('x');
           assert(evaluate("((x))", trues, falses, pf, answer) == 3);
           falses.erase('x');
           assert(evaluate("((x))", trues, falses, pf, answer) == 0  &&  pf == "x"  &&  answer);
           trues.erase('w');
           assert(evaluate("w| f", trues, falses, pf, answer) == 2);
           falses.insert('w');
           assert(evaluate("w| f", trues, falses, pf, answer) == 0  &&  pf == "wf|" &&  !answer);
           cout << "Passed all tests" << endl;
       }
