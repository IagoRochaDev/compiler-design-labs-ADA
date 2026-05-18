# 🛠️ Compilador em ADA - Laboratórios Práticos

[![Status](https://img.shields.io/badge/status-active-brightgreen)]()
[![License](https://img.shields.io/badge/license-academic-blue)]()

Repositório contendo os projetos práticos desenvolvidos durante a disciplina de **Construção de Compiladores**. O objetivo é a construção modular de um compilador para a linguagem ADA, implementando as principais fases: análise léxica, sintática e semântica.

## 📚 Conteúdo

Cada laboratório representa uma fase distinta na construção do compilador:

| Laboratório | Descrição | Linguagem |
|------------|-----------|-----------|
| **Lab 01** | Analisador Léxico (Tokenização) | ADA / JavaCC |
| **Lab 02** | Analisador Sintático (Parser LR) | C++ |
| **Lab 03** | Análise Semântica e Geração de Código | *em desenvolvimento* |

## 📂 Estrutura do Repositório

```
compiler-design-labs-ADA/
├── README.md                          # Este arquivo
├── lab01-analisador-lexico/           # Análise léxica
│   ├── README.MD
│   ├── src/                           # Implementação em ADA
│   └── lab01.pdf                      # Especificação
├── lab02-parser-lr/                   # Análise sintática
│   ├── README.md
│   ├── src/                           # Implementação em C++
│   ├── bin/                           # Executáveis
│   ├── exemplos/                      # Exemplos de gramáticas
│   └── lab02.pdf                      # Especificação
└── lab03/                             # Análise semântica
    └── lab03.pdf                      # Especificação
```

## 🚀 Quick Start

### Pré-requisitos
- **Compilador C++**: GCC (G++ 9+)
  - Windows: MSYS2/UCRT64
  - Linux/WSL: nativo
- **Compilador ADA**: GNAT (para Lab 01)
- **Gerador de Tabelas**: JSMachines (opcional, para Lab 02)

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

## 👨‍💻 Autoria

| Campo | Valor |
|-------|-------|
| **Nome** | Iago Rocha Oliveira |
| **Curso** | Ciência da Computação |
| **Instituição** | IFNMG - Campus Montes Claros |
| **Disciplina** | Construção de Compiladores |

## 📖 Referências

- Aho, A. V., Lam, M. S., Sethi, R., & Ullman, J. D. (2006). *Compilers: Principles, Techniques, and Tools* (2nd ed.)
- Appel, A. W. (2002). *Modern Compiler Implementation in Java*
- ISO/IEC 8652:2012 - Ada Programming Language Standard

## 📝 Licença

Desenvolvido com fins acadêmicos na instituição IFNMG - Campus Montes Claros.

---

**Última atualização:** 2026-05-18