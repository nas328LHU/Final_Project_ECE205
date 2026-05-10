#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <cctype>
#include <stdexcept>

using namespace std;

// Basis for building the Tree


struct TreeNode {
    string value;
    TreeNode* left;
    TreeNode* right;

    TreeNode(string val) : value(val), left(nullptr), right(nullptr) {}
};



// Function Tokenizer (clears space)


vector<string> tokenizer(const string& expr) {
    vector<string> tokens;
    string currentoke = "";
    for (char c : expr) {
        if (isspace(c)) continue;
        if (isdigit(c)) {
            currentoke += c;
        }
        else {
            if (!currentoke.empty()) {
                tokens.push_back(currentoke);
                currentoke = "";
            }
            tokens.push_back(string(1, c));
        }
    }
    if (!currentoke.empty()) {
        tokens.push_back(currentoke);
    }
    return tokens;
}


// Precedence formation


int get_precedence(const string& op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    return 0;
}

// Infix to postfix transformation 


vector<string> inFIX_to_postFIX(const vector<string>& tokens) {
    vector<string> postfix;
    stack<string> opStack;

    for (const string& token : tokens) {
        if (isdigit(token[0])) {
            postfix.push_back(token);
        }
        else if (token == "(") {
            opStack.push(token);
        }
        else if (token == ")") {
            while (!opStack.empty() && opStack.top() != "(") {
                postfix.push_back(opStack.top());
                opStack.pop();
            }
            if (!opStack.empty()) opStack.pop();
        }
        else {
            while (!opStack.empty() && opStack.top() != "(" &&
                get_precedence(opStack.top()) >= get_precedence(token)) {
                postfix.push_back(opStack.top());
                opStack.pop();
            }
            opStack.push(token);
        }
    }

    while (!opStack.empty()) {
        postfix.push_back(opStack.top());
        opStack.pop();
    }

    return postfix;
}


// Postfix to tree transformation


TreeNode* build_exp_tree(const vector<string>& postfix) {
    stack<TreeNode*> treeStack;

    for (const string& token : postfix) {
        if (isdigit(token[0])) {
            treeStack.push(new TreeNode(token));
        }
        else {
            TreeNode* node = new TreeNode(token);
            node->right = treeStack.top(); treeStack.pop();
            node->left = treeStack.top(); treeStack.pop();
            treeStack.push(node);
        }
    }
    return treeStack.top();
}

// postfix outputed expressionn


void print_postFIX(TreeNode* root) {
    if (!root) return;
    print_postFIX(root->left);
    print_postFIX(root->right);
    cout << root->value << " ";
}


// visual representation of the binary tree


void print_Tree(TreeNode* root, int depth = 0) {
    if (!root) return;

    for (int i = 0; i < depth; ++i) {
        cout << "  "; 
    }

    cout << root->value << "\n";
    print_Tree(root->left, depth + 1);
    print_Tree(root->right, depth + 1);
}


// stack evaluation of the tree

int stack_evaluation_tree(TreeNode* root) {
    if (!root) return 0;

    stack<TreeNode*> s1, s2;
    s1.push(root);

    while (!s1.empty()) {
        TreeNode* node = s1.top();
        s1.pop();
        s2.push(node);

        if (node->left) s1.push(node->left);
        if (node->right) s1.push(node->right);
    }

    stack<int> evalStack;

    while (!s2.empty()) {
        TreeNode* node = s2.top();
        s2.pop();

        if (isdigit(node->value[0])) {
            evalStack.push(stoi(node->value));
        }
        else {
            int Ropp = evalStack.top(); evalStack.pop();
            int Lopp = evalStack.top(); evalStack.pop();

            if (node->value == "+") evalStack.push(Lopp + Ropp);
            else if (node->value == "-") evalStack.push(Lopp - Ropp);
            else if (node->value == "*") evalStack.push(Lopp * Ropp);
            else if (node->value == "/") evalStack.push(Lopp / Ropp);
        }
    }
    return evalStack.top();
}


// tree cleaner to delete memory

void tree_cleaner(TreeNode* root) {
    if (!root) return;
    tree_cleaner(root->left);
    tree_cleaner(root->right);
    delete root;
}

int main() {
    string infixExpr = "( 16 + 2 ) * 5 - 28 / 4";
    cout << "Infix Expression:\n" << infixExpr << "\n\n";

    vector<string> tokens = tokenizer(infixExpr);
    vector<string> postfix = inFIX_to_postFIX(tokens);
    TreeNode* root = build_exp_tree(postfix);

    cout << "a) Postorder traversal of the binary tree:\n";
    print_postFIX(root);
    cout << "\n\n";


    cout << "b) Visual look of the binary tree:\n";
    print_Tree(root);
    cout << "\n";

    int result = stack_evaluation_tree(root);
    cout << "c) Evaluated result:\n" << result << "\n";

    tree_cleaner(root);

    return 0;
}
