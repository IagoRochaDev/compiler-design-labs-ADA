#ifndef _COMANDO_DECLARE_HPP_
#define _COMANDO_DECLARE_HPP_

#include "Comando.hpp"
#include "Variavel.hpp"
#include <vector>

using namespace std;

class ComandoDeclare : public Comando {
public:
  
  vector<Variavel*> declaracoes; 
  
  
  vector<Comando*> comandos;     

  ComandoDeclare();
  bool executa(TabelaSimbolos* memoria) override;
  void debug_com_tab(int tab) override;
};

#endif