#ifndef _STM_MOVE_HPP_
#define _STM_MOVE_HPP_
#include "Stm.hpp"
#include "Exp.hpp"

class StmMove : public Stm {
public:
  Exp* destino;
  Exp* origem;
  StmMove();
  StmMove(Exp* dest, Exp* orig);
  static StmMove* gera_Stm_Move(/*Adaptar aas necessidades.*/);
};

#endif
