#include "ComandoWhile.hpp"
#include "TabelaSimbolos.hpp"
#include <iostream>
#include <stdexcept>
#include "../debug-util.hpp"
using namespace std;

ComandoWhile::ComandoWhile() {
  condicao = NULL;
}

bool ComandoWhile::executa(TabelaSimbolos* memoria) {
  if (condicao == NULL) {
    throw runtime_error("Condicao do WHILE ausente");
  }
  while (true) {
    ValorLiteral cond = condicao->avalia(memoria);
    if (cond.tipo == NULL || cond.tipo->valor != Tipo::BOOL) {
      throw runtime_error("Condicao do WHILE deve ser booleana");
    }
    if (!cond.valor_bool) {
      break;
    }
    for (size_t i = 0; i < comandos.size(); ++i) {
      if (!comandos[i]->executa(memoria)) {
        return false;
      }
    }
  }
  return true;
}

void ComandoWhile::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "INI WHILE" << endl;
  if (condicao != NULL) condicao->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "DO" << endl;
  for (int i = 0; i < comandos.size(); ++i) {
    comandos[i]->debug_com_tab(tab+1);
  }
  tab3(tab);
  cerr << "FIM WHILE" << endl;
}
