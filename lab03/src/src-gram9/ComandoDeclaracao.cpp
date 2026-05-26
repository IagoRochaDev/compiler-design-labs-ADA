#include "ComandoDeclaracao.hpp"
#include "TabelaSimbolos.hpp"
#include <iostream>
#include <stdexcept>
#include "../debug-util.hpp"
using namespace std;

ComandoDeclaracao::ComandoDeclaracao() {
}

bool ComandoDeclaracao::executa(TabelaSimbolos* memoria) {
  if (variavel == NULL || variavel->nome == NULL) {
    throw runtime_error("Declaracao de variavel invalida");
  }
  if (!memoria->adiciona(variavel)) {
    throw runtime_error(string("Variavel ja declarada: ") + variavel->nome->nome);
  }
  return true;
}

void ComandoDeclaracao::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "[" << variavel->nome->nome << ":" << (variavel->tipo == NULL ? "TIPO_INVALIDO" : variavel->tipo->nome()) << "] Declaracao" << endl;
}
