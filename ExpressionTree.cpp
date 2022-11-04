#include "ExpressionTree.hpp"

// Constructor for tree
ExpressionTree::ExpressionTree() : root(nullptr) {}

// Destructor for tree
ExpressionTree::~ExpressionTree() {
    destroyNode(root);
}

// Recursive function to delete all nodes
void ExpressionTree::destroyNode(ExpressionNode* currNode) {
    if (currNode->nodeType == "operation") {
        std::cout << "DEBUG   op destroyed: " << ((OperationNode*)currNode)->op->getRep() << std::endl;
        destroyNode(((OperationNode*)currNode)->left);
        destroyNode(((OperationNode*)currNode)->right);
    } else {
        std::cout << "DEBUG   op destroyed: " << ((OperandNode*)currNode)->op->getRep() << std::endl;
    }


    delete currNode;
}

// Returns the root of the tree
ExpressionNode* ExpressionTree::getRoot() {
    return root;
}

// Formats the input string to get an acceptable string to decode into expression tree
std::string ExpressionTree::formatExpression(const std::string inputStr) {
    std::string formatStr = "";

    if (inputStr.length() == 1)
        return inputStr;
    
    // Removes spacing from string and ensures that string contains valid operations and operands
    for (int i = 0; i < inputStr.length(); i++) {
        if (inputStr[i] == ' ')
            continue;

        if ((inputStr[i] == '(') || (inputStr[i] == ')')) {
            formatStr += inputStr[i];
            continue;
        }

        if ((validOps.find(inputStr[i]) != validOps.end()) || (validVars.find(inputStr[i]) != validVars.end()))
            formatStr += inputStr[i];
        else {
            std::cout << "FORMAT_ERR   invalid operation/operand" << std::endl;
            throw; // EDIT: implement exception for throw
        }
    }

    int leftParCount = 0;
    int rightParCount = 0;
    int opCount = 0;
    int varCount = 0;

    for (int i = 0; i < inputStr.length(); i++) {
        if (inputStr[i] == '(') {
            leftParCount++; // Increment left parenthesis count by one when left parenthesis read
            continue;
        }
        
        if (inputStr[i] == ')') {
            rightParCount++; // Increment right parenthesis count by one when right parenthesis read
            continue;
        }

        if (validOps.find(inputStr[i]) != validOps.end()) {
            opCount++; // Increment operation count by one when operation read
            continue;
        }
        
        if (validVars.find(inputStr[i]) != validVars.end())
            varCount++; // Increment operand (variable) count by one when operand read
    }

    // Throws error if the parenthesis count or operation count is invalid
    if ((leftParCount != rightParCount) || (opCount - 1 > leftParCount)) {
        std::cout << "FORMAT_ERR   invalid parenthesis/operation count" << std::endl;
        throw; // EDIT: implement exceptions for throw
    }
    
    // Sheds extraneous outer parenthesis from string
    for (int i = 0; i < (leftParCount - (opCount - 1)); i++)
        formatStr = formatStr.substr(1, formatStr.length() - 2);
    
    return formatStr;
}

// Recursive method which builds tree by expression nodes
ExpressionNode* ExpressionTree::buildTree(const std::string inputStr) {
    int netPar = 0;
    static bool isRoot = true;

    std::string expressionStr = formatExpression(inputStr);

    if (expressionStr.length() == 1)
        return (new OperandNode(new AlgebraOperand(expressionStr[0])));
    
    for (int i = 0; i < expressionStr.length(); i++) {
        if (expressionStr[i] == '(')
            netPar++;
        
        if (expressionStr[i] == ')')
            netPar--;
        
        if ((validOps.find(expressionStr[i]) != validOps.end()) && (netPar == 0)) {
            std::string leftStr = formatExpression(expressionStr.substr(0, i));
            std::string rightStr = formatExpression(expressionStr.substr(i + 1));

            if (isRoot) {
                root = new OperationNode(validOps[expressionStr[i]], nullptr, buildTree(leftStr), buildTree(rightStr));
                ((OperationNode*)(root))->left->parent = root;
                ((OperationNode*)(root))->right->parent = root;
                isRoot = false;
                return root;
            }

            OperationNode* newNode = new OperationNode(validOps[expressionStr[i]], nullptr, buildTree(leftStr), buildTree(rightStr));
            newNode->left->parent = newNode;
            newNode->right->parent = newNode;

            return newNode;
        }
    }

    return (new ExpressionNode()); // Returns empty node if error occurs
}

