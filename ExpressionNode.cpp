#include "ExpressionNode.hpp"

// Constructors for expression node
ExpressionNode::ExpressionNode(ExpressionNode* p, std::string nodeType) : nodeType(nodeType), parent(p) {}

ExpressionNode::ExpressionNode(std::string nodeType) : nodeType(nodeType), parent(nullptr) {}

// Constructors for operation node
OperationNode::OperationNode(AlgebraOperation* op, OperationNode* p, ExpressionNode* l, ExpressionNode* r) : ExpressionNode(p, "operation"), op(op), left(l), right(r) {}

OperationNode::OperationNode(AlgebraOperation* op) : ExpressionNode("operation"), op(op) {}

// Constructors for operand node
OperandNode::OperandNode(AlgebraOperand* op, OperationNode* p) :  ExpressionNode(p, "operand"), op(op) {}

OperandNode::OperandNode(AlgebraOperand* op) : ExpressionNode("operand"), op(op) {}