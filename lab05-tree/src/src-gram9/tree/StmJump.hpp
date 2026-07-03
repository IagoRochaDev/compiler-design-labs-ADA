#ifndef _STM_JUMP_HPP_
#define _STM_JUMP_HPP_
#include "Stm.hpp"
#include "Exp.hpp"
#include<vector>
#include<string>
using namespace std;

class StmJump : public Stm {
public:
  Exp* destino;
  vector<string> alvos;
  StmJump();
  StmJump(string label);
  static StmJump* gera_Stm_Jump(/*Adaptar aas necessidades.*/);
};

#endif
