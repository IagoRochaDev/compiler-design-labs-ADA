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

  // --- REGRAS UNITÁRIAS (Pass-through, Valores ou Variáveis) ---
  if (no->filhos.size() == 1) {
    No_arv_parse* filho = no->filhos[0];
    
    // Antiga regra 97: Acesso à variável
    if (filho->simb == "Acesso") {
      ExpressaoVariavel* res = new ExpressaoVariavel();
      res->nome = extrai_id_de_acesso(filho);
      return res;
    }
    
    // Antigas regras 94, 95, 96: Valores Literais
    // Identificamos checando se o filho é uma folha (terminal sem filhos)
    if (filho->filhos.empty()) {
      ExpressaoValor* res = new ExpressaoValor();
      res->valor = ValorLiteral::extrai_valor_literal(filho);
      return res;
    }

    // Pass-through genérico (antigas regras 70, 75, 82, 85, 89, 92)
    // Se não for valor final nem acesso, apenas desce na árvore
    return extrai_expressao(filho);
  }

  // --- REGRAS UNÁRIAS (Prefixo com 2 filhos: Operador e Expressão) ---
  // Exemplo: "- 5" ou "NOT true"
  if (no->filhos.size() == 2) {
    string op = no->filhos[0]->simb;
    
    if (op == "-" || op == "MENOS") {
      ExpressaoNegacao* res = new ExpressaoNegacao();
      res->expressao = extrai_expressao(no->filhos[1]);
      res->eh_not = false;
      return res;
    }
    else if (op == "NOT" || op == "!" || op == "NAO") {
      ExpressaoNegacao* res = new ExpressaoNegacao();
      res->expressao = extrai_expressao(no->filhos[1]);
      res->eh_not = true;
      return res;
    }
  }

  // --- REGRAS BINÁRIAS E PARÊNTESES (3 filhos) ---
  if (no->filhos.size() == 3) {
    string op_esq = no->filhos[0]->simb;
    string op = no->filhos[1]->simb; // O operador do meio
    
    // Antiga regra 93: Parenteses -> ( Expressao )
    if (op_esq == "(" || op_esq == "ABRE_PARENTESES") {
      return extrai_expressao(no->filhos[1]);
    }

    // Avaliação dos operadores lógicos e aritméticos pelo Símbolo (Token) do meio
    if (op == "==" || op == "=" || op == "IGUAL") {
      ExpressaoIgualdade* res = new ExpressaoIgualdade();
      res->esquerda = extrai_expressao(no->filhos[0]);
      res->direita  = extrai_expressao(no->filhos[2]);
      return res;
    }
    else if (op == "<" || op == "MENOR") {
      ExpressaoMenor* res = new ExpressaoMenor();
      res->esquerda = extrai_expressao(no->filhos[0]);
      res->direita  = extrai_expressao(no->filhos[2]);
      return res;
    }
    else if (op == ">" || op == "MAIOR") {
      ExpressaoMenor* res = new ExpressaoMenor();
      // O truque continua: para A > B, montamos B < A
      res->esquerda = extrai_expressao(no->filhos[2]); // Cruzado
      res->direita  = extrai_expressao(no->filhos[0]);
      return res;
    }
    else if (op == "+" || op == "MAIS") {
      ExpressaoSoma* res = new ExpressaoSoma();
      res->esquerda = extrai_expressao(no->filhos[0]);
      res->direita  = extrai_expressao(no->filhos[2]);
      return res;
    }
    else if (op == "-" || op == "MENOS") {
      ExpressaoSubtracao* res = new ExpressaoSubtracao();
      res->esquerda = extrai_expressao(no->filhos[0]);
      res->direita  = extrai_expressao(no->filhos[2]);
      return res;
    }
    else if (op == "*" || op == "VEZES" || op == "MULT") {
      ExpressaoMultiplicacao* res = new ExpressaoMultiplicacao();
      res->esquerda = extrai_expressao(no->filhos[0]);
      res->direita  = extrai_expressao(no->filhos[2]);
      return res;
    }
    else if (op == "/" || op == "DIV" || op == "DIVISAO") {
      ExpressaoDivisao* res = new ExpressaoDivisao();
      res->esquerda = extrai_expressao(no->filhos[0]);
      res->direita  = extrai_expressao(no->filhos[2]);
      return res;
    }
    else if (op == "%" || op == "MOD") {
      ExpressaoMod* res = new ExpressaoMod();
      res->esquerda = extrai_expressao(no->filhos[0]);
      res->direita  = extrai_expressao(no->filhos[2]);
      return res;
    }
    else if (op == "AND" || op == "&&" || op == "E") {
      ExpressaoAnd* res = new ExpressaoAnd();
      res->esquerda = extrai_expressao(no->filhos[0]);
      res->direita  = extrai_expressao(no->filhos[2]);
      return res;
    }
    // Caso sua gramática também tenha OR sendo mapeado para esse fallback (do seu antigo TODO)
    else if (op == "OR" || op == "||" || op == "OU") {
      ExpressaoAnd* res = new ExpressaoAnd(); // Usando seu fallback pra AND por enquanto
      res->esquerda = extrai_expressao(no->filhos[0]);
      res->direita  = extrai_expressao(no->filhos[2]);
      return res;
    }
  }

  // --- FALLBACK GENÉRICO DE SEGURANÇA ---
  // Se ele caiu numa regra que não mapeamos acima, tenta descer no 1º filho
  if (!no->filhos.empty()) {
    return extrai_expressao(no->filhos[0]);
  }

  return NULL;
}

Expressao::~Expressao() {}

void Expressao::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "Expressao generica" << endl;
}