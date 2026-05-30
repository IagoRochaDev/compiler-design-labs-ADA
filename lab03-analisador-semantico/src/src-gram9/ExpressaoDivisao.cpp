#include "ExpressaoDivisao.hpp"
#include <iostream>
#include <stdexcept>
#include "../debug-util.hpp"

using namespace std;

ExpressaoDivisao::ExpressaoDivisao() { }

ValorLiteral ExpressaoDivisao::avalia(TabelaSimbolos* memoria) {
  ValorLiteral left = esquerda->avalia(memoria);
  ValorLiteral right = direita->avalia(memoria);
  if (left.tipo == NULL || right.tipo == NULL) {
    throw runtime_error("Tipo invalido em operacao de divisao");
  }
  if (left.tipo->valor == Tipo::BOOL || right.tipo->valor == Tipo::BOOL) {
    throw runtime_error("Operacao de divisao invalida para tipo booleano");
  }
  if (right.tipo->valor == Tipo::INT && right.valor_int == 0) {
    throw runtime_error("Divisao por zero");
  }
  if (right.tipo->valor == Tipo::FLOAT && right.valor_float == 0.0f) {
    throw runtime_error("Divisao por zero");
  }
  if (left.tipo->valor == Tipo::FLOAT || right.tipo->valor == Tipo::FLOAT) {
    ValorLiteral result;
    result.tipo = new Tipo(Tipo::FLOAT);
    float l = left.tipo->valor == Tipo::FLOAT ? left.valor_float : (float) left.valor_int;
    float r = right.tipo->valor == Tipo::FLOAT ? right.valor_float : (float) right.valor_int;
    result.valor_float = l / r;
    return result;
  }
  ValorLiteral result;
  result.tipo = left.tipo;
  result.valor_int = left.valor_int / right.valor_int;
  return result;
}

void ExpressaoDivisao::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "/ [ INI Divisao"<< endl;
  esquerda->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "/" << endl;
  direita->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "/ ] FIM Divisao"<< endl;
}
