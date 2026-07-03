#include "StmCJump.hh"

StmCJump::StmCJump() : expressao_booleana(nullptr), destino_verdade(""), destino_falso("") {
}

StmCJump::StmCJump(Exp* expr, string true_label, string false_label) 
    : expressao_booleana(expr), destino_verdade(true_label), destino_falso(false_label) {
}

StmCJump* StmCJump::gera_Stm_CJump(/*Adaptar aas necessidades.*/) {
    return nullptr;
}
