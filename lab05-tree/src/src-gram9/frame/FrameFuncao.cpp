#include "FrameFuncao.hpp"
#include "../ComandoAtribuicao.hpp"
#include "../ComandoIf.hpp"
#include "../ComandoWhile.hpp"
#include "../ComandoDeclaracao.hpp"
#include "../ComandoRetorno.hpp"
#include "../ComandoDeclare.hpp"
#include "../ExpressaoSoma.hpp"
#include "../ExpressaoSubtracao.hpp"
#include "../ExpressaoMultiplicacao.hpp"
#include "../ExpressaoDivisao.hpp"
#include "../ExpressaoMod.hpp"
#include "../ExpressaoIgualdade.hpp"
#include "../ExpressaoMenor.hpp"
#include "../ExpressaoNegacao.hpp"
#include "../ExpressaoVariavel.hpp"
#include <iostream>

FrameFuncao::FrameFuncao()
{
  tamanho_frame = 0;
  n_param_entrada = 0;
  n_maximo_param_saida = 0;
  n_pseudo_registradores = 0;
  n_variaveis_no_frame = 0;
}

FrameFuncao *FrameFuncao::gera_frame_de_funcao(Funcao *fun)
{
  if (fun == NULL)
    return NULL;

  FrameFuncao *frame = new FrameFuncao();
  frame->analisar_comandos_por_escape(fun->comandos);
  frame->calcular_layout(fun);
  frame->aplicar_acessos_comandos(fun->comandos);

  return frame;
}

void FrameFuncao::imprimir() const
{
  std::cout << "\n========= RESUMO DO FRAME =========" << std::endl;
  std::cout << "Tamanho do Frame:           " << tamanho_frame << " bytes" << std::endl;
  std::cout << "N. Parametros Entrada:      " << n_param_entrada << std::endl;
  std::cout << "N. Maximo Parametros Saida: " << n_maximo_param_saida << std::endl;
  std::cout << "N. Pseudo-Registradores:    " << n_pseudo_registradores << std::endl;
  std::cout << "N. Variaveis no Frame:      " << n_variaveis_no_frame << std::endl;
  std::cout << "Variaveis alocadas no Frame: ";
  for (const string &nome : variaveis_no_frame_set)
  {
    std::cout << nome << " ";
  }
  std::cout << "\n===================================\n"
            << std::endl;
}

void FrameFuncao::analisar_comandos_por_escape(const vector<Comando *> &comandos)
{
  for (Comando *cmd : comandos)
  {
    analisar_comando_por_escape(cmd);
  }
}
void FrameFuncao::analisar_comando_por_escape(Comando *cmd)
{
  if (cmd == NULL)
    return;

  if (auto *cAtrib = dynamic_cast<ComandoAtribuicao *>(cmd))
  {
    analisar_expressao_por_chamadas(cAtrib->direita);
  }
  else if (auto *cIf = dynamic_cast<ComandoIf *>(cmd))
  {
    analisar_expressao_por_chamadas(cIf->condicao);
    analisar_comandos_por_escape(cIf->comandos_then);
    analisar_comandos_por_escape(cIf->comandos_else);
  }
  else if (auto *cWhile = dynamic_cast<ComandoWhile *>(cmd))
  {
    analisar_expressao_por_chamadas(cWhile->condicao);
    analisar_comandos_por_escape(cWhile->comandos);
  }
  else if (auto *cDecl = dynamic_cast<ComandoDeclare *>(cmd))
  {
    analisar_comandos_por_escape(cDecl->comandos);
  }
  else if (auto *cRet = dynamic_cast<ComandoRetorno *>(cmd))
  {
    analisar_expressao_por_chamadas(cRet->expressao);
  }
}

