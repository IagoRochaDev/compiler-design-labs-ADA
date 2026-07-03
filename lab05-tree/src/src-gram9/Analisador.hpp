#ifndef _ANALISADOR_HPP_
#define _ANALISADOR_HPP_
#include "Funcao.hpp"
#include "ValorLiteral.hpp"
#include <vector>
using namespace std;

class Analisador {
public:
  ValorLiteral calcula_retorno(Funcao *f, const vector<ValorLiteral> &params);
};
  
#endif
