// Name: Parmveer Dayal
// Date: August 5, 2022

#include "hash.h"
#include "Scanner.h"
#include "Parse.h"
#include "assert.h"
#include <cstdlib>
#include <iostream>
#include <math.h>

using namespace std;

#define tabstop 4

class var {
    public:
        string key;
       
        // * * * add some more attributes and methods here * * * //
        int value;
        var() { key = ""; }
        var(string key, int value) { 
            this->key = key; 
            this->value = value;
        } 
};

// * * * declare more things here * * * //

Set<var> memmap;
int run(StmtsNode *stmts);
void printFunction(StmtsNode *root);
void addToHash(StmtsNode *root);
int calculate(ExpnNode *root);
bool leafNode(StmtNode *root);
int makeInt(string str);


// the main function that will go through the entire tree
int run(StmtsNode *stmts) {
    StmtsNode *curr = stmts;

    // will continue to loop until the final node is hit in the tree
    while (curr != NULL) {
        // will call a different funtion depending on what the node is
        if (curr->stmt->tok == printtok) {
            printFunction(curr);
        } 
        else if (curr->stmt->tok == asgntok) {
            if (curr->stmt->ident != "") {
                addToHash(curr);
            }
        } 
        else if (curr->stmt->tok == iftok) {
            int if_value = calculate(curr->stmt->expn);

            // if/elif/else
            if (if_value == 1) {
                run(curr->stmt->stmts);
            } 
            else if (curr->stmt->elif != NULL) {
                StmtNode *temp = curr->stmt->elif;
                while (temp != NULL) {
                    int elif_value = calculate(temp->expn);

                    if (elif_value == 1) {
                        run(temp->stmts);
                        break;
                    } 
                    else {
                        temp = temp->elif;
                    }
                }
            }
        } 
        else if (curr->stmt->tok == whiletok) {
            int while_check = calculate(curr->stmt->expn);

            while (while_check == 1) {
                run(curr->stmt->stmts);
                while_check = calculate(curr->stmt->expn);
            }
        }
        curr = curr->stmts;
    }
    return 0;
}


// function called when root is printtok, in order to print
void printFunction(StmtsNode *root) {
    if (leafNode(root->stmt) == true) {
        if (root->stmt->expn->tok == ident) {
            if (memmap.search(root->stmt->expn->text) == NULL) {
                // throw error if variable not present inside of hash table
                throw std::runtime_error("Variable could not be found.");
            }
            // else, print the value stored inside of the targeted object in hash table
            cout << memmap.search(root->stmt->expn->text)->value << endl;
        } 
        else {
            // if root not variable, print text
            cout << root->stmt->expn->text << endl;
        }
    } 
    // if not leaf node, then call calculate on the expn and print it
    else {
        int value = calculate(root->stmt->expn);
        cout << value << endl;
    }
}


// function called to insert new object into the hash table
void addToHash(StmtsNode *root) {
    int val = calculate(root->stmt->expn);
    string id = root->stmt->ident;

    var* hash = new var(id, val);
    int pos = memmap.insert(hash);
}


// function recursively called through tree to calculate right and left values, then return
int calculate(ExpnNode* root) {
    if (root == NULL) {
        return 0;
    }

    if (root->left_operand == NULL && root->right_operand == NULL){
        if (root->tok == ident) {
            if (memmap.search(root->text) == NULL) {
                // if root is a variable with no children and not in the hash table, throw error
                throw std::runtime_error("Variable not inside of hash.");
            }
            // return value of variable if found
            return memmap.search(root->text)->value;
        } 
        else {
            // else turn string into an int
            return makeInt(root->text);
        }
    }

    // recursively find the left value
    int left = calculate(root->left_operand);

    // Short-Circuit Evaluation
    if (root->tok == asttok && left == 0) {
        return 0;
    }

    if (root->tok == slashtok && left == 0) {
        return 0;
    }

    if (root->tok == ortok && left == 1) {
        return 1;
    }

    if (root->tok == andtok && left == 0) {
        return 0;
    }

    // recursively find the right value
    int right = calculate(root->right_operand);

    // returns the values depending on what calculation it is
    if (root->tok == pltok) {
        return left + right;
    } 
    else if (root->tok == mitok) {
        // if root does not have a left child then right value is negative
        if (root->left_operand == NULL) {
            return (-1) * right;
        } 
        else {
            return left - right;
        }
    } 
    else if (root->tok == asttok) {
        return left * right;
    } 
    else if (root->tok == slashtok) {
        // if the divisor is 0, throw a runtime error
        if (right == 0) {
            throw std::runtime_error("Undefined. Divisor is 0.");
        }
        return floor((double)left / (double)right);
    } 
    // returns true or false depending on what comparison it is and what the values are
    else if (root->tok == lttok) {
        return left < right;
    } 
    else if (root->tok == gttok) {
        return left > right;
    } 
    else if (root->tok == eqtok) {
        return left == right;
    } 
    else if (root->tok == netok) {
        return left != right;
    } 
    else if (root->tok == letok) {
        return left <= right;
    } 
    else if (root->tok == getok) {
        return left >= right;
    } 
    else if (root->tok == andtok) {
        return left && right;
    } 
    else if (root->tok == ortok) {
        return ((left == 1) || (right == 1));
    } 
    else if (root->tok == nottok) {
        if (root->left_operand == NULL) {
            if (right == 1) {
                return 0;
            } 
            else if (right == 0) {
                return 1;
            }
        }
    }
    return 0;
}


