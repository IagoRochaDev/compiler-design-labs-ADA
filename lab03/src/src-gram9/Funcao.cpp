#include "Funcao.hpp"
#include "TabelaSimbolos.hpp"
#include "../debug-util.hpp"
#include <iostream>
#include <string>

Funcao::Funcao() {
}

// NOVO: Função estática auxiliar para varrer a árvore e extrair a "ListaDeclaracoes"
static vector<Variavel*> extrai_lista_declaracoes_locais(No_arv_parse* no) {
  vector<Variavel*> res;
  if (no == NULL) return res;

  // Se encontramos o nó de declaração de variável
  if (no->simb == "DeclVar") {
    // DeclVar -> ListaIds DOIS_PONTOS TipoVar ...
    No_arv_parse* no_lista_ids = no->filhos[0];
    
    // Helper local para extrair de forma limpa os IDs da ListaIds
    struct Helper {
      static void coletar_ids(No_arv_parse* n, vector<ID*>& ids) {
        if (n == NULL) return;
        if (n->simb == "ID") {
          ids.push_back(ID::extrai_ID(n));
          return;
        }
        if (n->simb == "ListaIds") {
          if (n->filhos.size() == 1) { // ListaIds -> ID
            ids.push_back(ID::extrai_ID(n->filhos[0]));
          } else if (n->filhos.size() == 3) { // ListaIds -> ListaIds VIRGULA ID
            coletar_ids(n->filhos[0], ids);
            ids.push_back(ID::extrai_ID(n->filhos[2]));
          }
        }
      }
    };

    vector<ID*> ids;
    Helper::coletar_ids(no_lista_ids, ids);

    // Extrai o Tipo correto lidando com a regra intermediária TipoVar -> Acesso
    Tipo* tipo = NULL;
    if (no->filhos.size() >= 3 && no->filhos[2] != NULL) {
      No_arv_parse* no_tipo_var = no->filhos[2];
      // Se o primeiro filho for "Acesso", enviamos o Acesso para o Tipo extrair o ID interno
      if (!no_tipo_var->filhos.empty() && no_tipo_var->filhos[0]->simb == "Acesso") {
        tipo = Tipo::extrai_Tipo(no_tipo_var->filhos[0]);
      } else {
        tipo = Tipo::extrai_Tipo(no_tipo_var);
      }
    }

    // Instancia as variáveis locais encontradas
    for (ID* id : ids) {
      if (id != NULL) {
        res.push_back(new Variavel(id, tipo));
      }
    }
    return res;
  }

  // Caminha recursivamente por toda a árvore de ListaDeclaracoes
  for (size_t i = 0; i < no->filhos.size(); ++i) {
    vector<Variavel*> do_filho = extrai_lista_declaracoes_locais(no->filhos[i]);
    res.insert(res.end(), do_filho.begin(), do_filho.end());
  }
  return res;
}

