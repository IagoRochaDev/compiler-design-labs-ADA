#ifndef _EXP_HPP_
#define _EXP_HPP_
#include "../Expressao.hpp"
using namespace std;

class Exp {
public:
  Exp();
  virtual ~Exp() = default;
  static Exp* gera_Exp_de_Expressao(Expressao* e);
};

#endif
