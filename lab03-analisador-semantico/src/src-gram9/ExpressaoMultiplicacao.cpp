#include "ExpressaoMultiplicacao.hpp"
#include <iostream>
#include <stdexcept>
#include "../debug-util.hpp"

using namespace std;
ExpressaoMultiplicacao::ExpressaoMultiplicacao() { }

ValorLiteral ExpressaoMultiplicacao::avalia(TabelaSimbolos* memoria) {
  ValorLiteral left = esquerda->avalia(memoria);
  ValorLiteral right = direita->avalia(memoria);
  if (left.tipo == NULL || right.tipo == NULL) {
    throw runtime_error("Tipo invalido em operacao de multiplicacao");
  }
  if (left.tipo->valor == Tipo::BOOL || right.tipo->valor == Tipo::BOOL) {
    throw runtime_error("Operacao de multiplicacao invalida para tipo booleano");
  }
  if (left.tipo->valor == Tipo::FLOAT || right.tipo->valor == Tipo::FLOAT) {
    ValorLiteral result;
    result.tipo = new Tipo(Tipo::FLOAT);
    float l = left.tipo->valor == Tipo::FLOAT ? left.valor_float : (float) left.valor_int;
    float r = right.tipo->valor == Tipo::FLOAT ? right.valor_float : (float) right.valor_int;
    result.valor_float = l * r;
    return result;
  }
  ValorLiteral result;
  result.tipo = left.tipo;
  result.valor_int = left.valor_int * right.valor_int;
  return result;
}

void ExpressaoMultiplicacao::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "* [ INI Multiplicacao"<< endl;
  esquerda->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "*" << endl;
  direita->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "* ] FIM Multiplicao"<< endl;
}
