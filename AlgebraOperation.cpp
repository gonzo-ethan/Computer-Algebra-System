#include "AlgebraOperation.hpp"

// Constructors for algebra operation
AlgebraOperation::AlgebraOperation(char rep, bool a, bool c, std::map<char, AlgebraOperation*> d) : represent(rep), associative(a), commutative(c), distOver(d) {}

AlgebraOperation::AlgebraOperation(char rep, bool a, bool c) : represent(rep), associative(a), commutative(c) {}

AlgebraOperation::AlgebraOperation(char rep) : represent(rep), associative(false), commutative(false) {}

// Adds new 'distributive over' operation to back of 'distOver' vector
void AlgebraOperation::newDistOp(AlgebraOperation* distOp) {
    distOver[distOp->represent] = distOp;
}

// Returns character representative of operation
char AlgebraOperation::getRep() { return represent; }

bool AlgebraOperation::doesDist(AlgebraOperation* distOp) {
    if (distOver.find(distOp->represent) != distOver.end())
        return true;
    
    return false;
}

AlgebraOperand AlgebraOperation::getIdentity() const {
    return *identity;
}
