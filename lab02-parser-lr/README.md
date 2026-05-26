# 📐 Lab 02 - Analisador Sintático (Parser LR)

**Disciplina:** Construção de Compiladores  
**Autor:** Iago Rocha Oliveira  
**Data:** 2024-2025

## 📖 Descrição

O analisador sintático (ou *parser*) é a segunda fase de um compilador. Sua função é receber um fluxo de tokens (produzido pelo analisador léxico) e verificar se obedecem às regras sintáticas da linguagem, construindo uma **árvore de análise sintática** (parse tree).

Este laboratório implementa um parser LR(1) em C++, que utiliza uma tabela de transições pré-gerada para análise sintática determinística (bottom-up / ascendente).

### Características
- **Método:** LR(1) - Look-Ahead Left-to-Right Rightmost derivation com 1 símbolo de lookahead
- **Linguagem:** C++ (C++11 ou superior)
- **Tabelas:** Geradas via JSMachines ou ferramenta similar
- **Estrutura:** Modular e extensível

## 📁 Estrutura de Arquivos

```
lab02-parser-lr/
├── README.md                                 # Este arquivo
├── lab02.pdf                                 # Especificação e requisitos
├── src/                                      # Código-fonte C++
│   ├── sol.cpp                              # Programa principal
│   ├── Parser.hpp / Parser.cpp              # Classe do parser LR
│   ├── Gramatica.hpp / Gramatica.cpp        # Gramática da linguagem
│   ├── Arvore.hpp / Arvore.cpp              # Árvore de análise sintática
│   ├── TabelaLR1.hpp / TabelaLR1.cpp        # Tabela LR(1)
│   ├── matriz-util.hpp / matriz-util.cpp    # Utilitários de matriz
│   └── g++                                   # Script de compilação
├── bin/                                      # Executáveis compilados
└── exemplos/                                 # Exemplos de gramáticas
    ├── gramatica-1
    ├── gramatica-2
    ├── gramatica-3
    └── gramatica-4
```

## 🗂️ Descrição dos Arquivos Principais

### Código-fonte C++

| Arquivo | Responsabilidade |
|---------|------------------|
| **sol.cpp** | Ponto de entrada (main), orquestra parser |
| **Parser.hpp/cpp** | Implementação do parser LR |
| **Gramatica.hpp/cpp** | Definição de símbolos terminais/não-terminais e produções |
| **Arvore.hpp/cpp** | Estrutura de dados para árvore sintática |
| **TabelaLR1.hpp/cpp** | Tabela de ações e transições LR |
| **matriz-util.hpp/cpp** | Funções auxiliares para matrizes |

### Exemplos de Gramáticas

```
exemplos/
├── gramatica-1   # Exemplo básico (ex: expressões simples)
├── gramatica-2   # Exemplo intermediário
├── gramatica-3   # Exemplo complexo
└── gramatica-4   # Caso especial
```

Cada arquivo de gramática contém:
- Símbolos terminais (tokens)
- Símbolos não-terminais
- Produções (regras gramaticais)

## 🔧 Como Compilar

### Compilação

```bash
# Navegar para src
cd lab02-parser-lr/src

# Executável gerado em ../bin/
g++ *.cpp -o ../bin/parserLR.exe
```

## 🚀 Como Executar

```bash
# Navegar para bin
cd lab02-parser-lr/bin

# Executar jogando o resultado na tela do terminal
./parserLR.exe gramatica.conf tabela_lr1.conf < entrada.txt

# Executar salvando o resultado em um arquivo texto na pasta bin
./parserLR.exe gramatica.conf tabela_lr1.conf < entrada.txt > saida.txt
```

### Entrada Esperada

O programa espera receber um fluxo de tokens (da saída do Lab 01 ou simulados).

### Saída Esperada

```
Análise Sintática bem-sucedida!
Árvore Sintática:
├── S
│   ├── Expr
│   │   ├── Term
│   │   │   └── 42
│   │   └── ...
```

## 📚 Conceitos Principais

### Análise Sintática LR(1)

**LR(1)** significa:
- **L:** Leitura da entrada da esquerda para direita (Left-to-right)
- **R:** Derivação mais à direita (Rightmost derivation)
- **1:** Um símbolo de lookahead

### Estados e Transições

A tabela LR possui:
- **Estados:** Representam posições na análise (0, 1, 2, ...)
- **Ações:** SHIFT (empilhar token), REDUCE (reduzir por produção), ACCEPT (aceitar)
- **GOTO:** Transições de não-terminais

### Exemplo de Tabela LR

```
Estado | Ação (sobre terminal) | Transição (não-terminal)
-------|----------------------|------------------------
   0   | shift se vê (         | goto(1) se vê E
   1   | reduce por E→(E)      | ...
   ...
```

## 🧪 Teste do Parser

Para validar o parser com um exemplo simples:

```bash
# Criar arquivo de entrada
echo "( 1 + 2 )" > entrada.txt

# Compilar
cd src && ./g++ && cd ..

# Executar
./bin/parser < entrada.txt
```

Saída esperada:
```
Token: (
Token: 1
Token: +
Token: 2
Token: )

Parse bem-sucedido!
Árvore sintática construída com sucesso.
```

## 📊 Geração de Tabelas LR

Para gerar tabelas LR a partir de uma especificação de gramática:

1. Usar **JSMachines** ou ferramenta similar
2. Ou implementar na classe `TabelaLR1.cpp`

Exemplo com JSMachines:
```bash
java -jar jsmachines.jar gramatica.txt -o tabelaLR1.txt
```

## 🔍 Debugging

Se encontrar erros durante a análise:

1. **Erro de compilação:** Verificar versão C++ (use `-std=c++11`)
2. **Erro de parsing:** Verificar se entrada segue gramática esperada
3. **Crash:** Verificar limites de array e ponteiros nulos em `Arvore.cpp`

## 📝 Especificação

Consulte `lab02.pdf` para:
- Requisitos completos da gramática
- Casos de teste
- Critérios de avaliação

## 🔗 Referências

- Aho et al. (2006) - *Compilers: Principles, Techniques, and Tools*, Capítulos 4-5
- [LR Parsing Tutorial](https://en.wikipedia.org/wiki/LR_parser)
- Appel (2002) - *Modern Compiler Implementation in Java/C*

---

**Última atualização:** 2026-05-18