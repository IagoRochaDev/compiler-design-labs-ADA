#include "ExpBinop.hpp"

ExpBinop::ExpBinop() : op(nullptr), esq(nullptr), dir(nullptr) {
}

ExpBinop::ExpBinop(Operador* operador, Exp* esquerda, Exp* direita) 
    : op(operador), esq(esquerda), dir(direita) {
}

ExpBinop* ExpBinop::gera_Exp_Binop(Expressao* e) {
    return nullptr;
}
