#ifndef _EXP_TEMP_HPP_
#define _EXP_TEMP_HPP_
#include "Exp.hpp"
#include "../Expressao.hpp"

class ExpTemp : public Exp {
public:
  int id;
  ExpTemp();
  ExpTemp(int id_temp);
  static ExpTemp* gera_Exp_Temp(/*Adaptar aas necessidades*/);
};

#endif
