#include "ExpressaoNegacao.hpp"
#include <iostream>
#include <stdexcept>
#include "../debug-util.hpp"

using namespace std;

ExpressaoNegacao::ExpressaoNegacao() : expressao(NULL), eh_not(false) { }

ValorLiteral ExpressaoNegacao::avalia(TabelaSimbolos* memoria) {
  ValorLiteral value = expressao->avalia(memoria);
  if (value.tipo == NULL) {
    throw runtime_error("Tipo invalido em operacao unaria");
  }
  if (eh_not) {
    if (value.tipo->valor != Tipo::BOOL) {
      throw runtime_error("Operador NOT exige operando booleano");
    }
    ValorLiteral result;
    result.tipo = value.tipo;
    result.valor_bool = !value.valor_bool;
    return result;
  }
  if (value.tipo->valor == Tipo::BOOL) {
    throw runtime_error("Negacao aritmetica exige operando numerico");
  }
  if (value.tipo->valor == Tipo::FLOAT) {
    ValorLiteral result;
    result.tipo = value.tipo;
    result.valor_float = -value.valor_float;
    return result;
  }
  ValorLiteral result;
  result.tipo = value.tipo;
  result.valor_int = -value.valor_int;
  return result;
}

void ExpressaoNegacao::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "! [ INI Negacao"<< endl;
  expressao->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "! ] FIM Negacao"<< endl;
}
