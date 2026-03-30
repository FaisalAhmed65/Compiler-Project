#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "codegen.h"

/* Run all optimization passes on the TAC list.
   Returns the (possibly modified) head of the list.          */
TAC *optimize_tac(TAC *list);

#endif /* OPTIMIZER_H */
