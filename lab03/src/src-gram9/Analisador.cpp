#include "Analisador.hpp"
#include "TabelaSimbolos.hpp"
#include <iostream>
#include <stdexcept> 

using namespace std;

ValorLiteral Analisador::calcula_retorno(Funcao *f, const vector<ValorLiteral> &params) {
  ValorLiteral resultado;
  resultado.tipo = NULL;
  if (f == NULL) return resultado;

  TabelaSimbolos* memoria = f->cria_memoria_execucao(params);

  
  for (size_t i = 0; i < f->comandos.size(); ++i) {
    if (f->comandos[i] != NULL) {
      if (!f->comandos[i]->executa(memoria)) {
        break; 
      }
    }
  }

  
  resultado = memoria->ultimo_valor_atribuido;

  
  
  if (f->tipo_retorno != NULL && resultado.tipo != NULL) {
    
    
    string tipo_esperado = f->tipo_retorno->nome();
    string tipo_obtido = resultado.tipo->nome();

    if (tipo_esperado != tipo_obtido) {
      string nome_func = (f->nome_funcao != NULL) ? f->nome_funcao->nome : "desconhecida";
      
      delete memoria; 
      
      throw runtime_error("Erro Semantico na funcao '" + nome_func + 
                          "': Esperava retornar " + tipo_esperado + 
                          ", mas tentou retornar " + tipo_obtido);
    }
  }
  

  delete memoria;
  return resultado;
}