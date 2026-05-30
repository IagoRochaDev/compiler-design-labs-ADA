#ifndef _TABELA_SIMBOLOS_HPP_
#define _TABELA_SIMBOLOS_HPP_

#include "Variavel.hpp"
#include "ValorLiteral.hpp"
#include <map>
#include <string>
#include <vector>

using namespace std;

class TabelaSimbolos {
public:
  map<string, Variavel*> variaveis;
  
  // NOVO: Ponteiro para a tabela do escopo externo
  TabelaSimbolos* pai; 
  
  ValorLiteral ultimo_valor_atribuido;

  TabelaSimbolos();
  // NOVO: Construtor que recebe a tabela pai
  TabelaSimbolos(TabelaSimbolos* pai); 
  
  ~TabelaSimbolos();

  bool adiciona(Variavel* variavel);
  Variavel* busca(const string& nome) const;
  bool atribuir(const string& nome, const ValorLiteral& valor);

  void inicializa_parametros(const vector<Variavel*>& parametros, const vector<ValorLiteral>& valores);
  static TabelaSimbolos* cria_com_parametros(const vector<Variavel*>& parametros, const vector<ValorLiteral>& valores);
  static vector<ValorLiteral> le_parametros(const string& arquivo, const vector<Variavel*>& parametros);
};

#endif