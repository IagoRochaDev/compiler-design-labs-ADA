Aqui está o seu `README.md` geral atualizado, incluindo as adições do Lab 04 (Layout de Frame) e do Lab 05 (Representação Intermediária), mantendo exatamente o mesmo padrão e formatação que você definiu:

```markdown
# 🛠️ Compilador em ADA - Laboratórios Práticos


Repositório contendo os projetos práticos desenvolvidos durante a disciplina de **Compiladores**. O objetivo é a construção modular de um compilador para a linguagem ADA, implementando as principais fases: análise léxica, sintática, semântica e tradução para código intermediário.

## 📚 Conteúdo

Cada laboratório representa uma fase distinta na construção do compilador:

| Laboratório | Descrição | Linguagem |
|------------|-----------|-----------|
| **Lab 01** | Analisador Léxico (Tokenização) | ADA / JavaCC |
| **Lab 02** | Analisador Sintático (Parser LR) | C++ |
| **Lab 03** | Análise Semântica e Interpretação | C++ |
| **Lab 04** | Geração de Layout de Frame e Análise de Escape | C++ |
| **Lab 05** | Representação Intermediária (IR) | C++ |

## 📂 Estrutura do Repositório

```text
compiler-design-labs-ADA/
├── README.md                          # Este arquivo
├── lab01-analisador-lexico/           # Análise léxica
│   ├── README.MD
│   ├── src/                           # Implementação
│   └── lab01.pdf                      # Especificação
├── lab02-parser-lr/                   # Análise sintática
│   ├── README.md
│   ├── src/                           # Implementação em C++
│   ├── bin/                           # Executáveis
│   ├── exemplos/                      # Exemplos de gramáticas
│   └── lab02.pdf                      # Especificação
├── lab03-analisador-semantico/        # Análise semântica e Interpretação
│   ├── README.md                      # Documentação de arquitetura e execução
│   ├── Makefile                       # Automação de compilação
│   ├── src/                           # Implementação principal em C++
│   ├── ins/                           # Entradas de teste (.tokens)
│   └── lab03.pdf                      # Especificação
├── lab04-frame/                       # Análise de Escape e Memória
│   ├── README.md                      # Documentação sobre o FrameFuncao
│   ├── Makefile                       # Automação de compilação
│   ├── src/                           # Implementação da AST com acessos
│   ├── ins/                           # Testes de escape e frame
│   └── lab04.pdf                      # Especificação
└── lab05-tree/                        # Representação Intermediária (IR)
    ├── README.md                      # Documentação do Tradutor IR
    ├── Makefile                       # Automação de compilação
    ├── src/                           # Tradutor AST->IR e Classes do Appel
    ├── ins/                           # Casos de teste em C adaptados
    └── lab05.pdf                      # Especificação

```

## 🚀 Quick Start

### Pré-requisitos

* **Compilador C++**: GCC (G++ 9+)
* Windows: MSYS2/UCRT64
* Linux/WSL: nativo


* **Compilador ADA**: GNAT (para Lab 01)
* **Java (JDK)**: Para execução do analisador gerado no Lab 01
* **Gerador de Tabelas**: JSMachines (opcional, para Lab 02)
* **GNU Make**: Utilitário para automação de builds (para os Labs em C++)

### Lab 01 - Analisador Léxico

```bash
cd lab01-analisador-lexico
# Consulte README.MD para instruções

```

### Lab 02 - Analisador Sintático

```bash
cd lab02-parser-lr
# Consulte README.md para instruções de compilação e execução

```

### Lab 03 - Interpretador e Analisador Semântico

```bash
cd lab03-analisador-semantico
# Compila e executa injetando uma árvore de tokens de teste (exemplo)
make run < ins/0-retorna_soma_int.tokens
# Consulte o README.md do diretório para testar passando seus próprios parâmetros

```

### Lab 04 - Geração de Layout de Frame

```bash
cd lab04-frame
# Compila e executa mapeando variáveis para a memória ou registradores
make run < ins/teste_lab04.tokens

```

### Lab 05 - Representação Intermediária (IR)

```bash
cd lab05-tree
# Compila e converte a Árvore Sintática para código de máquina abstrato (IR)
make run < ins/teste_lab05.tokens

```

## 👨‍💻 Autoria

| Campo | Valor |
| --- | --- |
| **Nome** | Iago Rocha Oliveira |
| **Curso** | Ciência da Computação |
| **Instituição** | IFNMG - Campus Montes Claros |
| **Disciplina** | Construção de Compiladores |

## 📖 Referências

* Aho, A. V., Lam, M. S., Sethi, R., & Ullman, J. D. (2006). *Compilers: Principles, Techniques, and Tools* (2nd ed.)
* Appel, A. W. (2002). *Modern Compiler Implementation in Java*
* ISO/IEC 8652:2012 - Ada Programming Language Standard

## 📝 Licença

Desenvolvido com fins acadêmicos na instituição IFNMG - Campus Montes Claros.

---

**Última atualização:** 2026-07-03
