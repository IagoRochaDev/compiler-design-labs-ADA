#ifndef _TRADUTOR_IR_HPP_
#define _TRADUTOR_IR_HPP_

#include <vector>
#include <string>
#include "Funcao.hpp"
#include "Comando.hpp"
#include "Expressao.hpp"
#include "frame/FrameFuncao.hpp"

// Includes básicos da IR
#include "tree/Stm.hpp"
#include "tree/Exp.hpp"

class TradutorIR {
public:
    FrameFuncao* frame;

    TradutorIR(FrameFuncao* f);

    Stm* traduzir_funcao(Funcao* func);
    
    // NOVOS MÉTODOS PARA O LAB 05 (IMPRESSÃO)
    void imprimir_ir(Stm* stm);
    std::string formatar_exp(Exp* exp);

private:
    int cont_label = 0; 
    Stm* traduzir_comandos(const std::vector<Comando*>& comandos);
    Stm* traduzir_comando(Comando* cmd);
    Exp* traduzir_expressao(Expressao* exp);
};

#endif