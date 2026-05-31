#include "FrameFuncao.hpp"
#include <iostream>

// Inicializando os atributos para evitar lixo de memória
FrameFuncao::FrameFuncao() {
    tamanho_frame = 0;
    n_param_entrada = 0;
    n_maximo_param_saida = 0;
    n_pseudo_registradores = 0;
    n_variaveis_no_frame = 0;
}

FrameFuncao* FrameFuncao::gera_frame_de_funcao(Funcao* fun) {
    
    return NULL;
}

void FrameFuncao::imprimir() const {
    std::cout << "\n========= RESUMO DO FRAME =========" << std::endl;
    std::cout << "Tamanho do Frame:           " << tamanho_frame << " bytes" << std::endl;
    std::cout << "N. Parametros Entrada:      " << n_param_entrada << std::endl;
    std::cout << "N. Maximo Parametros Saida: " << n_maximo_param_saida << std::endl;
    std::cout << "N. Pseudo-Registradores:    " << n_pseudo_registradores << std::endl;
    std::cout << "N. Variaveis no Frame:      " << n_variaveis_no_frame << std::endl;
    std::cout << "===================================\n" << std::endl;
}