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

  if (no->regra == 1) { // S -> Programa
    return extrai_funcao(no->filhos[0]);
  }
  if (no->regra == 2) { // Programa -> ListaContexto PROCEDURE ID IS ListaDeclaracoes BEGIN ListaComandos END ID PONTO_VIRGULA
    Funcao* res = new Funcao();
    res->tipo_retorno = NULL;
    res->nome_funcao = ID::extrai_ID(no->filhos[2]);
    res->parametros = vector<Variavel*>();
    res->comandos = Comando::extrai_lista_comandos(no->filhos[6]);
    return res;
  }
  if (no->regra == 31) { // DeclFunc -> FUNCTION ID ParametrosFunc RETURN Acesso IS ListaDeclaracoes BEGIN ListaComandos END ID PONTO_VIRGULA
    Funcao* res = new Funcao();
    res->tipo_retorno = Tipo::extrai_Tipo(no->filhos[4]);
    res->nome_funcao = ID::extrai_ID(no->filhos[1]);
    res->parametros = Variavel::extrai_lista_parametros(no->filhos[2]);
    res->comandos = Comando::extrai_lista_comandos(no->filhos[8]);
    return res;
  }

  // Try recursively on children
  for (int i = 0; i < no->filhos.size(); ++i) {
    Funcao* res = extrai_funcao(no->filhos[i]);
    if (res != NULL) return res;
  }

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
