#ifndef _STM_SEQ_HPP_
#define _STM_SEQ_HPP_
#include "Stm.hpp"

class StmSeq : public Stm {
public:
  Stm* esq;
  Stm* dir;
  StmSeq();
  StmSeq(Stm* esquerda, Stm* direita);
  static StmSeq* gera_Stm_Seq(/*Adaptar aas necessidades.*/);
};

#endif
