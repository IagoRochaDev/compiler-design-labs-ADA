#include "Expressao.hpp"
#include "ExpressaoDivisao.hpp"
#include "ExpressaoIgualdade.hpp"
#include "ExpressaoMenor.hpp"
#include "ExpressaoMod.hpp"
#include "ExpressaoMultiplicacao.hpp"
#include "ExpressaoNegacao.hpp"
#include "ExpressaoSoma.hpp"
#include "ExpressaoSubtracao.hpp"
#include "ExpressaoValor.hpp"
#include "ExpressaoVariavel.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

class ExpressaoAnd : public Expressao {
public:
  Expressao* esquerda;
  Expressao* direita;
  
  ValorLiteral avalia(TabelaSimbolos* memoria) override {
    ValorLiteral esq = esquerda->avalia(memoria);
    ValorLiteral dir = direita->avalia(memoria);
    ValorLiteral res;
    res.tipo = esq.tipo; // Assume que ambos são booleanos
    res.valor_bool = esq.valor_bool && dir.valor_bool;
    return res;
  }
  
  void debug_com_tab(int tab) override {
    tab3(tab); cerr << "AND" << endl;
    if (esquerda != NULL) esquerda->debug_com_tab(tab + 1);
    if (direita != NULL) direita->debug_com_tab(tab + 1);
  }
};

static ID* extrai_id_de_acesso(No_arv_parse* no) {
  if (no == NULL) return NULL;
  if (no->simb == "ID" && no->filhos.empty()) return ID::extrai_ID(no);
  if (!no->filhos.empty()) return extrai_id_de_acesso(no->filhos[0]);
  return NULL;
}

Expressao* Expressao::extrai_expressao(No_arv_parse* no) {
  if (no == NULL) return NULL;
  switch(no->regra) {
  case 70: { return extrai_expressao(no->filhos[0]); }
  // Agrupamos todas as regras lógicas para que NENHUMA retorne NULL e quebre a árvore!
  case 71: case 72: case 73: case 74: { //TODO: Adicionar suporte para OR e NOT aqui também
    if (no->filhos.size() >= 3) {
      ExpressaoAnd* res = new ExpressaoAnd();
      res->esquerda = extrai_expressao(no->filhos[0]); // Lado esquerdo (a < b)
      res->direita  = extrai_expressao(no->filhos[2]); // Lado direito  (b < c)
      return res;
    }
    return NULL;
  }
  case 75: { return extrai_expressao(no->filhos[0]); }
  case 76: {
    ExpressaoIgualdade* res = new ExpressaoIgualdade();
    res->esquerda = extrai_expressao(no->filhos[0]);
    res->direita = extrai_expressao(no->filhos[2]);
    return res;
  }
  case 77: case 79: case 81: { return NULL; }
  case 78: {
    ExpressaoMenor* res = new ExpressaoMenor();
    res->esquerda = extrai_expressao(no->filhos[0]);
    res->direita = extrai_expressao(no->filhos[2]);
    return res;
  }
  case 80: { // Regra do operador MAIOR ( > )
    ExpressaoMenor* res = new ExpressaoMenor();
    // Truque: Para avaliar A > B, nós montamos a árvore como B < A
    res->esquerda = extrai_expressao(no->filhos[2]); // Lado direito vai para a esquerda
    res->direita = extrai_expressao(no->filhos[0]);  // Lado esquerdo vai para a direita
    return res;
  }
  case 82: { return extrai_expressao(no->filhos[0]); }
  case 83: {
    ExpressaoSoma* res = new ExpressaoSoma();
    res->esquerda = extrai_expressao(no->filhos[0]);
    res->direita = extrai_expressao(no->filhos[2]);
    return res;
  }
  case 84: {
    ExpressaoSubtracao* res = new ExpressaoSubtracao();
    res->esquerda = extrai_expressao(no->filhos[0]);
    res->direita = extrai_expressao(no->filhos[2]);
    return res;
  }
  case 85: { return extrai_expressao(no->filhos[0]); }
  case 86: {
    ExpressaoMultiplicacao* res = new ExpressaoMultiplicacao();
    res->esquerda = extrai_expressao(no->filhos[0]);
    res->direita = extrai_expressao(no->filhos[2]);
    return res;
  }
  case 87: {
    ExpressaoDivisao* res = new ExpressaoDivisao();
    res->esquerda = extrai_expressao(no->filhos[0]);
    res->direita = extrai_expressao(no->filhos[2]);
    return res;
  }
  case 88: {
    ExpressaoMod* res = new ExpressaoMod();
    res->esquerda = extrai_expressao(no->filhos[0]);
    res->direita = extrai_expressao(no->filhos[2]);
    return res;
  }
  case 89: { return extrai_expressao(no->filhos[0]); }
  case 90: {
    ExpressaoNegacao* res = new ExpressaoNegacao();
    res->expressao = extrai_expressao(no->filhos[1]);
    res->eh_not = false;
    return res;
  }
  case 91: {
    ExpressaoNegacao* res = new ExpressaoNegacao();
    res->expressao = extrai_expressao(no->filhos[1]);
    res->eh_not = true;
    return res;
  }
  case 92: { return extrai_expressao(no->filhos[0]); }
  case 93: { return extrai_expressao(no->filhos[1]); }
  case 94: case 95: case 96: case 97: {
    if (no->regra == 97) {
      ExpressaoVariavel* res = new ExpressaoVariavel();
      res->nome = extrai_id_de_acesso(no->filhos[0]);
      return res;
    }
    ExpressaoValor* res = new ExpressaoValor();
    res->valor = ValorLiteral::extrai_valor_literal(no->filhos[0]);
    return res;
  }
  default: { return NULL; }
  }
}

Expressao::~Expressao() {}

void Expressao::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "Expressao generica" << endl;
}