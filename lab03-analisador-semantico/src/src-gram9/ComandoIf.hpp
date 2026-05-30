#ifndef _COMANDO_IF_HPP_
#define _COMANDO_IF_HPP_
#include "Comando.hpp"
#include "Expressao.hpp"
#include <vector>
using namespace std;

class ComandoIf : public Comando {
public:
  Expressao* condicao;
  vector<Comando*> comandos_then;
  ComandoIf* proximo_elsif;
  vector<Comando*> comandos_else;

  ComandoIf();
  bool executa(TabelaSimbolos* memoria);
  void debug_com_tab(int tab);
};

#endif
