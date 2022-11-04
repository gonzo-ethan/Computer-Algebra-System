#include "ExpressionTree.hpp"
#include <iostream>

int main() {
    ExpressionTree* myTree = new ExpressionTree();

    myTree->defaultOps();

    myTree->buildTree("((((a * b) * d) + ((a * c) * d)) + (((a * b) * e) + ((a * c) * e)))");

    myTree->expand();

    std::cout << "Expanded: " << myTree->toString() << std::endl;

    myTree->simplify();

    std::cout << "Simplified:" << myTree->toString() << std::endl;

    //delete myTree; // EDIT: fix malloc 'pointer being frred was not allocated' error

    return 0;
}