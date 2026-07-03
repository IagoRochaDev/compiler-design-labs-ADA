#include "TradutorIR.hpp"
#include "ComandoAtribuicao.hpp"
#include "ComandoIf.hpp"
#include "ComandoWhile.hpp"
#include "ComandoRetorno.hpp"
#include "ExpressaoVariavel.hpp"
#include "ExpressaoValor.hpp"
#include "ExpressaoSoma.hpp"
#include "ExpressaoMenor.hpp"
#include "ExpressaoSubtracao.hpp"
#include "ExpressaoMultiplicacao.hpp"
#include "ExpressaoDivisao.hpp"
#include "ExpressaoMod.hpp"
#include "ExpressaoIgualdade.hpp"
#include "ExpressaoNegacao.hpp"
#include "tree/StmSeq.hpp"
#include "tree/StmMove.hpp"
#include "tree/StmLabel.hpp"
#include "tree/StmJump.hpp"
#include "tree/StmCJump.hh"
#include "tree/StmExp.hpp"
#include "tree/ExpMem.hpp"
#include "tree/ExpTemp.hpp"
#include "tree/ExpTempFramePointer.hpp"
#include "tree/ExpBinop.hpp"
#include "tree/ExpConst.hpp"
#include "tree/ExpCall.hpp"
#include "tree/ExpName.hpp"
#include "tree/OperadorSoma.hpp"
#include "tree/OperadorMaior.hpp"
#include "tree/OperadorMenor.hpp"

#include <iostream>

using namespace std;

TradutorIR::TradutorIR(FrameFuncao* f) {
    this->frame = f;
}

Stm* TradutorIR::traduzir_funcao(Funcao* func) {
    if (func == nullptr) return nullptr;
    return traduzir_comandos(func->comandos);
}

Stm* TradutorIR::traduzir_comandos(const vector<Comando*>& comandos) {
    if (comandos.empty()) return nullptr;
    
    if (comandos.size() == 1) {
        return traduzir_comando(comandos[0]);
    }

    Stm* sequencia = traduzir_comando(comandos[0]);
    for (size_t i = 1; i < comandos.size(); ++i) {
        sequencia = new StmSeq(sequencia, traduzir_comando(comandos[i]));
    }
    return sequencia;
}

Exp* TradutorIR::traduzir_expressao(Expressao* exp) {
    if (exp == nullptr) return nullptr;

    if (auto* eVariavel = dynamic_cast<ExpressaoVariavel*>(exp)) {
        if (!eVariavel->argumentos.empty()) {
            vector<Exp*> args_ir;
            for (Expressao* arg : eVariavel->argumentos) {
                args_ir.push_back(traduzir_expressao(arg));
            }

            ExpName* nome_func = new ExpName();
            if (eVariavel->nome != nullptr) {
                nome_func->nome = eVariavel->nome->nome;
            }

            ExpCall* chamada = new ExpCall();
            chamada->nome_funcao = nome_func;
            chamada->parametros = args_ir;
            return chamada;
        }

        if (auto* frameAcesso = dynamic_cast<FrameAcessoNoFrame*>(eVariavel->acesso_frame)) {
            Exp* fp = new ExpTempFramePointer();
            Exp* offset = new ExpConst(frameAcesso->posicao_no_frame);
            Operador* op_soma = new OperadorSoma();
            
            return new ExpMem(new ExpBinop(op_soma, fp, offset));
            
        } else if (auto* tempAcesso = dynamic_cast<FrameAcessoTemp*>(eVariavel->acesso_frame)) {
            return new ExpTemp(tempAcesso->id);
        }
    }
    else if (auto* eValor = dynamic_cast<ExpressaoValor*>(exp)) {
        return new ExpConst(eValor->valor->valor_int); 
    }
    else if (auto* eSoma = dynamic_cast<ExpressaoSoma*>(exp)) {
        Exp* esq = traduzir_expressao(eSoma->esquerda);
        Exp* dir = traduzir_expressao(eSoma->direita);
        Operador* op_soma = new OperadorSoma();
        
        return new ExpBinop(op_soma, esq, dir);
    }
    else if (auto* eMenor = dynamic_cast<ExpressaoMenor*>(exp)) {
        Exp* esq = traduzir_expressao(eMenor->esquerda);
        Exp* dir = traduzir_expressao(eMenor->direita);
        Operador* op_menor = new OperadorMenor();
        
        return new ExpBinop(op_menor, esq, dir);
    }
    else if (auto* eSubtracao = dynamic_cast<ExpressaoSubtracao*>(exp)) {
        Exp* esq = traduzir_expressao(eSubtracao->esquerda);
        Exp* dir = traduzir_expressao(eSubtracao->direita);
        Operador* op_subtracao = new OperadorSoma(); // TODO: criar OperadorSubtracao
        
        return new ExpBinop(op_subtracao, esq, dir);
    }
    else if (auto* eMultiplicacao = dynamic_cast<ExpressaoMultiplicacao*>(exp)) {
        Exp* esq = traduzir_expressao(eMultiplicacao->esquerda);
        Exp* dir = traduzir_expressao(eMultiplicacao->direita);
        Operador* op_mult = new OperadorSoma(); // TODO: criar OperadorMultiplicacao
        
        return new ExpBinop(op_mult, esq, dir);
    }
    else if (auto* eDivisao = dynamic_cast<ExpressaoDivisao*>(exp)) {
        Exp* esq = traduzir_expressao(eDivisao->esquerda);
        Exp* dir = traduzir_expressao(eDivisao->direita);
        Operador* op_div = new OperadorSoma(); // TODO: criar OperadorDivisao
        
        return new ExpBinop(op_div, esq, dir);
    }
    else if (auto* eMod = dynamic_cast<ExpressaoMod*>(exp)) {
        Exp* esq = traduzir_expressao(eMod->esquerda);
        Exp* dir = traduzir_expressao(eMod->direita);
        Operador* op_mod = new OperadorSoma(); // TODO: criar OperadorMod
        
        return new ExpBinop(op_mod, esq, dir);
    }
    else if (auto* eIgualdade = dynamic_cast<ExpressaoIgualdade*>(exp)) {
        Exp* esq = traduzir_expressao(eIgualdade->esquerda);
        Exp* dir = traduzir_expressao(eIgualdade->direita);
        Operador* op_igual = new OperadorSoma(); // TODO: criar OperadorIgualdade
        
        return new ExpBinop(op_igual, esq, dir);
    }
    else if (auto* eNegacao = dynamic_cast<ExpressaoNegacao*>(exp)) {
        Exp* operando = traduzir_expressao(eNegacao->expressao);
        // TODO: implementar negação
        return operando;
    }
    
    return nullptr;
}

