#ifndef _TABELALR1_HPP_
#define _TABELALR1_HPP_
#include<vector>
#include<string>
#include<iostream>
#include<fstream>   
#include<sstream>
#include<map>
using namespace std;

class Transicao {
public:
  int tipo; 
  int reducao; 
  int prox_estado;
  map<string,Transicao> mapa_LR2;
  string impressao();
  Transicao(string tok); 
  Transicao();
private:
  void inicializa(string tok);
};

class Tabela_LR1 {
public:
  vector<map<string,Transicao> > Tab;
  void debug();
  Tabela_LR1(ifstream &arq_tabela_lr1);
};

#endif
