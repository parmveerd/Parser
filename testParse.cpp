
#include "Scanner.h"
#include "Parse.h"
#include <assert.h>

#include <iostream>
using namespace std;

#define tabstop 4


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
    cout << print(pTree); 
}

