#include "Analisador.hpp"
#include "TabelaSimbolos.hpp"
#include <iostream>
#include <stdexcept> // NOVO: Para usar std::runtime_error

using namespace std;

ValorLiteral Analisador::calcula_retorno(Funcao *f, const vector<ValorLiteral> &params) {
  ValorLiteral resultado;
  resultado.tipo = NULL;
  if (f == NULL) return resultado;

  TabelaSimbolos* memoria = f->cria_memoria_execucao(params);

  // Executa os comandos linha a linha
  for (size_t i = 0; i < f->comandos.size(); ++i) {
    if (f->comandos[i] != NULL) {
      if (!f->comandos[i]->executa(memoria)) {
        break; // ComandoReturn ou erro parou a execução
      }
    }
  }

  // Captura o que o return deixou na memória
  resultado = memoria->ultimo_valor_atribuido;

  // --- TRAVA DE SEGURANÇA SEMÂNTICA ---
  // Verifica se a função tem um tipo de retorno exigido e se o valor retornado também tem um tipo
  if (f->tipo_retorno != NULL && resultado.tipo != NULL) {
    
    // Compara os tipos pelo nome (ex: "INT" vs "BOOL")
    string tipo_esperado = f->tipo_retorno->nome();
    string tipo_obtido = resultado.tipo->nome();

    if (tipo_esperado != tipo_obtido) {
      string nome_func = (f->nome_funcao != NULL) ? f->nome_funcao->nome : "desconhecida";
      
      delete memoria; // Evita vazamento de memória antes de lançar a exceção
      
      throw runtime_error("Erro Semantico na funcao '" + nome_func + 
                          "': Esperava retornar " + tipo_esperado + 
                          ", mas tentou retornar " + tipo_obtido);
    }
  }
  // ------------------------------------

  delete memoria;
  return resultado;
}