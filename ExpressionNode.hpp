#ifndef _EXPRESSION_NODE_HPP_
#define _EXPRESSION_NODE_HPP_

#include "AlgebraOperation.hpp"
#include "AlgebraOperand.hpp"
#include <string>

class ExpressionNode {
public:
    ExpressionNode(ExpressionNode* parent, std::string nodeType = "");
    ExpressionNode(std::string nodeType = "");
private:
    std::string nodeType;
    ExpressionNode* parent;
    friend class ExpressionTree;
};

class OperationNode : public ExpressionNode {
public:
    OperationNode(AlgebraOperation* op, OperationNode* p, ExpressionNode* l, ExpressionNode* r);
    OperationNode(AlgebraOperation* op);
private:
    AlgebraOperation* op;
    ExpressionNode* left;
    ExpressionNode* right;
    friend class ExpressionTree;
};

class OperandNode : public ExpressionNode {
public:
    OperandNode(AlgebraOperand* op, OperationNode* p);
    OperandNode(AlgebraOperand* op);
private:
    AlgebraOperand* op;
    friend class ExpressionTree;
};

#endif