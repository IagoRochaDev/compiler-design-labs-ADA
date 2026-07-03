#include "StmJump.hpp"

StmJump::StmJump() : destino(nullptr) {
}

StmJump::StmJump(string label) : destino(nullptr) {
    alvos.push_back(label);
}

StmJump* StmJump::gera_Stm_Jump(/*Adaptar aas necessidades.*/) {
    return nullptr;
}