Stm* TradutorIR::traduzir_comando(Comando* cmd) {
    if (cmd == nullptr) return nullptr;

    if (auto* cAtrib = dynamic_cast<ComandoAtribuicao*>(cmd)) {
        Exp* lado_direito = traduzir_expressao(cAtrib->direita);
        Exp* lado_esquerdo = nullptr;
        
        if (auto* frameAcesso = dynamic_cast<FrameAcessoNoFrame*>(cAtrib->acesso_frame)) {
            Exp* fp = new ExpTempFramePointer();
            Exp* offset = new ExpConst(frameAcesso->posicao_no_frame);
            Operador* op_soma = new OperadorSoma();
            lado_esquerdo = new ExpMem(new ExpBinop(op_soma, fp, offset));
        } else if (auto* tempAcesso = dynamic_cast<FrameAcessoTemp*>(cAtrib->acesso_frame)) {
            lado_esquerdo = new ExpTemp(tempAcesso->id);
        }

        return new StmMove(lado_esquerdo, lado_direito);
    }
    // Tradução do IF
    else if (auto* cIf = dynamic_cast<ComandoIf*>(cmd)) {
        Exp* condicao = traduzir_expressao(cIf->condicao);
        
        // Gera as Labels
        string l_true = "L" + to_string(cont_label++);
        string l_false = "L" + to_string(cont_label++);
        string l_fim = "L" + to_string(cont_label++);

        // Cria os blocos
        Stm* cjump = new StmCJump(condicao, l_true, l_false);
        Stm* label_true = new StmLabel(l_true);
        Stm* bloco_then = traduzir_comandos(cIf->comandos_then);
        Stm* jump_fim = new StmJump(l_fim);
        Stm* label_false = new StmLabel(l_false);
        Stm* bloco_else = traduzir_comandos(cIf->comandos_else);
        Stm* label_fim = new StmLabel(l_fim);

        // Agrupa tudo em uma grande Sequência (Seq)
        // cjump -> L_TRUE -> then -> JUMP fim -> L_FALSE -> else -> L_FIM
        Stm* seq = new StmSeq(cjump, label_true);
        if (bloco_then) {
            seq = new StmSeq(seq, new StmSeq(bloco_then, jump_fim));
        } else {
            seq = new StmSeq(seq, jump_fim);
        }
        seq = new StmSeq(seq, label_false);
        if (bloco_else) {
            seq = new StmSeq(seq, bloco_else);
        }
        seq = new StmSeq(seq, label_fim);

        return seq;
    }
    // Tradução do WHILE
    else if (auto* cWhile = dynamic_cast<ComandoWhile*>(cmd)) {
        Exp* condicao = traduzir_expressao(cWhile->condicao);
        
        string l_teste = "L" + to_string(cont_label++);
        string l_laco = "L" + to_string(cont_label++);
        string l_fim = "L" + to_string(cont_label++);

        Stm* label_teste = new StmLabel(l_teste);
        Stm* cjump = new StmCJump(condicao, l_laco, l_fim);
        Stm* label_laco = new StmLabel(l_laco);
        Stm* bloco = traduzir_comandos(cWhile->comandos);
        Stm* jump_inicio = new StmJump(l_teste);
        Stm* label_fim = new StmLabel(l_fim);

        // Agrupa tudo (L_TESTE -> cjump -> L_LACO -> bloco -> JUMP teste -> L_FIM)
        Stm* seq = new StmSeq(label_teste, cjump);
        seq = new StmSeq(seq, label_laco);
        if (bloco) {
            seq = new StmSeq(seq, new StmSeq(bloco, jump_inicio));
        } else {
            seq = new StmSeq(seq, jump_inicio);
        }
        seq = new StmSeq(seq, label_fim);

        return seq;
    }
    else if (auto* cRetorno = dynamic_cast<ComandoRetorno*>(cmd)) {
        Exp* exp_retorno = traduzir_expressao(cRetorno->expressao);
        StmExp* stm_exp = new StmExp();
        stm_exp->exp = exp_retorno;
        return stm_exp;
    }
    
    return nullptr;
}

