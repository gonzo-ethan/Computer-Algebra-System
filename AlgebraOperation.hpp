#ifndef _ALGEBRA_OPERATION_HPP_
#define _ALGEBRA_OPERATION_HPP_

#include "AlgebraOperand.hpp"
#include <map>

class AlgebraOperation {
public:
    AlgebraOperation(char rep, bool a, bool c, std::map<char, AlgebraOperation*> d);
    AlgebraOperation(char rep, bool a, bool c);
    AlgebraOperation(char rep = '\0');

    void newDistOp(AlgebraOperation* distOp);
    bool doesDist(AlgebraOperation* distOp);
    char getRep();
    AlgebraOperand getIdentity() const;
private:
    char represent;
    bool associative;
    bool commutative;
    AlgebraOperand* identity;
    std::map<char, AlgebraOperation*> distOver;
    friend class OperationNode;
};

#endif