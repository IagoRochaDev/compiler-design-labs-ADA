#include "ExpressaoMod.hpp"
#include <iostream>
#include <stdexcept>
#include "../debug-util.hpp"

using namespace std;
ExpressaoMod::ExpressaoMod() { }

ValorLiteral ExpressaoMod::avalia(TabelaSimbolos* memoria) {
  ValorLiteral left = esquerda->avalia(memoria);
  ValorLiteral right = direita->avalia(memoria);
  if (left.tipo == NULL || right.tipo == NULL) {
    throw runtime_error("Tipo invalido em operacao de modulo");
  }
  if (left.tipo->valor != Tipo::INT || right.tipo->valor != Tipo::INT) {
    throw runtime_error("Operacao modulo exige operandos inteiros");
  }
  if (right.valor_int == 0) {
    throw runtime_error("Modulo por zero");
  }
  ValorLiteral result;
  result.tipo = left.tipo;
  result.valor_int = left.valor_int % right.valor_int;
  return result;
}

void ExpressaoMod::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "% [ INI Mod"<< endl;
  esquerda->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "%" << endl;
  direita->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "% ] FIM Mod"<< endl;
}
