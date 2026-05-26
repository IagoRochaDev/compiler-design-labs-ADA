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

static ID* extrai_id_de_acesso(No_arv_parse* no) {
  if (no == NULL) return NULL;
  if (no->simb == "ID" && no->filhos.empty()) return ID::extrai_ID(no);
  if (!no->filhos.empty()) return extrai_id_de_acesso(no->filhos[0]);
  return NULL;
}

// Tratei ExpLogica, ExpRelacional, ExpAditiva, ExpMultiplicativa, ExpUnaria e Fator na mesma funcao.
Expressao* Expressao::extrai_expressao(No_arv_parse* no) {
  // Regras Ada: 69..96
  if (no == NULL) return NULL;
  switch(no->regra) {
  case 69: { // Expressao -> ExpLogica
    return extrai_expressao(no->filhos[0]);
  }
  case 70: // ExpLogica -> ExpLogica OR ExpRelacional
  case 71: // ExpLogica -> ExpLogica AND ExpRelacional
  case 72: // ExpLogica -> ExpLogica OR ELSE ExpRelacional
  case 73: { // ExpLogica -> ExpLogica AND THEN ExpRelacional
    return NULL; // operador logico não suportado nesta AST atual
  }
  case 74: { // ExpLogica -> ExpRelacional
    return extrai_expressao(no->filhos[0]);
  }
  case 75: { // ExpRelacional -> ExpRelacional IGUAL ExpAditiva
    ExpressaoIgualdade* res = new ExpressaoIgualdade();
    res->esquerda = extrai_expressao(no->filhos[0]);
    res->direita = extrai_expressao(no->filhos[2]);
    return res;
  }
  case 76: // ExpRelacional -> ExpRelacional DIFERENTE ExpAditiva
  case 78: // ExpRelacional -> ExpRelacional MENOR_IGUAL ExpAditiva
  case 79: // ExpRelacional -> ExpRelacional MAIOR ExpAditiva
  case 80: { // ExpRelacional -> ExpRelacional MAIOR_IGUAL ExpAditiva
    return NULL; // operador relacional não suportado nesta AST atual
  }
  case 77: { // ExpRelacional -> ExpRelacional MENOR ExpAditiva
    ExpressaoMenor* res = new ExpressaoMenor();
    res->esquerda = extrai_expressao(no->filhos[0]);
    res->direita = extrai_expressao(no->filhos[2]);
    return res;
  }
  case 81: { // ExpRelacional -> ExpAditiva
    return extrai_expressao(no->filhos[0]);
  }
  case 82: { // ExpAditiva -> ExpAditiva MAIS ExpMultiplicativa
    ExpressaoSoma* res = new ExpressaoSoma();
    res->esquerda = extrai_expressao(no->filhos[0]);
    res->direita = extrai_expressao(no->filhos[2]);
    return res;
  }
  case 83: { // ExpAditiva -> ExpAditiva MENOS ExpMultiplicativa
    ExpressaoSubtracao* res = new ExpressaoSubtracao();
    res->esquerda = extrai_expressao(no->filhos[0]);
    res->direita = extrai_expressao(no->filhos[2]);
    return res;
  }
  case 84: { // ExpAditiva -> ExpMultiplicativa
    return extrai_expressao(no->filhos[0]);
  }
  case 85: { // ExpMultiplicativa -> ExpMultiplicativa MULTIPLICACAO ExpUnaria
    ExpressaoMultiplicacao* res = new ExpressaoMultiplicacao();
    res->esquerda = extrai_expressao(no->filhos[0]);
    res->direita = extrai_expressao(no->filhos[2]);
    return res;
  }
  case 86: { // ExpMultiplicativa -> ExpMultiplicativa DIVISAO ExpUnaria
    ExpressaoDivisao* res = new ExpressaoDivisao();
    res->esquerda = extrai_expressao(no->filhos[0]);
    res->direita = extrai_expressao(no->filhos[2]);
    return res;
  }
  case 87: { // ExpMultiplicativa -> ExpMultiplicativa MOD ExpUnaria
    ExpressaoMod* res = new ExpressaoMod();
    res->esquerda = extrai_expressao(no->filhos[0]);
    res->direita = extrai_expressao(no->filhos[2]);
    return res;
  }
  case 88: { // ExpMultiplicativa -> ExpUnaria
    return extrai_expressao(no->filhos[0]);
  }
  case 89: { // ExpUnaria -> MENOS Fator
    ExpressaoNegacao* res = new ExpressaoNegacao();
    res->expressao = extrai_expressao(no->filhos[1]);
    res->eh_not = false;
    return res;
  }
  case 90: { // ExpUnaria -> NOT Fator
    ExpressaoNegacao* res = new ExpressaoNegacao();
    res->expressao = extrai_expressao(no->filhos[1]);
    res->eh_not = true;
    return res;
  }
  case 91: { // ExpUnaria -> Fator
    return extrai_expressao(no->filhos[0]);
  }
  case 92: { // Fator -> ABRE_PARENTESES Expressao FECHA_PARENTESES
    return extrai_expressao(no->filhos[1]);
  }
  case 93: // Fator -> NUM_INT
  case 94: // Fator -> NUM_DEC
  case 95: // Fator -> STRING
  case 96: { // Fator -> Acesso
    if (no->regra == 96) {
      ExpressaoVariavel* res = new ExpressaoVariavel();
      res->nome = extrai_id_de_acesso(no->filhos[0]);
      return res;
    }
    ExpressaoValor* res = new ExpressaoValor();
    res->valor = ValorLiteral::extrai_valor_literal(no->filhos[0]);
    return res;
  }
  default: {
    return NULL;
  }
  }
}

Expressao::~Expressao() {
}

void Expressao::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "Expressao generica" << endl;
}
