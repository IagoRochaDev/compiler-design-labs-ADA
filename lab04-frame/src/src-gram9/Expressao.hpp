#ifndef _EXPRESSAO_HPP_
#define _EXPRESSAO_HPP_
#include "ValorLiteral.hpp"
#include "../Arvore.hpp"
using namespace std;

class TabelaSimbolos;

class Expressao {
public:
  static Expressao* extrai_expressao(No_arv_parse* no);
  virtual ValorLiteral avalia(TabelaSimbolos* memoria) = 0;
  virtual void debug_com_tab(int tab);
  virtual ~Expressao();
};

#endif
