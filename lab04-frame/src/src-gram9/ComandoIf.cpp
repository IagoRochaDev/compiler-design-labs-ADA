#include "ComandoIf.hpp"
#include "TabelaSimbolos.hpp"
#include <iostream>
#include <stdexcept>
#include "../debug-util.hpp"
using namespace std;

ComandoIf::ComandoIf() {
  condicao = NULL;
  proximo_elsif = NULL;
}

bool ComandoIf::executa(TabelaSimbolos* memoria) {
  if (condicao == NULL) {
    throw runtime_error("Condicao do IF ausente");
  }
  ValorLiteral cond = condicao->avalia(memoria);
  if (cond.tipo == NULL || cond.tipo->valor != Tipo::BOOL) {
    throw runtime_error("Condicao do IF deve ser booleana");
  }
  if (cond.valor_bool) {
    for (size_t i = 0; i < comandos_then.size(); ++i) {
      if (!comandos_then[i]->executa(memoria)) {
        return false;
      }
    }
    return true;
  }
  if (proximo_elsif != NULL) {
    return proximo_elsif->executa(memoria);
  }
  for (size_t i = 0; i < comandos_else.size(); ++i) {
    if (!comandos_else[i]->executa(memoria)) {
      return false;
    }
  }
  return true;
}

void ComandoIf::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "INI IF" << endl;
  if (condicao != NULL) condicao->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "THEN" << endl;
  for (int i = 0; i < comandos_then.size(); ++i) {
    comandos_then[i]->debug_com_tab(tab+1);
  }
  if (proximo_elsif != NULL) {
    tab3(tab);
    cerr << "ELSIF" << endl;
    proximo_elsif->debug_com_tab(tab+1);
  }
  if (!comandos_else.empty()) {
    tab3(tab);
    cerr << "ELSE" << endl;
    for (int i = 0; i < comandos_else.size(); ++i) {
      comandos_else[i]->debug_com_tab(tab+1);
    }
  }
  tab3(tab);
  cerr << "FIM IF" << endl;
}