// Compare two nodes to see if the nodes/subtrees are equal
bool ExpressionTree::equalNodes(ExpressionNode* node1, ExpressionNode* node2) {
    if (node1->nodeType != node2->nodeType)
        return false;
    
    if (node1->nodeType == "operand")
        return (((OperandNode*)node1)->op->getRep() == ((OperandNode*)node2)->op->getRep());
    
    if (node1->nodeType == "operation") {
        if (((OperationNode*)node1)->op->getRep() != ((OperationNode*)node2)->op->getRep())
            return false;
        
        return (equalNodes(((OperationNode*)node1)->left, ((OperationNode*)node2)->left) && equalNodes(((OperationNode*)node1)->right, ((OperationNode*)node2)->right));
    }

    std::cout << "FATAL ERROR   equalNodes" << std::endl;
    return false; // Should not trigger unless error
}

// Simplifies the expression tree (user-accessable)
void ExpressionTree::simplify() {
    if (root->nodeType == "operation")
        simplify((OperationNode*)root);
}

// Expands the expression tree (user-accessable)
void ExpressionTree::expand() {
    if (root->nodeType == "operation")
        expand((OperationNode*)root);
}

// Recursive method that checks nodes to see if they can be simplified
void ExpressionTree::simplify(OperationNode* currNode) {
    // Check for identities or inverses
    if (currNode->nodeType == "operation") {
        if (currNode->right->nodeType == "operand") {
            if (((OperandNode*)(currNode->right))->op->getRep() == currNode->op->getIdentity().getRep()) {
                std::cout << "IDENTITY SIMP" << std::endl;
            }
        }
    }

    // Check for factoring
    if ((currNode->left->nodeType == "operation") && (currNode->right->nodeType == "operation")) {
        if (((OperationNode*)(currNode->left))->op == ((OperationNode*)(currNode->right))->op) {
            if (((OperationNode*)(currNode->left))->op->doesDist(currNode->op)) {
                if (equalNodes(((OperationNode*)(currNode->left))->left, ((OperationNode*)(currNode->right))->left)) {
                    OperationNode* newRight = new OperationNode(currNode->op, currNode, ((OperationNode*)(currNode->left))->right, ((OperationNode*)(currNode->right))->right);
                    currNode->op = ((OperationNode*)(currNode->left))->op;
                    currNode->left = ((OperationNode*)(currNode->left))->left;
                    currNode->right = newRight;
                    simplify((OperationNode*)root);
                    return;
                }

                if (equalNodes(((OperationNode*)(currNode->left))->right, ((OperationNode*)(currNode->right))->right)) {
                    OperationNode* newLeft = new OperationNode(currNode->op, currNode, ((OperationNode*)(currNode->left))->left, ((OperationNode*)(currNode->right))->left);
                    currNode->op = ((OperationNode*)(currNode->right))->op;
                    currNode->right = ((OperationNode*)(currNode->right))->right;
                    currNode->left = newLeft;
                    simplify((OperationNode*)root);
                    return;
                } 
            }
        }
    }

    if (currNode->left->nodeType == "operation")
        simplify((OperationNode*)(currNode->left));
    
    if (currNode->right->nodeType == "operation")
        simplify((OperationNode*)(currNode->right));
}

