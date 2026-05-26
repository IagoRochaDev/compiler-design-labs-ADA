#include "Variavel.hpp"
#include "ID.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

Variavel::Variavel() {
  tipo = NULL;
  nome = NULL;
  inicializado = false;
}

Variavel::Variavel(ID* nome, Tipo* tipo) {
  this->nome = nome;
  this->tipo = tipo;
  this->inicializado = false;
}

Variavel* Variavel::clone() const {
  Variavel* copia = new Variavel(this->nome, this->tipo);
  copia->valor = this->valor;
  copia->inicializado = this->inicializado;
  return copia;
}

void Variavel::atribuir(const ValorLiteral& valor) {
  if (tipo == NULL || valor.tipo == NULL) return;
  if (tipo->valor == valor.tipo->valor) {
    this->valor = valor;
    this->inicializado = true;
    return;
  }
  switch (tipo->valor) {
  case Tipo::INT:
    if (valor.tipo->valor == Tipo::FLOAT) {
      this->valor.valor_int = (int) valor.valor_float;
      this->valor.tipo = tipo;
      this->inicializado = true;
    }
    break;
  case Tipo::FLOAT:
    if (valor.tipo->valor == Tipo::INT) {
      this->valor.valor_float = (float) valor.valor_int;
      this->valor.tipo = tipo;
      this->inicializado = true;
    }
    break;
  case Tipo::BOOL:
    if (valor.tipo->valor == Tipo::INT) {
      this->valor.valor_bool = (valor.valor_int != 0);
      this->valor.tipo = tipo;
      this->inicializado = true;
    }
    break;
  }
}

bool Variavel::esta_inicializada() const {
  return inicializado;
}

const ValorLiteral& Variavel::get_valor() const {
  return valor;
}

Tipo* Variavel::get_tipo() const {
  return tipo;
}

static vector<ID*> extrai_lista_ids(No_arv_parse* no) {
  if (no == NULL) return vector<ID*>();
  if (no->regra == 20) { // ListaIds -> ID
    return vector<ID*>{ ID::extrai_ID(no->filhos[0]) };
  }
  vector<ID*> res = extrai_lista_ids(no->filhos[0]);
  res.push_back(ID::extrai_ID(no->filhos[2]));
  return res;
}

static vector<Variavel*> extrai_variaveis_parametro(No_arv_parse* no) {
  vector<ID*> ids = extrai_lista_ids(no->filhos[0]);
  Tipo* tipo = Tipo::extrai_Tipo(no->filhos[3]);
  vector<Variavel*> res;
  for (int i = 0; i < ids.size(); ++i) {
    Variavel* var = new Variavel(ids[i], tipo);
    res.push_back(var);
  }
  return res;
}

vector<Variavel*> Variavel::extrai_lista_nao_vazia_parametros(No_arv_parse* no) {
  // ListaParams. 35) ListaParams -> Parametro  34) ListaParams -> ListaParams PONTO_VIRGULA Parametro
  if (no->regra == 35) {
    return extrai_variaveis_parametro(no->filhos[0]);
  }
  vector<Variavel*> res = extrai_lista_nao_vazia_parametros(no->filhos[0]);
  vector<Variavel*> restante = extrai_variaveis_parametro(no->filhos[2]);
  res.insert(res.end(), restante.begin(), restante.end());
  return res;
}

vector<Variavel *> Variavel::extrai_lista_parametros(No_arv_parse* no) {
  // ParametrosFunc. 33) ParametrosFunc ->  32) ParametrosFunc -> ABRE_PARENTESES ListaParams FECHA_PARENTESES
  if (no->regra == 33) return vector<Variavel*>();
  return extrai_lista_nao_vazia_parametros(no->filhos[1]);
}

Variavel* Variavel::extrai_variavel_P(No_arv_parse* no) {
  Variavel* res = new Variavel();
  // Esta funcao não e usada diretamente pela gramática Ada atual.
  res->tipo = Tipo::extrai_Tipo(no->filhos[0]);
  res->nome = ID::extrai_ID(no->filhos[1]);
  return res;
}

void Variavel::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "[" << nome->nome << ":" <<  (tipo == NULL ? "TIPO_INVALIDO" : tipo->nome()) << "] Variavel Declarada" << endl;
  if (inicializado) {
    cerr << "  Valor = " << valor.como_string() << endl;
  }
}