// Substitua o método analisar_expressao_por_chamadas por este:
void FrameFuncao::analisar_expressao_por_chamadas(Expressao *exp)
{
  if (exp == NULL)
    return;

  if (auto *eVariavel = dynamic_cast<ExpressaoVariavel *>(exp))
  {
    if (!eVariavel->argumentos.empty())
    {
      for (Expressao *arg : eVariavel->argumentos)
      {
        coletar_variaveis_de_expressao(arg);
      }

      int qtd_args = eVariavel->argumentos.size();
      if (qtd_args > n_maximo_param_saida)
      {
        n_maximo_param_saida = qtd_args;
      }
    }
  }
  else if (auto *eSoma = dynamic_cast<ExpressaoSoma *>(exp))
  {
    analisar_expressao_por_chamadas(eSoma->esquerda);
    analisar_expressao_por_chamadas(eSoma->direita);
  }
  else if (auto *eSub = dynamic_cast<ExpressaoSubtracao *>(exp))
  {
    analisar_expressao_por_chamadas(eSub->esquerda);
    analisar_expressao_por_chamadas(eSub->direita);
  }
  else if (auto *eMult = dynamic_cast<ExpressaoMultiplicacao *>(exp))
  {
    analisar_expressao_por_chamadas(eMult->esquerda);
    analisar_expressao_por_chamadas(eMult->direita);
  }
  else if (auto *eDiv = dynamic_cast<ExpressaoDivisao *>(exp))
  {
    analisar_expressao_por_chamadas(eDiv->esquerda);
    analisar_expressao_por_chamadas(eDiv->direita);
  }
  else if (auto *eMod = dynamic_cast<ExpressaoMod *>(exp))
  {
    analisar_expressao_por_chamadas(eMod->esquerda);
    analisar_expressao_por_chamadas(eMod->direita);
  }
  else if (auto *eIgual = dynamic_cast<ExpressaoIgualdade *>(exp))
  {
    analisar_expressao_por_chamadas(eIgual->esquerda);
    analisar_expressao_por_chamadas(eIgual->direita);
  }
  else if (auto *eMenor = dynamic_cast<ExpressaoMenor *>(exp))
  {
    analisar_expressao_por_chamadas(eMenor->esquerda);
    analisar_expressao_por_chamadas(eMenor->direita);
  }
  else if (auto *eNeg = dynamic_cast<ExpressaoNegacao *>(exp))
  {
    analisar_expressao_por_chamadas(eNeg->expressao);
  }
}

void FrameFuncao::coletar_variaveis_de_expressao(Expressao *exp)
{
  if (exp == NULL)
    return;

  if (auto *eVariavel = dynamic_cast<ExpressaoVariavel *>(exp))
  {
    if (eVariavel->argumentos.empty())
    {
      variaveis_no_frame_set.insert(eVariavel->nome->nome);
    }
    else
    {
      for (Expressao *arg : eVariavel->argumentos)
      {
        coletar_variaveis_de_expressao(arg);
      }
    }
  }
  else if (auto *eSoma = dynamic_cast<ExpressaoSoma *>(exp))
  {
    coletar_variaveis_de_expressao(eSoma->esquerda);
    coletar_variaveis_de_expressao(eSoma->direita);
  }
  else if (auto *eSub = dynamic_cast<ExpressaoSubtracao *>(exp))
  {
    coletar_variaveis_de_expressao(eSub->esquerda);
    coletar_variaveis_de_expressao(eSub->direita);
  }
  else if (auto *eMult = dynamic_cast<ExpressaoMultiplicacao *>(exp))
  {
    coletar_variaveis_de_expressao(eMult->esquerda);
    coletar_variaveis_de_expressao(eMult->direita);
  }
  else if (auto *eDiv = dynamic_cast<ExpressaoDivisao *>(exp))
  {
    coletar_variaveis_de_expressao(eDiv->esquerda);
    coletar_variaveis_de_expressao(eDiv->direita);
  }
  else if (auto *eMod = dynamic_cast<ExpressaoMod *>(exp))
  {
    coletar_variaveis_de_expressao(eMod->esquerda);
    coletar_variaveis_de_expressao(eMod->direita);
  }
  else if (auto *eIgual = dynamic_cast<ExpressaoIgualdade *>(exp))
  {
    coletar_variaveis_de_expressao(eIgual->esquerda);
    coletar_variaveis_de_expressao(eIgual->direita);
  }
  else if (auto *eMenor = dynamic_cast<ExpressaoMenor *>(exp))
  {
    coletar_variaveis_de_expressao(eMenor->esquerda);
    coletar_variaveis_de_expressao(eMenor->direita);
  }
  else if (auto *eNeg = dynamic_cast<ExpressaoNegacao *>(exp))
  {
    coletar_variaveis_de_expressao(eNeg->expressao);
  }
}

void FrameFuncao::calcular_layout(Funcao *fun)
{
  if (fun == NULL)
    return;

  int offset_param = 8;
  for (Variavel *param : fun->parametros)
  {
    string nome_param = param->nome->nome;

    FrameAcessoNoFrame *acesso = new FrameAcessoNoFrame(offset_param);
    acessos_por_variavel[nome_param] = acesso;

    offset_param += 8;
    n_param_entrada++;
  }

  int offset_frame = -40;
  int id_registrador = 1;

  for (Variavel *var : fun->variaveis_locais)
  {
    string nome_var = var->nome->nome;

    if (variaveis_no_frame_set.count(nome_var) > 0)
    {
      FrameAcessoNoFrame *acesso = new FrameAcessoNoFrame(offset_frame);
      acessos_por_variavel[nome_var] = acesso;

      offset_frame -= 8;
      n_variaveis_no_frame++;
    }
    else
    {
      FrameAcessoTemp *acesso = new FrameAcessoTemp(id_registrador);
      acessos_por_variavel[nome_var] = acesso;

      id_registrador++;
      n_pseudo_registradores++;
    }
  }

  tamanho_frame = 40 + (8 * n_variaveis_no_frame);
}

