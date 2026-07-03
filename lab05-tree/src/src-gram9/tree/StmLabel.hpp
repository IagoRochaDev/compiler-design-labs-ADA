#ifndef _STM_LABEL_HPP_
#define _STM_LABEL_HPP_
#include "Stm.hpp"
#include<string>
using namespace std;

class StmLabel : public Stm {
public:
  string nome;
  StmLabel();
  StmLabel(string label_nome);
  static StmLabel* gera_Stm_Label(/*Adaptar aas necessidades.*/);
};

#endif
