#ifndef _ALGEBRA_OPERAND_HPP_
#define _ALGEBRA_OPERAND_HPP_

#include <map>

class AlgebraOperand {
public:
    AlgebraOperand(char rep = '\0');

    char getRep();
private:
    char represent; // The character that represents the operand
    AlgebraOperand* inverse;
    friend class OperandNode;
};

#endif