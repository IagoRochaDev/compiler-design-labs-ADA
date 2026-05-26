#include "Comando.hpp"
#include "ComandoAtribuicao.hpp"
#include "ComandoDeclaracao.hpp"
#include "ComandoIf.hpp"
#include "ComandoLista.hpp"
#include "ComandoRetorno.hpp"
#include "ComandoWhile.hpp"
#include "ID.hpp"
#include "Variavel.hpp"
#include <iostream>
#include "../debug-util.hpp"

static ID* extrai_id_de_acesso(No_arv_parse* no) {
  if (no == NULL) return NULL;
  if (no->simb == "ID" && no->filhos.empty()) return ID::extrai_ID(no);
  if (!no->filhos.empty()) return extrai_id_de_acesso(no->filhos[0]);
  return NULL;
}

static void extrai_fim_if(No_arv_parse* no, ComandoIf* if_node) {
  if (no == NULL) return;
  if (no->regra == 48) {
    // FimIf -> END IF PONTO_VIRGULA
    return;
  }
  if (no->regra == 49) {
    // FimIf -> ELSIF Expressao THEN ListaComandos FimIf
    ComandoIf* elsif_node = new ComandoIf();
    elsif_node->condicao = Expressao::extrai_expressao(no->filhos[1]);
    elsif_node->comandos_then = Comando::extrai_lista_comandos(no->filhos[3]);
    if_node->proximo_elsif = elsif_node;
    extrai_fim_if(no->filhos[4], elsif_node);
    return;
  }
  if (no->regra == 50) {
    // FimIf -> ELSE ListaComandos END IF PONTO_VIRGULA
    if_node->comandos_else = Comando::extrai_lista_comandos(no->filhos[1]);
    return;
  }
}

vector<Comando*> Comando::extrai_lista_comandos(No_arv_parse* no) {
  if (no == NULL || no->regra == 10) return vector<Comando*>();
  
  // filhos[0] é a ListaComandos (recursão para trás)
  vector<Comando*> res = extrai_lista_comandos(no->filhos[0]);
  
  // filhos[1] é o Comando atual isolado
  Comando* cmd = extrai_comando(no->filhos[1]);
  if (cmd != NULL) {
      res.push_back(cmd);
  }
  return res;
}

Comando* Comando::extrai_comando(No_arv_parse* no) {
  if (no == NULL) return NULL;
  if (no->simb == "Comando" && no->filhos.size() == 1) {
    return extrai_comando(no->filhos[0]);
  }

  if (no->regra == 47) { // ComandoIf -> IF Expressao THEN ListaComandos FimIf
    ComandoIf* res = new ComandoIf();
    res->condicao = Expressao::extrai_expressao(no->filhos[1]);
    res->comandos_then = extrai_lista_comandos(no->filhos[3]);
    extrai_fim_if(no->filhos[4], res);
    return res;
  } else if (no->regra == 51) { // ComandoWhile -> WHILE Expressao LOOP ListaComandos END LOOP PONTO_VIRGULA
    ComandoWhile* res = new ComandoWhile();
    res->condicao = Expressao::extrai_expressao(no->filhos[1]);
    res->comandos = extrai_lista_comandos(no->filhos[3]);
    return res;
  } else if (no->regra == 57) { // ComandoReturn -> RETURN Expressao PONTO_VIRGULA
    ComandoRetorno* res = new ComandoRetorno();
    res->expressao = Expressao::extrai_expressao(no->filhos[1]);
    return res;
  } else if (no->regra == 58) { // ComandoReturn -> RETURN PONTO_VIRGULA
    ComandoRetorno* res = new ComandoRetorno();
    res->expressao = NULL;
    return res;
  } else if (no->regra == 59) { // ComandoAtribuicao -> Acesso ATRIBUICAO Expressao PONTO_VIRGULA
    ComandoAtribuicao* res = new ComandoAtribuicao();
    res->esquerda = extrai_id_de_acesso(no->filhos[0]);
    res->direita = Expressao::extrai_expressao(no->filhos[2]);
    return res;
  }
  return NULL;
}

void Comando::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "Comando generico" << endl;
}

Comando::~Comando() {
}