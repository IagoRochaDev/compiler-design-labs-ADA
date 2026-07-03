#include "StmSeq.hpp"

StmSeq::StmSeq() : esq(nullptr), dir(nullptr) {
}

StmSeq::StmSeq(Stm* esquerda, Stm* direita) : esq(esquerda), dir(direita) {
}

StmSeq* StmSeq::gera_Stm_Seq(/*Adaptar aas necessidades.*/) {
    return nullptr;
}
