#ifndef _EXPRESSAO_VARIAVEL_HPP_
#define _EXPRESSAO_VARIAVEL_HPP_

#include "Expressao.hpp"
#include "ID.hpp"
#include "frame/FrameAcesso.hpp"
#include <vector>

class ExpressaoVariavel : public Expressao {
public:
    ID* nome;

    std::vector<Expressao*> argumentos; 
    FrameAcesso* acesso_frame = nullptr; 

    ExpressaoVariavel();
    ValorLiteral avalia(TabelaSimbolos* memoria) override;
    void debug_com_tab(int tab) override;
};

#endif