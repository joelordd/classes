// Audrey Cards 005980230
// CSE 330 Summer 2018
// Dr. Murphy 


#include <iostream>
#include <stack>

using namespace std;

//A function that figures out what prescedence the operators go in when added to the stack.
//@param check is the operator symbol that will be checked.
int prec(char check);

//A function that takes in an infix expression and converts it to an outfix expression.
int main() 
{
    cout << "Enter an expression\n";
    stack<char> operation;
    char ch;
    cin.get(ch);
    
    while(!cin.eof()) {
        if((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) 
            cout << ch;
        else {
            if(ch == '(')
                operation.push(ch);
            else if(ch == ')') {
                while(!operation.empty() && operation.top() != '(') {
                    cout << operation.top();
                    operation.pop();
                }
                if(!operation.empty())
                    operation.pop();
                else 
                    cout << "Error.  No Matching '('\n";
            }
            else if(ch == '+' || ch == '-' || ch == '*' || ch == '/') {
                if(operation.empty() || prec(operation.top()) < prec(ch))
                    operation.push(ch);
                else {
                    while(!operation.empty() && prec(operation.top()) >= prec(ch)) {
                        cout << operation.top();
                        operation.pop();
                    }
                    operation.push(ch);
                }
            }
        }
    cin.get(ch);
    }    
    while(!operation.empty()) {
        cout << operation.top();
        operation.pop();
    }
    
    cout << endl;
    return 0;
}

int prec(char check) {
    if(check == '+' || check == '-')
        return 1;
    else if(check == '*' || check == '/')
        return 2;
    else
        return 0;                                   //for errors and other operators
}