Funcao* Funcao::extrai_funcao(No_arv_parse *no) {
  if (no == NULL) return NULL;

  cerr << "DEBUG extrai_funcao: regra=" << no->regra << " simb=" << no->simb << " filhos=" << no->filhos.size() << endl;

  if (no->simb == "Programa" && no->regra == 1) { 
    // Procura por funções internas declaradas no escopo do programa principal
    for (int i = 0; i < no->filhos.size(); ++i) {
      if (no->filhos[i] != NULL) {
        cerr << "  -> Programa filho " << i << " = " << no->filhos[i]->simb << " regra=" << no->filhos[i]->regra << "\n";
      }
      Funcao* nested = extrai_funcao(no->filhos[i]);
      if (nested != NULL) {
        return nested;
      }
    }
    cerr << "  -> Encontrou Programa (regra 1), retornando procedure main" << endl;
    Funcao* res = new Funcao();
    res->tipo_retorno = NULL;
    res->nome_funcao = ID::extrai_ID(no->filhos[2]);
    res->parametros = vector<Variavel*>();
    // CORREÇÃO: Extrai também as variáveis locais do procedimento principal (índice 4)
    res->variaveis_locais = extrai_lista_declaracoes_locais(no->filhos[4]);
    res->comandos = Comando::extrai_lista_comandos(no->filhos[6]);
    return res;
  }
  
  if (no->simb == "DeclFunc" && no->regra == 32) {
    cerr << "  -> Encontrou DeclFunc (regra " << no->regra << "), extraindo..." << endl;
    if (no->filhos.size() >= 9) {
      Funcao* res = new Funcao();
      res->tipo_retorno = Tipo::extrai_Tipo(no->filhos[4]);
      res->nome_funcao = ID::extrai_ID(no->filhos[1]);
      res->parametros = Variavel::extrai_lista_parametros(no->filhos[2]);
      
      // CORREÇÃO: Mapeia e extrai a variável "resultado" da ListaDeclaracoes (índice 6)
      res->variaveis_locais = extrai_lista_declaracoes_locais(no->filhos[6]);
      
      res->comandos = Comando::extrai_lista_comandos(no->filhos[8]);
      cerr << "  -> DeclFunc extraido com sucesso" << endl;
      return res;
    } else {
      cerr << "  -> DeclFunc com tamanho insuficiente (" << no->filhos.size() << " filhos)" << endl;
    }
  }

  for (int i = 0; i < no->filhos.size(); ++i) {
    Funcao* res = extrai_funcao(no->filhos[i]);
    if (res != NULL) {
      cerr << "  -> Encontrou em filho " << i << endl;
      return res;
    }
  }

  return NULL;
}

TabelaSimbolos* Funcao::cria_memoria_execucao(const vector<ValorLiteral>& valores) const {
  // Inicializa a tabela contendo os parâmetros recebidos (a e b)
  TabelaSimbolos* tabela = TabelaSimbolos::cria_com_parametros(parametros, valores);
  
  // CORREÇÃO: Registra e aloca espaço para cada variável local na memória antes da execução
  for (Variavel* var_local : variaveis_locais) {
    tabela->adiciona(var_local);
  }
  
  return tabela;
}

TabelaSimbolos* Funcao::cria_memoria_execucao(const vector<Variavel*>& parametros, const string& arquivo_params) {
  vector<ValorLiteral> valores = TabelaSimbolos::le_parametros(arquivo_params, parametros);
  return TabelaSimbolos::cria_com_parametros(parametros, valores);
}

void debug_comandos(const vector<Comando*> &coms, int tab) {
  for (int ic = 0; ic < coms.size(); ++ic) {
    if (coms[ic] != NULL) {
      coms[ic]->debug_com_tab(tab+1);
    } else {
      tab3(tab+1);
      cerr << "[COMANDO_NULO]" << endl;
    }
  }  
}

void Funcao::debug() {
  std::string nome_retorno_seguro = (tipo_retorno == NULL) ? "void" : tipo_retorno->nome();
  std::string nome_funcao_seguro  = (nome_funcao == NULL) ? "NULL" : nome_funcao->nome;

  cerr << "Funcao:[retorno=" << nome_retorno_seguro << "][nome=" << nome_funcao_seguro << "]" << endl;
  cerr << "      (Param:(";
  for (int i_par = 0; i_par < parametros.size(); ++i_par) {
    cerr << ((parametros[i_par])->tipo == NULL ? "TIPO_INVALIDO" : (parametros[i_par])->tipo->nome()) << " " <<
      (parametros[i_par])->nome->nome << ", ";
  }
  cerr << ") { " << endl;
  
  // Imprime também no debug as variáveis locais encontradas
  for (size_t i_var = 0; i_var < variaveis_locais.size(); ++i_var) {
    if (variaveis_locais[i_var] != NULL) {
      variaveis_locais[i_var]->debug_com_tab(2);
    }
  }

  debug_comandos(comandos, 1);
  cerr << "}" <<  endl;
}