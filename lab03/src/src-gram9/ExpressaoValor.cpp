#include "ExpressaoValor.hpp"
#include <iostream>
#include <stdexcept>
#include "../debug-util.hpp"

using namespace std;

ExpressaoValor::ExpressaoValor() { }

ValorLiteral ExpressaoValor::avalia(TabelaSimbolos* /*memoria*/) {
  if (valor == NULL || valor->tipo == NULL) {
    throw runtime_error("Valor literal invalido");
  }
  return *valor;
}

void ExpressaoValor::debug_com_tab(int tab) {
  tab3(tab);
  cerr << (valor == NULL ? "VALOR_INVALIDO" : valor->como_string())
       << " [" << (valor == NULL || valor->tipo == NULL ? "TIPO_INVALIDO" : valor->tipo->nome()) << "]"<< endl;
}
