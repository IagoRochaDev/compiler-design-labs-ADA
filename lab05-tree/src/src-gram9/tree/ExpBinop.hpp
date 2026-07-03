#ifndef _EXP_BINOP_HPP_
#define _EXP_BINOP_HPP_
#include "Exp.hpp"
#include "../Expressao.hpp"
#include "Operador.hpp"

class ExpBinop : public Exp {
public:
  Operador* op;
  Exp* esq;
  Exp* dir;  
  ExpBinop();
  ExpBinop(Operador* operador, Exp* esquerda, Exp* direita);
  static ExpBinop* gera_Exp_Binop(Expressao* e);
};

#endif
