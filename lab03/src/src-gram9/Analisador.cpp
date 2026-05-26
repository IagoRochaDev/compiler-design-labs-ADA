#include "Analisador.hpp"
#include "TabelaSimbolos.hpp"
#include <iostream>
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
  delete memoria;
  return resultado;
}
