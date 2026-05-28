  #include "Funcao.hpp"
  #include "TabelaSimbolos.hpp"
  #include "../debug-util.hpp"
  #include <iostream>
  #include <string>

  Funcao::Funcao() {
  }

  Funcao* Funcao::extrai_funcao(No_arv_parse *no) {
  if (no == NULL) return NULL;

  cerr << "DEBUG extrai_funcao: regra=" << no->regra << " simb=" << no->simb << " filhos=" << no->filhos.size() << endl;

  if (no->simb == "Programa" && no->regra == 1) { // Programa -> ListaContexto PROCEDURE ID IS ListaDeclaracoes BEGIN ListaComandos END ID PONTO_VIRGULA ListaFuncoesTop
    // Se houver funções internas, extraímos a primeira função declarada dentro de ListaFuncoesTop.
    for (int i = 0; i < no->filhos.size(); ++i) {
      if (no->filhos[i] != NULL) {
        cerr << "  -> Programa filho " << i << " = " << no->filhos[i]->simb << " regra=" << no->filhos[i]->regra << "\n";
      }
      Funcao* nested = extrai_funcao(no->filhos[i]);
      if (nested != NULL) {
        return nested;
      }
    }
    // Caso não haja funções internas, retornamos o procedimento principal como função de entrada.
    cerr << "  -> Encontrou Programa (regra 1), retornando procedure main" << endl;
    Funcao* res = new Funcao();
    res->tipo_retorno = NULL;
    res->nome_funcao = ID::extrai_ID(no->filhos[2]);
    res->parametros = vector<Variavel*>();
    res->comandos = Comando::extrai_lista_comandos(no->filhos[6]);
    return res;
  }
  
  // DeclFunc parser rule is actual line 33 -> regra 32
  if (no->simb == "DeclFunc" && no->regra == 32) {
    cerr << "  -> Encontrou DeclFunc (regra " << no->regra << "), extraindo..." << endl;
    if (no->filhos.size() >= 9) {
      // DeclFunc structure: FUNCTION ID ParametrosFunc RETURN Acesso IS ListaDeclaracoes BEGIN ListaComandos END ID PONTO_VIRGULA
      Funcao* res = new Funcao();
      res->tipo_retorno = Tipo::extrai_Tipo(no->filhos[4]);
      res->nome_funcao = ID::extrai_ID(no->filhos[1]);
      res->parametros = Variavel::extrai_lista_parametros(no->filhos[2]);
      res->comandos = Comando::extrai_lista_comandos(no->filhos[8]);
      cerr << "  -> DeclFunc extraido com sucesso" << endl;
      return res;
    } else {
      cerr << "  -> DeclFunc com tamanho insuficiente (" << no->filhos.size() << " filhos)" << endl;
    }
  }

  // Try recursively on children
  cerr << "  -> Procurando recursivamente em " << no->filhos.size() << " filhos" << endl;
  for (int i = 0; i < no->filhos.size(); ++i) {
    Funcao* res = extrai_funcao(no->filhos[i]);
    if (res != NULL) {
      cerr << "  -> Encontrou em filho " << i << endl;
      return res;
    }
  }

  cerr << "  -> Nao encontrou, retornando NULL" << endl;
  return NULL;
}

TabelaSimbolos* Funcao::cria_memoria_execucao(const vector<ValorLiteral>& valores) const {
  return TabelaSimbolos::cria_com_parametros(parametros, valores);
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
  // CORREÇÃO: Cria variáveis seguras para evitar o crash com ponteiros nulos
  std::string nome_retorno_seguro = (tipo_retorno == NULL) ? "void" : tipo_retorno->nome();
  std::string nome_funcao_seguro  = (nome_funcao == NULL) ? "NULL" : nome_funcao->nome;

  if (tipo_retorno == NULL) cerr << "TR NULL" << endl;
  if (nome_funcao == NULL) cerr << "NF NULL" << endl;

  // Agora usamos as strings seguras aqui
  cerr << "Funcao:[retorno=" << nome_retorno_seguro << "][nome=" << nome_funcao_seguro << "]" << endl;
  cerr << "      (Param:(";
  fflush(stderr);
  for (int i_par = 0; i_par < parametros.size(); ++i_par) {
    cerr << ((parametros[i_par])->tipo == NULL ? "TIPO_INVALIDO" : (parametros[i_par])->tipo->nome()) << " " <<
      (parametros[i_par])->nome->nome << ", ";
  }
  cerr << ") { " << endl;
  debug_comandos(comandos, 1);
  cerr << "}" <<  endl;
}
