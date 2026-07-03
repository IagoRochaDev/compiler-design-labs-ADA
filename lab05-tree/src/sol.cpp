#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>   
#include <sstream>
#include <map>
#include <stack>

#include "Gramatica.hpp"
#include "TabelaLR1.hpp"
#include "Arvore.hpp"
#include "Parser.hpp"
#include "Funcao.hpp"
#include "Analisador.hpp"
#include "TabelaSimbolos.hpp"
#include "src-gram9/frame/FrameFuncao.hpp" 
#include "src-gram9/TradutorIR.hpp"

using namespace std;

int main(int argc, char * argv[]) {
  if (argc != 3 && argc != 1 && argc != 4) {
    cerr << "Parametros: 1) csv com gramática 2) csv com tabela LR1 [3) arquivo parametros opcional]" << endl;
    return 1;
  }
  string nome_gramatica, nome_tab_lr1, arquivo_params;
  if (argc == 1) {
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
    cerr << "Falha ao abrir arquivos..." << endl;
    return 1;    
  }
  
  Parser parser(arq_gramatica, arq_tabela_lr1);
  Arvore_parse arv = parser.executa_parse(cin);
  cerr << "Parse executado" << endl;
  
  Funcao* func = Funcao::extrai_funcao(arv.raiz);
  if (func == NULL) {
    cerr << "ERRO: Nenhuma funcao encontrada na arvore de parse" << endl;
    return 1;
  }
  
  // EXECUÇÃO DO LAB 04 (LAYOUT DO FRAME)
  cout << "\n--- Gerando Layout do Frame (Lab 04) ---" << endl;
  FrameFuncao* frame = FrameFuncao::gera_frame_de_funcao(func);
  if (frame != NULL) {
      frame->imprimir();
  }
  cout << "---------------------------------------\n" << endl;


  // EXECUÇÃO DO LAB 05 (REPRESENTAÇÃO INTERMEDIÁRIA)
  cout << "\n------- Gerando IR (Lab 05) -----------" << endl;
  TradutorIR tradutor(frame);
  Stm* arvore_ir = tradutor.traduzir_funcao(func);
  
  // Imprime a IR - um comando por linha!
  tradutor.imprimir_ir(arvore_ir);
  
  cout << "---------------------------------------\n" << endl;

  // Como o Lab 05 foca em gerar o código Assembly/IR, 
  // geralmente não precisamos mais rodar o "interpretador" antigo.
  // Você pode comentar a chamada de "ana.calcula_retorno" se quiser,
  // ou deixá-la aí apenas para validação.
  
  return 0;
}