// ----------------------------------------------------------------------
// PASSO 4: APLICAR ACESSOS NA ÁRVORE (EXPRESSAO VARIAVEL)
// ----------------------------------------------------------------------
void FrameFuncao::aplicar_acessos_comandos(const vector<Comando*>& comandos) {
    for (Comando* cmd : comandos) {
        aplicar_acessos_comando(cmd);
    }
}

void FrameFuncao::aplicar_acessos_comando(Comando* cmd) {
    if (cmd == NULL) return;

    if (auto* cAtrib = dynamic_cast<ComandoAtribuicao*>(cmd)) {
        aplicar_acessos_expressao(cAtrib->direita);
        
        if (cAtrib->esquerda != NULL) {
            string nome_esq = cAtrib->esquerda->nome;
            if (acessos_por_variavel.count(nome_esq) > 0) {
                cAtrib->acesso_frame = acessos_por_variavel[nome_esq];
            }
        }
    }
    else if (auto* cIf = dynamic_cast<ComandoIf*>(cmd)) {
        aplicar_acessos_expressao(cIf->condicao);
        aplicar_acessos_comandos(cIf->comandos_then);
        aplicar_acessos_comandos(cIf->comandos_else);
    }
    else if (auto* cWhile = dynamic_cast<ComandoWhile*>(cmd)) {
        aplicar_acessos_expressao(cWhile->condicao);
        aplicar_acessos_comandos(cWhile->comandos);
    }
    else if (auto* cDecl = dynamic_cast<ComandoDeclare*>(cmd)) {
        aplicar_acessos_comandos(cDecl->comandos);
    }
    else if (auto* cRet = dynamic_cast<ComandoRetorno*>(cmd)) {
        aplicar_acessos_expressao(cRet->expressao);
    }
}

void FrameFuncao::aplicar_acessos_expressao(Expressao* exp) {
    if (exp == NULL) return;

    if (auto* eVariavel = dynamic_cast<ExpressaoVariavel*>(exp)) {
        string nome_var = eVariavel->nome->nome;
        
        if (acessos_por_variavel.count(nome_var) > 0) {
            eVariavel->acesso_frame = acessos_por_variavel[nome_var];
        }

        for (Expressao* arg : eVariavel->argumentos) {
            aplicar_acessos_expressao(arg);
        }
    }
    else if (auto* eSoma = dynamic_cast<ExpressaoSoma*>(exp)) {
        aplicar_acessos_expressao(eSoma->esquerda);
        aplicar_acessos_expressao(eSoma->direita);
    }
    else if (auto* eSub = dynamic_cast<ExpressaoSubtracao*>(exp)) {
        aplicar_acessos_expressao(eSub->esquerda);
        aplicar_acessos_expressao(eSub->direita);
    }
    else if (auto* eMult = dynamic_cast<ExpressaoMultiplicacao*>(exp)) {
        aplicar_acessos_expressao(eMult->esquerda);
        aplicar_acessos_expressao(eMult->direita);
    }
    else if (auto* eDiv = dynamic_cast<ExpressaoDivisao*>(exp)) {
        aplicar_acessos_expressao(eDiv->esquerda);
        aplicar_acessos_expressao(eDiv->direita);
    }
    else if (auto* eMod = dynamic_cast<ExpressaoMod*>(exp)) {
        aplicar_acessos_expressao(eMod->esquerda);
        aplicar_acessos_expressao(eMod->direita);
    }
    else if (auto* eIgual = dynamic_cast<ExpressaoIgualdade*>(exp)) {
        aplicar_acessos_expressao(eIgual->esquerda);
        aplicar_acessos_expressao(eIgual->direita);
    }
    else if (auto* eMenor = dynamic_cast<ExpressaoMenor*>(exp)) {
        aplicar_acessos_expressao(eMenor->esquerda);
        aplicar_acessos_expressao(eMenor->direita);
    }
    else if (auto* eNeg = dynamic_cast<ExpressaoNegacao*>(exp)) {
        aplicar_acessos_expressao(eNeg->expressao);
    }
}