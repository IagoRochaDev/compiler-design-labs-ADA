#include "ComandoDeclare.hpp"
#include "TabelaSimbolos.hpp"
#include <iostream>
#include <stdexcept>
#include "../debug-util.hpp"

using namespace std;

ComandoDeclare::ComandoDeclare() {
}

bool ComandoDeclare::executa(TabelaSimbolos* memoria) {
  // 1. Cria um NOVO escopo de memória! 
  // Passamos a 'memoria' atual como "pai" para não perder as variáveis externas.
  TabelaSimbolos* memoria_local = new TabelaSimbolos(memoria);

  // 2. Registra as variáveis locais do bloco neste novo escopo
  for (size_t i = 0; i < declaracoes.size(); ++i) {
    if (declaracoes[i] != NULL) {
      memoria_local->adiciona(declaracoes[i]); // (Mesmo método usado na Funcao)
    }
  }

  // 3. Executa os comandos internos usando APENAS a memória local
  bool propagar_retorno = true;
  for (size_t i = 0; i < comandos.size(); ++i) {
    if (comandos[i] != NULL) {
      if (!comandos[i]->executa(memoria_local)) {
        propagar_retorno = false; // Um "return" foi disparado dentro do bloco
        break;
      }
    }
  }

  // 4. LIMPEZA VITAL: Ao sair do bloco, destruímos o escopo temporário.
  // Isso mata as variáveis sombreadas e devolve o programa ao estado original.
  delete memoria_local;

  return propagar_retorno;
}

void ComandoDeclare::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "INI DECLARE" << endl;
  
  // Imprime as variáveis declaradas no bloco
  for (size_t i = 0; i < declaracoes.size(); ++i) {
    if (declaracoes[i] != NULL) {
      declaracoes[i]->debug_com_tab(tab + 1);
    }
  }
  
  tab3(tab);
  cerr << "BEGIN" << endl;
  
  // Imprime os comandos de dentro do bloco
  for (size_t i = 0; i < comandos.size(); ++i) {
    if (comandos[i] != NULL) {
      comandos[i]->debug_com_tab(tab + 1);
    }
  }
  
  tab3(tab);
  cerr << "FIM DECLARE" << endl;
}