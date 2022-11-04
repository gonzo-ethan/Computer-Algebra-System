#include "AlgebraOperand.hpp"

// Constructors for operand
AlgebraOperand::AlgebraOperand(char rep) : represent(rep) {}

// Returns character representative of operand
char AlgebraOperand::getRep() { return represent; }