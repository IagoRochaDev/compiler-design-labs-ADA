#include "ExpressaoMenor.hpp"
#include <iostream>
#include <stdexcept>
#include "../debug-util.hpp"

using namespace std;

ExpressaoMenor::ExpressaoMenor() { }

ValorLiteral ExpressaoMenor::avalia(TabelaSimbolos* memoria) {
  ValorLiteral left = esquerda->avalia(memoria);
  ValorLiteral right = direita->avalia(memoria);
  if (left.tipo == NULL || right.tipo == NULL) {
    throw runtime_error("Tipo invalido em comparacao menor");
  }
  if (left.tipo->valor == Tipo::BOOL || right.tipo->valor == Tipo::BOOL) {
    throw runtime_error("Comparacao menor invalida para tipo booleano");
  }
  ValorLiteral result;
  result.tipo = new Tipo(Tipo::BOOL);
  float l = left.tipo->valor == Tipo::FLOAT ? left.valor_float : (float) left.valor_int;
  float r = right.tipo->valor == Tipo::FLOAT ? right.valor_float : (float) right.valor_int;
  result.valor_bool = (l < r);
  return result;
}

void ExpressaoMenor::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "< [ INI Menor"<< endl;
  esquerda->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "<" << endl;
  direita->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "< ] FIM Menor"<< endl;
}
