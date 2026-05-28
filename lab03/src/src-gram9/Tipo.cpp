#include "Tipo.hpp"
#include <algorithm>

Tipo::Tipo(Valor valor) {
  this->valor = valor;
}

string Tipo::nome() const {
  switch(valor) {
  case INT:
    return "INT";
  case FLOAT:
    return "FLOAT";
  case BOOL:
    return "BOOL";
  }
  return "";
}

string normaliza_nome_tipo(string nome) {
  transform(nome.begin(), nome.end(), nome.begin(), ::toupper);
  return nome;
}

Tipo* tipo_por_nome(string nome) {
  nome = normaliza_nome_tipo(nome);
  if (nome == "INT" || nome == "INTEGER") return new Tipo(Tipo::INT);
  if (nome == "FLOAT" || nome == "REAL") return new Tipo(Tipo::FLOAT);
  if (nome == "BOOL" || nome == "BOOLEAN") return new Tipo(Tipo::BOOL);
  return NULL;
}

Tipo* Tipo::extrai_Tipo(No_arv_parse* no) {
  // Tipo -> INT | BOOLEAN | FLOAT
  if (no == NULL) return NULL;
  No_arv_parse* terminal = no;
  if (!no->filhos.empty()) terminal = no->filhos[0];
  string nome = terminal->dado_extra.empty() ? terminal->simb : terminal->dado_extra;
  return tipo_por_nome(nome);
}

Tipo* Tipo::extrai_tipo_literal(No_arv_parse* no) {
  if (no == NULL) return NULL;
  // CORRIGIDO: Nomes dos tokens gerados pelo seu Lexer e Gramática!
  if (no->simb == "NUM_INT") return new Tipo(INT);
  if (no->simb == "NUM_DEC") return new Tipo(FLOAT);
  if (no->simb == "BOOL_VAL") return new Tipo(BOOL);
  return NULL;
}