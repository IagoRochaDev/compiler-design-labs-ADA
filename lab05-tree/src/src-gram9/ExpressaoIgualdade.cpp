#include "ExpressaoIgualdade.hpp"
#include <iostream>
#include <stdexcept>
#include "../debug-util.hpp"

using namespace std;

ExpressaoIgualdade::ExpressaoIgualdade() { }

ValorLiteral ExpressaoIgualdade::avalia(TabelaSimbolos* memoria) {
  ValorLiteral left = esquerda->avalia(memoria);
  ValorLiteral right = direita->avalia(memoria);
  if (left.tipo == NULL || right.tipo == NULL) {
    throw runtime_error("Tipo invalido em comparacao de igualdade");
  }
  ValorLiteral result;
  result.tipo = new Tipo(Tipo::BOOL);

  if (left.tipo->valor == Tipo::BOOL && right.tipo->valor == Tipo::BOOL) {
    result.valor_bool = (left.valor_bool == right.valor_bool);
    return result;
  }

  if ((left.tipo->valor == Tipo::INT || left.tipo->valor == Tipo::FLOAT) &&
      (right.tipo->valor == Tipo::INT || right.tipo->valor == Tipo::FLOAT)) {
    float l = left.tipo->valor == Tipo::FLOAT ? left.valor_float : (float) left.valor_int;
    float r = right.tipo->valor == Tipo::FLOAT ? right.valor_float : (float) right.valor_int;
    result.valor_bool = (l == r);
    return result;
  }

  throw runtime_error("Comparacao de igualdade invalida para tipos " + left.tipo->nome() + " e " + right.tipo->nome());
}

void ExpressaoIgualdade::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "== [ INI Igualdade"<< endl;
  esquerda->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "==" << endl;
  direita->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "== ] FIM Igualdade"<< endl;
}
