#include "ExpressaoVariavel.hpp"
#include "TabelaSimbolos.hpp"
#include "Tipo.hpp"
#include <iostream>
#include <stdexcept>
#include <algorithm> // Necessário para o std::transform
#include "../debug-util.hpp"

using namespace std;

ExpressaoVariavel::ExpressaoVariavel() { }

ValorLiteral ExpressaoVariavel::avalia(TabelaSimbolos* memoria) {
  if (nome == NULL) {
    throw runtime_error("Nome da variavel ausente");
  }

  string var_nome = nome->nome;

  // Cria uma cópia em maiúsculo (TRUE/FALSE) para não dependermos de como o código foi digitado
  string upper_nome = var_nome;
  transform(upper_nome.begin(), upper_nome.end(), upper_nome.begin(), ::toupper);

  // === A MÁGICA: INTERCEPTA LITERAIS BOOLEANOS AQUI ===
  if (upper_nome == "TRUE" || upper_nome == "FALSE") {
    ValorLiteral valor_booleano;
    valor_booleano.tipo = new Tipo(Tipo::BOOL); // Instancia já passando o tipo correto
    valor_booleano.valor_bool = (upper_nome == "TRUE"); // Salva true ou false
    return valor_booleano;
  }

  // === FLUXO NORMAL (Busca na memória por variáveis reais) ===
  if (memoria == NULL) {
    throw runtime_error("Memoria de execucao nao inicializada");
  }
  
  Variavel* var = memoria->busca(var_nome);
  if (var == NULL) {
    throw runtime_error(string("Variavel nao declarada: ") + var_nome);
  }
  if (!var->esta_inicializada()) {
    throw runtime_error(string("Variavel nao inicializada: ") + var_nome);
  }
  
  return var->get_valor();
}

void ExpressaoVariavel::debug_com_tab(int tab) {
  tab3(tab);
  
  // Imprime de forma diferente no debug para sabermos que ele entendeu o booleano
  string upper_nome = nome->nome;
  transform(upper_nome.begin(), upper_nome.end(), upper_nome.begin(), ::toupper);
  
  if (upper_nome == "TRUE" || upper_nome == "FALSE") {
    cerr << "LITERAL_BOOL[" << nome->nome << "]" << endl;
  } else {
    cerr << "VAR[" << nome->nome << "]" << endl;
  }
}