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
#include <vector>
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
    res.tipo = esq.tipo; 
    res.valor_bool = esq.valor_bool && dir.valor_bool;
    return res;
  }
  
  void debug_com_tab(int tab) override {
    tab3(tab); cerr << "AND" << endl;
    if (esquerda != NULL) esquerda->debug_com_tab(tab + 1);
    if (direita != NULL) direita->debug_com_tab(tab + 1);
  }
};

// =====================================================================
// NOVOS HELPERS PARA LER ARGUMENTOS DE FUNÇÕES (LAB 04)
// =====================================================================

// Varre uma regra recursiva do tipo ListaExp -> ListaExp VIRGULA Exp
static void extrai_elementos_lista(No_arv_parse* no, vector<Expressao*>& args) {
    if (no == NULL) return;
    
    // Padrão de repetição com vírgula (tamanho 3)
    if (no->filhos.size() == 3 && (no->filhos[1]->simb == "," || no->filhos[1]->simb == "VIRGULA")) {
        extrai_elementos_lista(no->filhos[0], args); // Desce na lista (esquerda)
        extrai_elementos_lista(no->filhos[2], args); // Desce na expressão (direita)
        return;
    }
    
    // Se não tiver vírgula dividindo, tenta tratar como uma Expressão solitária
    Expressao* exp = Expressao::extrai_expressao(no);
    if (exp != NULL) {
        args.push_back(exp);
    } else if (!no->filhos.empty()) {
        // Se falhar e tiver filhos (ex: nó estrutural vazio), desce
        extrai_elementos_lista(no->filhos[0], args);
    }
}

// Navega no nó "Acesso", resgatando tanto o ID quanto os Argumentos!
static void analisa_acesso(No_arv_parse* no, ID*& id_out, vector<Expressao*>& args_out) {
    if (no == NULL) return;

    // Caso Base: Achou o token de identificador (nome da variável/função)
    if (no->simb == "ID" && no->filhos.empty()) {
        id_out = ID::extrai_ID(no);
        return;
    }

    // Padrão Chamada com Argumentos: Acesso -> Acesso ( ListaExp ) 
    if (no->filhos.size() == 4 && 
       (no->filhos[1]->simb == "(" || no->filhos[1]->simb == "ABRE_PARENTESES")) {
        analisa_acesso(no->filhos[0], id_out, args_out);  // 1. Extrai o ID da função
        extrai_elementos_lista(no->filhos[2], args_out);  // 2. Extrai os argumentos
        return;
    }
    
    // Padrão Chamada sem Argumentos: Acesso -> Acesso ( ) 
    if (no->filhos.size() == 3 && 
       (no->filhos[1]->simb == "(" || no->filhos[1]->simb == "ABRE_PARENTESES") &&
       (no->filhos[2]->simb == ")" || no->filhos[2]->simb == "FECHA_PARENTESES")) {
        analisa_acesso(no->filhos[0], id_out, args_out);
        return;
    }

    // Caso recursivo geral: continua descendo a árvore
    if (!no->filhos.empty()) {
        analisa_acesso(no->filhos[0], id_out, args_out);
    }
}
// =====================================================================


Expressao* Expressao::extrai_expressao(No_arv_parse* no) {
  if (no == NULL) return NULL;

  if (no->filhos.size() == 1) {
    No_arv_parse* filho = no->filhos[0];
    
    if (filho->simb == "Acesso") {
      ExpressaoVariavel* res = new ExpressaoVariavel();
      
      // NOVA LÓGICA DE EXTRAÇÃO PARA O LAB 04
      ID* id_encontrado = NULL;
      vector<Expressao*> args_encontrados;
      
      analisa_acesso(filho, id_encontrado, args_encontrados);
      
      res->nome = id_encontrado;
      res->argumentos = args_encontrados;
      return res;
    }
    
    if (filho->filhos.empty()) {
      ExpressaoValor* res = new ExpressaoValor();
      res->valor = ValorLiteral::extrai_valor_literal(filho);
      return res;
    }

    return extrai_expressao(filho);
  }

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

  if (no->filhos.size() == 3) {
    string op_esq = no->filhos[0]->simb;
    string op = no->filhos[1]->simb; 
    
    if (op_esq == "(" || op_esq == "ABRE_PARENTESES") {
      return extrai_expressao(no->filhos[1]);
    }

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
      res->esquerda = extrai_expressao(no->filhos[2]); 
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
    else if (op == "OR" || op == "||" || op == "OU") {
      ExpressaoAnd* res = new ExpressaoAnd(); 
      res->esquerda = extrai_expressao(no->filhos[0]);
      res->direita  = extrai_expressao(no->filhos[2]);
      return res;
    }
  }

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