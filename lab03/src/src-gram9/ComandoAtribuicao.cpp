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
  
  
  ValorLiteral valor = direita->avalia(memoria);

  
  
  
  
  

  
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