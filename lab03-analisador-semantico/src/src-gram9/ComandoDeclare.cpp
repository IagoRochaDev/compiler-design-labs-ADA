#include "ComandoDeclare.hpp"
#include "TabelaSimbolos.hpp"
#include <iostream>
#include <stdexcept>
#include "../debug-util.hpp"

using namespace std;

ComandoDeclare::ComandoDeclare() {
}

bool ComandoDeclare::executa(TabelaSimbolos* memoria) {
  
  
  TabelaSimbolos* memoria_local = new TabelaSimbolos(memoria);

  
  for (size_t i = 0; i < declaracoes.size(); ++i) {
    if (declaracoes[i] != NULL) {
      memoria_local->adiciona(declaracoes[i]); 
    }
  }

  
  bool propagar_retorno = true;
  for (size_t i = 0; i < comandos.size(); ++i) {
    if (comandos[i] != NULL) {
      if (!comandos[i]->executa(memoria_local)) {
        propagar_retorno = false; 
        break;
      }
    }
  }

  
  
  delete memoria_local;

  return propagar_retorno;
}

void ComandoDeclare::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "INI DECLARE" << endl;
  
  
  for (size_t i = 0; i < declaracoes.size(); ++i) {
    if (declaracoes[i] != NULL) {
      declaracoes[i]->debug_com_tab(tab + 1);
    }
  }
  
  tab3(tab);
  cerr << "BEGIN" << endl;
  
  
  for (size_t i = 0; i < comandos.size(); ++i) {
    if (comandos[i] != NULL) {
      comandos[i]->debug_com_tab(tab + 1);
    }
  }
  
  tab3(tab);
  cerr << "FIM DECLARE" << endl;
}