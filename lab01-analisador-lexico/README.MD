# 🔤 Lab 01 - Analisador Léxico

**Disciplina:** Construção de Compiladores  
**Autor:** Iago Rocha Oliveira  
**Data:** 2024-2025

## 📖 Descrição

O analisador léxico (ou *scanner*) é a primeira fase de um compilador. Sua responsabilidade é ler o código-fonte caractere por caractere e agrupar sequências de caracteres em **tokens** (unidades léxicas significativas).

Este laboratório implementa um analisador léxico para uma linguagem baseada em ADA, capaz de reconhecer:
- **Palavras-chave** (ex: `program`, `var`, `if`, `while`)
- **Identificadores** (nomes de variáveis e funções)
- **Operadores** (aritméticos, relacionais, lógicos)
- **Literais** (números inteiros, reais, strings)
- **Delimitadores** (parênteses, ponto-e-vírgula, etc)
- **Comentários** (ignorar durante tokenização)

## 📁 Estrutura de Arquivos

```
lab01-analisador-lexico/
├── README.MD                                          # Este arquivo
├── lab01.pdf                                          # Especificação e requisitos
└── src/
    ├── compil-lab1-solucao-a-iago-rocha.jj          # Solução A: Definição em JavaCC
    ├── compil-lab1-amostra-b-iago-rocha.adb         # Solução B (Amostra): Implementação em ADA
    └── compil-lab1-resposta-c-iago-rocha.txt        # Solução C: Documentação de análise
```

## 🗂️ Descrição dos Arquivos

### 1. **compil-lab1-solucao-a-iago-rocha.jj**
- **Tipo:** Especificação JavaCC
- **Propósito:** Define a gramática léxica e as regras de tokenização
- **Conteúdo:**
  - Declarações de tokens
  - Expressões regulares para cada tipo de token
  - Ações léxicas (ex: ignorar espaços em branco, comentários)

### 2. **compil-lab1-amostra-b-iago-rocha.adb**
- **Tipo:** Código-fonte ADA
- **Propósito:** Implementação do analisador léxico em ADA puro
- **Conteúdo:**
  - Procedimentos para leitura e processamento de caracteres
  - Funções para classificação de tokens
  - Estrutura de dados para armazenar tokens
  - Testes unitários

### 3. **compil-lab1-resposta-c-iago-rocha.txt**
- **Tipo:** Documentação técnica
- **Propósito:** Responde perguntas conceituais sobre análise léxica
- **Conteúdo:**
  - Explicações de conceitos e decisões de design
  - Análise de casos extremos
  - Discussão de trade-offs

## 🔧 Como Executar

### Pré-requisitos
- **Compilador ADA:** GNAT (incluído no GCC)
- **JavaCC** (opcional, para processar arquivo .jj)

### Compilar e Executar em ADA

```bash
# Navegar para o diretório
cd lab01-analisador-lexico/src

# Compilar o programa javacc
javacc compil-lab1-solucao-a-iago-rocha.jj

# Executar o programa javacc
javac *.java

# Executar o AnalisadorAda 
java AnalisadorAda < compil-lab1-amostra-b-iago-rocha.adb

```

## 📚 Conceitos Principais

### Tokens Reconhecidos

| Categoria | Exemplos |
|-----------|----------|
| **Palavras-chave** | `program`, `var`, `if`, `while` |
| **Identificadores** | `contador`, `x`, `saldo_final` |
| **Números** | `42`, `3.14`, `1E-5` |
| **Strings** | `"Hello"`, `'text'` |
| **Operadores** | `+`, `-`, `*`, `/`, `=`, `<`, `>` |
| **Delimitadores** | `(`, `)`, `;`, `,`, `.` |
| **Comentários** | `-- isto é comentário` (ignorados) |

## 📝 Especificação

Consulte `lab01.pdf` para requisitos detalhados e critérios de avaliação.

## 🔗 Referências

- [ADA Programming Language](https://www.adaic.org/)
- [JavaCC Documentation](https://javacc.github.io/javacc/)
- Aho et al. (2006) - *Compilers*, Capítulo 2

---

**Última atualização:** 2026-05-18