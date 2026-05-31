#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>   
#include <sstream>
#include <map>
#include <stack>
using namespace std;

#include "Gramatica.hpp"
#include "TabelaLR1.hpp"
#include "Arvore.hpp"
#include "Parser.hpp"
#include "Funcao.hpp"
#include "Analisador.hpp"
#include "TabelaSimbolos.hpp"
// INCLUDE NOVO PARA O LAB 04
#include "FrameFuncao.hpp" 

int main(int argc, char * argv[]) {
  if (argc != 3 && argc != 1 && argc != 4) {
    cerr << "Parametros: 1) csv com gramática 2) csv com tabela LR1 [3) arquivo parametros opcional]" << endl;
    return 1;
  }
  string nome_gramatica, nome_tab_lr1, arquivo_params;
  if (argc == 1) {
    // valores padrao
    nome_gramatica = string("gramatica9.site");
    nome_tab_lr1 = string("tabela_lr1.conf");
  } else {
    nome_gramatica = string(argv[1]);
    nome_tab_lr1 = string(argv[2]);
    if (argc == 4) arquivo_params = string(argv[3]);
  }

  ifstream arq_gramatica(nome_gramatica);
  ifstream arq_tabela_lr1(nome_tab_lr1);
  if (arq_tabela_lr1.fail() || arq_gramatica.fail()) {
    cerr << "Falha ao abrir arquivos: " << 
      ((arq_gramatica.fail()) ? nome_gramatica : "") << ", " << 
      ((arq_tabela_lr1.fail()) ? nome_tab_lr1 : "") << endl;
    return 1;    
  }
  Parser parser(arq_gramatica, arq_tabela_lr1);
  // parser.tabela.debug(); // Comentado para poluir menos a tela
  // parser.gram.debug();   // Comentado para poluir menos a tela

  
  Arvore_parse arv = parser.executa_parse(cin);
  cerr << "Parse executado" << endl;
  // arv.debug(); // Comentado para focar no Lab 4
  
  Funcao* func = Funcao::extrai_funcao(arv.raiz);
  if (func == NULL) {
    cerr << "ERRO: Nenhuma funcao encontrada na arvore de parse" << endl;
    return 1;
  }
  func->debug();
  
  // =======================================================
  // INÍCIO DA EXECUÇÃO DO LAB 04 (LAYOUT DO FRAME)
  // =======================================================
  cout << "\n--- Gerando Layout do Frame (Lab 04) ---" << endl;
  FrameFuncao* frame = FrameFuncao::gera_frame_de_funcao(func);
  if (frame != NULL) {
      frame->imprimir();
  }
  cout << "----------------------------------------\n" << endl;
  // =======================================================


  // Preparar parâmetros: aceita arquivo de parâmetros opcional
  vector<ValorLiteral> parametros_passados;
  if (!arquivo_params.empty()) {
    parametros_passados = TabelaSimbolos::le_parametros(arquivo_params, func->parametros);
    if (parametros_passados.empty()) {
      cerr << "Aviso: nenhum parametro lido do arquivo: " << arquivo_params << endl;
    }
  } else {
    // Exemplo padrão caso não haja arquivo de parâmetros
    for (int i = 1; i <= 3; ++i) {
      ValorLiteral valor_parametro;
      valor_parametro.tipo = new Tipo(Tipo::INT);
      valor_parametro.valor_int = i*10;
      parametros_passados.push_back(valor_parametro);
    }
  }

  Analisador ana;
  cout << "Retorno calculado:" << endl;
  try {
    cout << ana.calcula_retorno(func, parametros_passados).como_string() << endl;
  } catch (const runtime_error& erro) {
    cerr << "\n[Erro em Tempo de Execucao]: " << erro.what() << endl;
    return 1; 
  }
}