string TradutorIR::formatar_exp(Exp* exp) {
    if (exp == nullptr) return "";
    
    if (auto* eMem = dynamic_cast<ExpMem*>(exp)) {
        // Ex: MEM( FP + -40 )
        return "MEM( " + formatar_exp(eMem->endereco) + " )";
    }
    else if (auto* eTemp = dynamic_cast<ExpTemp*>(exp)) {
        return "TEMP(" + to_string(eTemp->id) + ")";
    }
    else if (dynamic_cast<ExpTempFramePointer*>(exp)) {
        return "FP";
    }
    else if (auto* eConst = dynamic_cast<ExpConst*>(exp)) {
        // Substitua 'valor' pela propriedade interna do esqueleto
        return "CONST(" + to_string(eConst->valor) + ")"; 
    }
    else if (auto* eBinop = dynamic_cast<ExpBinop*>(exp)) {
        // Substitua 'esquerda' e 'direita' pelos nomes reais (ex: left/right)
        return "BINOP( " + formatar_exp(eBinop->esq) + ", " + formatar_exp(eBinop->dir) + " )";
    }
    else if (auto* eCall = dynamic_cast<ExpCall*>(exp)) {
        string res = "CALL( " + formatar_exp(eCall->nome_funcao) + ", [";
        for (size_t i = 0; i < eCall->parametros.size(); ++i) {
            res += formatar_exp(eCall->parametros[i]);
            if (i < eCall->parametros.size() - 1) {
                res += ", ";
            }
        }
        res += "] )";
        return res;
    }
    else if (auto* eName = dynamic_cast<ExpName*>(exp)) {
        return "NAME(" + eName->nome + ")";
    }
    
    return "EXP_DESCONHECIDA";
}

void TradutorIR::imprimir_ir(Stm* stm) {
    if (stm == nullptr) return;

    // Se for uma sequência, não imprime ela mesma, apenas desenrola os filhos
    // Isso garante a regra do PDF de "um comando por linha"
    if (auto* seq = dynamic_cast<StmSeq*>(stm)) {
        imprimir_ir(seq->esq);
        imprimir_ir(seq->dir);
        return;
    }
    
    // Imprime um comando MOVE
    if (auto* move = dynamic_cast<StmMove*>(stm)) {
        cout << "MOVE " << formatar_exp(move->destino) << ", " << formatar_exp(move->origem) << endl;
        return;
    }
    
    // Imprime o CJUMP (Desvio Condicional)
    if (auto* cjump = dynamic_cast<StmCJump*>(stm)) {
        cout << "CJUMP " << formatar_exp(cjump->expressao_booleana) << " ? " << cjump->destino_verdade << " : " << cjump->destino_falso << endl;
        return;
    }
    
    // Imprime JUMP
    if (auto* jump = dynamic_cast<StmJump*>(stm)) {
        cout << "JUMP " << jump->destino << endl;
        return;
    }
    
    // Imprime LABEL
    if (auto* label = dynamic_cast<StmLabel*>(stm)) {
        cout << label->nome << ":" << endl;
        return;
    }
    
    // Imprime StmExp (Expressão solta)
    if (auto* stmExp = dynamic_cast<StmExp*>(stm)) {
        cout << "EXP " << formatar_exp(stmExp->exp) << endl;
        return;
    }
}