// return true if node is a leaf, else false
bool leafNode(StmtNode *root) {
    return (root->expn->left_operand == NULL && root->expn->right_operand == NULL);
}


// Change the string to an int and return it
int makeInt(string str) {
    int sum = 0;

    // if not negative
    if (str[0] != '-') {
        for (int i = 0; i < str.length(); i++) {
            sum = sum * 10 + (int(str[i]) - 48);
        }
    } 
    else {
        for (int i = 1; i < str.length(); i++) {
            sum = sum * 10 + (int(str[i]) - 48);
            // make it negative
            sum = sum * (-1);
        }
    }
    return sum;
}


///////////////////////////////////////////////
//----- carried over from testParse.cpp -----//
///////////////////////////////////////////////

// Desc:  These 3 functions return a string which is a pretty-printed
//        version of the subtree rooted there.
string printE(ExpnNode *root);
string printStmts(StmtsNode *root, int ilevel);
string print(StmtsNode *root);


// Desc:  Generate pretty printed pish of the parse tree rooted at root.
string print(StmtsNode *root) {
    return printStmts(root, 0);  // call helper
}


// Desc:  Generate pretty printed pish, by traversing like a linked list.
//        Parameter ilevel is the indentation level.
string printStmts(StmtsNode *root, int ilevel) {
    // generate leading tab
    string spc = "", tab = "";
    for (int i = 0; i < tabstop; i++) spc += " ";
    for (int i = 0; i < ilevel; i++) tab += spc;


    // traverse StmtsNode * like it is a linked list
    StmtsNode *cur = root;
    string accum = "";  // pretty-printed version


    while (cur != NULL) {
        assert(cur->stmt != NULL);

        // print statement
        if (cur->stmt->tok == printtok) {
            accum += tab + toktotext(printtok) + " " + printE(cur->stmt->expn) + toktotext(sctok) + "\n";
        }
        // assignment statement
        else if (cur->stmt->tok == asgntok) {
            accum += tab + cur->stmt->ident + " " + toktotext(asgntok) + " " + printE(cur->stmt->expn) + toktotext(sctok) + "\n";
        }
        // while statement: tab level increases for subStmts
        else if (cur->stmt->tok == whiletok) {
            accum += tab + toktotext(whiletok) + " " + printE(cur->stmt->expn) + " " + toktotext(lctok) + "\n" + printStmts(cur->stmt->stmts, ilevel+1) + tab + toktotext(rctok) + "\n";
        }
        // if/elif/else statement: tab level increases for subStmts
        else if (cur->stmt->tok == iftok) {
            StmtNode *elif = cur->stmt;
            while (elif != NULL) {
                accum += tab + toktotext(elif->tok) + ((elif->tok == elsetok) ? "" : (" " + printE(elif->expn))) + " " + toktotext(lctok) + "\n" + printStmts(elif->stmts, ilevel+1) + tab + toktotext(rctok) + "\n";
                elif = elif->elif;
            }
        }
        // break statement
        else if (cur->stmt->tok == breaktok) {
            accum += tab + toktotext(breaktok) + toktotext(sctok) + "\n";
        }
        else { assert(0); }


        // next node in the chain
        cur = cur->stmts;
    }
    return accum;
}


// Desc:  Generate bracketed infix via postorder traversal
string printE(ExpnNode *root) {
    if (root == NULL) return "";
    if (root->text != "") return root->text;
    if (root->left_operand == NULL) return "(" + toktotext(root->tok) + ((root->tok == nottok) ? " " : "") + printE(root->right_operand) + ")";
    else return "(" + printE(root->left_operand) + " " + toktotext(root->tok) + " " + printE(root->right_operand) + ")";
}


int main () {
    StmtsNode *pTree;
    try {
        pTree = Parse(cin);
    }
    catch(string s) {
        cout << "Parse error: " << s << endl;
        return 0;
    }

    cout << "Parse Successful!\n";

    try{
       run(pTree); 
    } 
    catch(exception &e) {
        cout << "Runtime Error! " << e.what() << endl;
    }
}
