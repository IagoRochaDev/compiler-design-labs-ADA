#include "StmMove.hpp"

StmMove::StmMove() : destino(nullptr), origem(nullptr) {
}

StmMove::StmMove(Exp* dest, Exp* orig) : destino(dest), origem(orig) {
}

StmMove* StmMove::gera_Stm_Move(/*Adaptar aas necessidades.*/) {
    return nullptr;
}