// Recursive method that checks nodes to see if they can be expanded
void ExpressionTree::expand(OperationNode* currNode) {
    if (currNode->right->nodeType == "operation") {
        if (currNode->op->doesDist(((OperationNode*)(currNode->right))->op)) {
            OperationNode* newLeft = new OperationNode(currNode->op, currNode, currNode->left, ((OperationNode*)(currNode->right))->left);
            OperationNode* newRight = new OperationNode(currNode->op, currNode, currNode->left, ((OperationNode*)(currNode->right))->right);
            currNode->op = ((OperationNode*)(currNode->right))->op;
            currNode->left = newLeft;
            currNode->right = newRight;
            expand((OperationNode*)root);
            return;
        }
    }
    
    if (currNode->left->nodeType == "operation") {
        if (currNode->op->doesDist(((OperationNode*)(currNode->left))->op)) {
            OperationNode* newLeft = new OperationNode(currNode->op, currNode, ((OperationNode*)(currNode->left))->left, currNode->right);
            OperationNode* newRight = new OperationNode(currNode->op, currNode, ((OperationNode*)(currNode->left))->right, currNode->right);
            currNode->op = ((OperationNode*)(currNode->left))->op;
            currNode->left = newLeft;
            currNode->right = newRight;
            expand((OperationNode*)root);
            return;
        }
    }

    if (currNode->left->nodeType == "operation")
        expand((OperationNode*)(currNode->left));
    
    if (currNode->right->nodeType == "operation")
        expand((OperationNode*)(currNode->right));
}

// Adds valid operation to tree
void ExpressionTree::addOperation(AlgebraOperation* newOp) {
    validOps[newOp->getRep()] = newOp;
}

// Adds valid operand to tree
void ExpressionTree::addOperand(AlgebraOperand* newOp) {
    validVars[newOp->getRep()] = newOp;
}

// Loads some of the default operations and lowercase alphabetical operands into the tree
void ExpressionTree::defaultOps() {
    AlgebraOperation* add = new AlgebraOperation('+', true, true);
    AlgebraOperation* sub = new AlgebraOperation('-', false, false);

    std::map<char, AlgebraOperation*> multDistOps;

    multDistOps['+'] = add;
    multDistOps['-'] = sub;

    AlgebraOperation* mult = new AlgebraOperation('*', true, true, multDistOps);
    AlgebraOperation* div = new AlgebraOperation('/', false, false);

    validOps.clear();

    validOps['+'] = add;
    validOps['-'] = sub;
    validOps['*'] = mult;
    validOps['/'] = div;

    for (char i = 'a'; i <= 'z'; i++)
        validVars[i] = new AlgebraOperand(i);
}

// toString method in the case that no parameter is applied (user-accessable)
std::string ExpressionTree::toString() {
    if (root->nodeType == "operation")
        return toString((OperationNode*)root);
    else
        return toString((OperandNode*)root);
}

// Recursive function for converting tree to string if parameter is operation node
std::string ExpressionTree::toString(OperationNode* currNode) {
    std::string treeStr = "";

    if (validOps.find(currNode->op->getRep()) != validOps.end()) {
        treeStr += '(';
        // If left node is operation node then call toString(operationNode*), otherwise call toString(operandNode*)
        treeStr += (currNode->left->nodeType == "operation") ? toString((OperationNode*)(currNode->left)) : toString((OperandNode*)(currNode->left));
        treeStr += ' ';
        treeStr += currNode->op->getRep(); // Insert operation in center of string
        treeStr += ' ';
        // If right node is operation node then call toString(operationNode*), otherwise call toString(operandNode*)
        treeStr += (currNode->right->nodeType == "operation") ? toString((OperationNode*)(currNode->right)) : toString((OperandNode*)(currNode->right));
        treeStr += ')';
    }

    return treeStr;
}

// Returns the character representative of the operand
std::string ExpressionTree::toString(OperandNode* currNode) {
    std::string treeStr = "";

    treeStr += currNode->op->getRep();

    return treeStr;
}
