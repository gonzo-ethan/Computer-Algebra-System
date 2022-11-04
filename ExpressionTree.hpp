#ifndef _EXPRESSION_TREE_HPP_
#define _EXPRESSION_TREE_HPP_

#include "ExpressionNode.hpp"
#include <string>
#include <iostream> // For debugging purposes
#include <vector>
#include <map>

class ExpressionTree {
public:
    ExpressionTree();
    ~ExpressionTree();

    ExpressionNode* getRoot();
    ExpressionNode* buildTree(const std::string expressionStr);
    bool equalNodes(ExpressionNode* node1, ExpressionNode* node2);
    void simplify();
    void expand();
    void addOperation(AlgebraOperation* newOp);
    void addOperand(AlgebraOperand* newOp);
    void defaultOps(); // Loads default operations into the tree's valid operations map, along with lowercase characters into operands map
    std::string toString();
private:
    ExpressionNode* root;
    std::map<char, AlgebraOperation*> validOps;
    std::map<char, AlgebraOperand*> validVars;

    void destroyNode(ExpressionNode* currNode);
    std::string formatExpression(const std::string inputStr);
    void simplify(OperationNode* currNode);
    void expand(OperationNode* currNode);
    std::string toString(OperationNode* currNode);
    std::string toString(OperandNode* currNode);
};

#endif