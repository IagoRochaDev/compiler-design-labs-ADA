#ifndef _COMANDO_HPP_
#define _COMANDO_HPP_
#include<vector>
#include "../Arvore.hpp"
using namespace std;

class TabelaSimbolos;

class Comando {
public:
  static vector<Comando*> extrai_lista_comandos(No_arv_parse* no);
  static Comando* extrai_comando(No_arv_parse* no);
  virtual bool executa(TabelaSimbolos* memoria) = 0;
  virtual void debug_com_tab(int tab);
  virtual ~Comando();
};

#endif
