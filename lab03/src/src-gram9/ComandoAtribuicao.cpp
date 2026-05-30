#include "ComandoAtribuicao.hpp"
#include "TabelaSimbolos.hpp"
#include <iostream>
#include <stdexcept>
#include "../debug-util.hpp"
using namespace std;

ComandoAtribuicao::ComandoAtribuicao() {
}

bool ComandoAtribuicao::executa(TabelaSimbolos* memoria) {
  if (esquerda == NULL) {
    throw runtime_error("ID de atribuicao ausente");
  }
  if (direita == NULL) {
    throw runtime_error("Expressao de atribuicao ausente");
  }
  
  // 1. Avalia o lado direito primeiro
  ValorLiteral valor = direita->avalia(memoria);

  // --- TRAVA DE SEGURANÇA SEMÂNTICA ---
  // Tente buscar a variável na memória para inspecionar o tipo dela antes de atribuir.
  // ATENÇÃO: Ajuste 'busca_variavel' para o método real que sua TabelaSimbolos usa!
  /*
  Variavel* var_destino = memoria->busca_variavel(esquerda->nome);
  if (var_destino != NULL && var_destino->tipo != NULL && valor.tipo != NULL) {
      string tipo_destino = var_destino->tipo->nome();
      string tipo_valor = valor.tipo->nome();

      if (tipo_destino != tipo_valor) {
          throw runtime_error("Erro Semantico: Variavel '" + esquerda->nome + 
                              "' e do tipo " + tipo_destino + 
                              ", mas recebeu um valor do tipo " + tipo_valor);
      }
  }
  */
  // ------------------------------------

  // 2. Faz a atribuição real na memória
  if (!memoria->atribuir(esquerda->nome, valor)) {
    throw runtime_error(string("Variavel nao declarada: ") + esquerda->nome);
  }
  
  return true;
}

void ComandoAtribuicao::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "INI ATRIB: " << esquerda->nome << " = " << endl;
  direita->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "FIM ATRIB" << endl;
  cerr << endl;
}