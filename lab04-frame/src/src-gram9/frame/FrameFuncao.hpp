#ifndef _FRAME_FUNCAO_HPP_
#define _FRAME_FUNCAO_HPP_

#include "../Funcao.hpp"
#include "../Comando.hpp"
#include "../Expressao.hpp"
#include <set>
#include <string>

using namespace std;

class FrameFuncao {
public:
    int tamanho_frame;
    int n_param_entrada;
    int n_maximo_param_saida;
    int n_pseudo_registradores;
    int n_variaveis_no_frame;

    FrameFuncao();
    static FrameFuncao* gera_frame_de_funcao(Funcao* fun);

    void imprimir() const;

private:
    set<string> variaveis_no_frame_set;

    void analisar_comandos_por_escape(const vector<Comando*>& comandos);
    void analisar_comando_por_escape(Comando* cmd);
    void analisar_expressao_por_chamadas(Expressao* exp);
    void coletar_variaveis_de_expressao(Expressao* exp);
 
    void calcular_layout(Funcao* fun);
};

#endif