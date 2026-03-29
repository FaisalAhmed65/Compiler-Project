#ifndef OPTIMIZER_H
#define OPTIMIZER_H

/* ============================================================
   optimizer.h  –  TAC optimization passes
   TS2C Compiler  |  TypeScript → C translator
   ============================================================
   Optimization techniques implemented:
     1. Constant Folding    – evaluate constant expressions at
                             compile time (e.g. 3 + 4  →  7)
     2. Constant Propagation– replace variable uses with their
                             known constant values
     3. Dead Code Elimination – remove assignments to variables
                             that are never used afterwards
     4. Copy Propagation    – replace  x = y; ... use(x) with
                             use(y) where safe
   ============================================================ */

#include "codegen.h"

/* Run all optimization passes on the TAC list.
   Returns the (possibly modified) head of the list.          */
TAC *optimize_tac(TAC *list);

#endif /* OPTIMIZER_H */
