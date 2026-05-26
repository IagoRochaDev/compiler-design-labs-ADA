#include "ExpressaoVariavel.hpp"
#include "TabelaSimbolos.hpp"
#include <iostream>
#include <stdexcept>
#include "../debug-util.hpp"

using namespace std;
ExpressaoVariavel::ExpressaoVariavel() { }

ValorLiteral ExpressaoVariavel::avalia(TabelaSimbolos* memoria) {
  if (nome == NULL) {
    throw runtime_error("Nome da variavel ausente");
  }
  if (memoria == NULL) {
    throw runtime_error("Memoria de execucao nao inicializada");
  }
  Variavel* var = memoria->busca(nome->nome);
  if (var == NULL) {
    throw runtime_error(string("Variavel nao declarada: ") + nome->nome);
  }
  if (!var->esta_inicializada()) {
    throw runtime_error(string("Variavel nao inicializada: ") + nome->nome);
  }
  return var->get_valor();
}

void ExpressaoVariavel::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "VAR[" << nome->nome << "]" << endl;
}
