#ifndef _STM_EXP_HPP_
#define _STM_EXP_HPP_
#include "Stm.hpp"
#include "Exp.hpp"

class StmExp : public Stm {
public:
  Exp* exp;
  StmExp() : exp(nullptr) {}
  static StmExp* gera_Stm_Exp(/*Adaptar aas necessidades.*/);
};

#endif
