#include "TabelaSimbolos.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

// Construtor original (Escopo Global)
TabelaSimbolos::TabelaSimbolos() {
  pai = NULL;
}

// NOVO: Construtor para blocos internos (DECLARE)
TabelaSimbolos::TabelaSimbolos(TabelaSimbolos* pai) {
  this->pai = pai;
}

TabelaSimbolos::~TabelaSimbolos() {
  for (auto &par : variaveis) {
    delete par.second;
  }
}

bool TabelaSimbolos::adiciona(Variavel* variavel) {
  if (variavel == NULL || variavel->nome == NULL) return false;
  string nome = variavel->nome->nome;
  
  // Verifica apenas no escopo atual! Se já existir no pai, tudo bem, 
  // nós vamos sombrear (shadowing).
  if (variaveis.count(nome)) return false; 
  
  variaveis[nome] = variavel->clone();
  return true;
}

// MODIFICADO: Agora faz a busca em cascata (escopo local -> escopo pai)
Variavel* TabelaSimbolos::busca(const string& nome) const {
  auto it = variaveis.find(nome);
  
  // Achou no escopo atual? Retorna.
  if (it != variaveis.end()) {
    return it->second;
  }
  
  // Não achou? Se tiver um escopo pai, procura lá.
  if (pai != NULL) {
    return pai->busca(nome);
  }
  
  // Se não achou e não tem pai, a variável não existe.
  return NULL;
}

bool TabelaSimbolos::atribuir(const string& nome, const ValorLiteral& valor) {
  // O busca() já faz o trabalho de achar a variável certa (local ou externa)
  Variavel* var = busca(nome);
  if (var == NULL) return false;
  var->atribuir(valor);
  ultimo_valor_atribuido = var->get_valor();
  return true;
}

void TabelaSimbolos::inicializa_parametros(const vector<Variavel*>& parametros, const vector<ValorLiteral>& valores) {
  size_t limite = min(parametros.size(), valores.size());
  for (size_t i = 0; i < limite; ++i) {
    const string& nome = parametros[i]->nome->nome;
    Variavel* var = busca(nome);
    if (var != NULL) {
      var->atribuir(valores[i]);
    }
  }
}

TabelaSimbolos* TabelaSimbolos::cria_com_parametros(const vector<Variavel*>& parametros, const vector<ValorLiteral>& valores) {
  TabelaSimbolos* tabela = new TabelaSimbolos();
  for (Variavel* p : parametros) {
    tabela->adiciona(p);
  }
  tabela->inicializa_parametros(parametros, valores);
  return tabela;
}

static string trim_string(const string& texto) {
  auto inicio = texto.find_first_not_of(" \t\r\n");
  if (inicio == string::npos) return string();
  auto fim = texto.find_last_not_of(" \t\r\n");
  return texto.substr(inicio, fim - inicio + 1);
}

static bool texto_bool(const string& texto) {
  string copia = texto;
  transform(copia.begin(), copia.end(), copia.begin(), ::toupper);
  return copia == "TRUE" || copia == "1";
}

vector<ValorLiteral> TabelaSimbolos::le_parametros(const string& arquivo, const vector<Variavel*>& parametros) {
  vector<ValorLiteral> valores;
  ifstream input(arquivo);
  if (!input.is_open()) return valores;

  string linha;
  size_t indice = 0;
  while (getline(input, linha) && indice < parametros.size()) {
    string texto = trim_string(linha);
    if (texto.empty()) continue;
    ValorLiteral valor;
    valor.tipo = parametros[indice]->tipo;
    switch (valor.tipo->valor) {
      case Tipo::INT:
        valor.valor_int = atoi(texto.c_str());
        break;
      case Tipo::FLOAT:
        valor.valor_float = atof(texto.c_str());
        break;
      case Tipo::BOOL:
        valor.valor_bool = texto_bool(texto);
        break;
    }
    valores.push_back(valor);
    indice++;
  }

  return valores;
}