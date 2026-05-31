#include "Comando.hpp"
#include "ComandoAtribuicao.hpp"
#include "ComandoDeclaracao.hpp"
#include "ComandoIf.hpp"
#include "ComandoLista.hpp"
#include "ComandoRetorno.hpp"
#include "ComandoWhile.hpp"
#include "ComandoDeclare.hpp" 
#include "ID.hpp"
#include "Variavel.hpp"
#include <iostream>
#include "../debug-util.hpp"


static vector<Variavel*> extrai_lista_declaracoes_locais_cmd(No_arv_parse* no) {
  vector<Variavel*> res;
  if (no == NULL) return res;

  if (no->simb == "DeclVar") {
    No_arv_parse* no_lista_ids = no->filhos[0];
    
    struct Helper {
      static void coletar_ids(No_arv_parse* n, vector<ID*>& ids) {
        if (n == NULL) return;
        if (n->simb == "ID") {
          ids.push_back(ID::extrai_ID(n));
          return;
        }
        if (n->simb == "ListaIds") {
          if (n->filhos.size() == 1) { 
            ids.push_back(ID::extrai_ID(n->filhos[0]));
          } else if (n->filhos.size() == 3) { 
            coletar_ids(n->filhos[0], ids);
            ids.push_back(ID::extrai_ID(n->filhos[2]));
          }
        }
      }
    };

    vector<ID*> ids;
    Helper::coletar_ids(no_lista_ids, ids);

    Tipo* tipo = NULL;
    if (no->filhos.size() >= 3 && no->filhos[2] != NULL) {
      No_arv_parse* no_tipo_var = no->filhos[2];
      if (!no_tipo_var->filhos.empty() && no_tipo_var->filhos[0]->simb == "Acesso") {
        tipo = Tipo::extrai_Tipo(no_tipo_var->filhos[0]);
      } else {
        tipo = Tipo::extrai_Tipo(no_tipo_var);
      }
    }

    for (ID* id : ids) {
      if (id != NULL) {
        res.push_back(new Variavel(id, tipo));
      }
    }
    return res;
  }

  for (size_t i = 0; i < no->filhos.size(); ++i) {
    vector<Variavel*> do_filho = extrai_lista_declaracoes_locais_cmd(no->filhos[i]);
    res.insert(res.end(), do_filho.begin(), do_filho.end());
  }
  return res;
}

static ID* extrai_id_de_acesso(No_arv_parse* no) {
  if (no == NULL) return NULL;
  if (no->simb == "ID" && no->filhos.empty()) return ID::extrai_ID(no);
  if (!no->filhos.empty()) return extrai_id_de_acesso(no->filhos[0]);
  return NULL;
}

static void extrai_fim_if(No_arv_parse* no, ComandoIf* if_node) {
  if (no == NULL || no->filhos.empty()) return;
  
  
  string token = no->filhos[0]->simb;

  if (token == "END" || no->filhos.size() == 3) { 
    
    return;
  }
  else if (token == "ELSIF") { 
    
    ComandoIf* elsif_node = new ComandoIf();
    elsif_node->condicao = Expressao::extrai_expressao(no->filhos[1]);
    elsif_node->comandos_then = Comando::extrai_lista_comandos(no->filhos[3]);
    if_node->proximo_elsif = elsif_node;
    extrai_fim_if(no->filhos[4], elsif_node);
    return;
  }
  else if (token == "ELSE") { 
    
    if_node->comandos_else = Comando::extrai_lista_comandos(no->filhos[1]);
    return;
  }
}

vector<Comando*> Comando::extrai_lista_comandos(No_arv_parse* no) {
  
  if (no == NULL || no->filhos.empty()) return vector<Comando*>(); 
  
  vector<Comando*> res = extrai_lista_comandos(no->filhos[0]);
  
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

  

  if (no->simb == "ComandoDeclare" || no->simb == "Declare") {
    ComandoDeclare* res = new ComandoDeclare();
    for (size_t i = 0; i < no->filhos.size(); ++i) {
      No_arv_parse* filho = no->filhos[i];
      if (filho == NULL) continue;

      if (filho->simb == "ListaDeclaracoes") {
        res->declaracoes = extrai_lista_declaracoes_locais_cmd(filho);
      } else if (filho->simb == "ListaComandos") {
        res->comandos = extrai_lista_comandos(filho);
      }
    }
    return res;
  } 
  
  else if (no->simb == "ComandoIf") { 
    ComandoIf* res = new ComandoIf();
    res->condicao = Expressao::extrai_expressao(no->filhos[1]);
    res->comandos_then = extrai_lista_comandos(no->filhos[3]);
    extrai_fim_if(no->filhos[4], res);
    return res;
  } 
  
  else if (no->simb == "ComandoWhile") { 
    ComandoWhile* res = new ComandoWhile();
    res->condicao = Expressao::extrai_expressao(no->filhos[1]);
    res->comandos = extrai_lista_comandos(no->filhos[3]);
    return res;
  } 
  
  else if (no->simb == "ComandoReturn") { 
    ComandoRetorno* res = new ComandoRetorno();
    
    if (no->filhos.size() > 2) { 
      res->expressao = Expressao::extrai_expressao(no->filhos[1]);
    } else { 
      res->expressao = NULL;
    }
    return res;
  } 
  
  else if (no->simb == "ComandoAtribuicao") { 
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