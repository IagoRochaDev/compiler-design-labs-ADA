# Laboratório 5: Tradução para IR (Intermediate Representation)

## 📌 O que é?

O Laboratório 5 avança no pipeline do compilador traduzindo a Árvore de Sintaxe Abstrata (AST) para uma Representação Intermediária (IR) inspirada no estilo do livro do Appel. Nesta etapa transformamos expressões e comandos de alto nível em uma árvore de operações simples (`Stm`, `Exp`, `CALL`, `MEM`, `TEMP`, etc.) que facilitará passos posteriores como alocação de registradores e geração de código final.

Principais objetivos:

- Suportar chamadas de função como expressões (`CALL`) e nomes simbólicos (`NAME`).
- Traduzir expressões (binárias, constantes, variáveis) para nós IR (`ExpBinop`, `ExpConst`, `ExpMem`, `ExpTemp`, ...).
- Traduzir comandos (atribuições, condicionais, laços, retorno) para statements IR (`StmMove`, `StmCJump`, `StmLabel`, `StmExp`, ...).
- Imprimir/serializar a IR para depuração e validação.

---

## 🛠️ Como Compilar

O projeto utiliza um `Makefile` central na raiz de cada laboratório. Para compilar o Laboratório 5, abra um terminal na raiz do workspace do lab e execute:

```bash
cd lab05-tree
make
```

Para limpar objetos e binários:

```bash
make clean
```

---

## 🚀 Como Executar

O executável espera receber a árvore de tokens via stdin e os caminhos da gramática/tabela como argumentos, como nas etapas anteriores. Também há uma regra `run` no `Makefile` que facilita testes com arquivos `.tokens`.

Exemplo rápido (no diretório `lab05-tree`):

```bash
make run < ins/new_tests/caso01.tokens
```

Saída esperada (trecho):

```text
------- Gerando IR (Lab 05) -----------
MOVE TEMP(1), CALL( NAME(soma), [TEMP(2), TEMP(3)] )
EXP TEMP(1)
---------------------------------------
```

Observações:

- Dependendo do layout do frame (etapa anterior) os parâmetros podem aparecer como `MEM( BINOP( FP, CONST(offset) ) )` em vez de `TEMP(...)`. Isso é normal enquanto a integração com o cálculo de frame estiver ativa.
- Caso precise testar rapidamente, use os arquivos em `ins/` (ex.: `ins/new_tests/caso01.tokens`).

---

## ⚙️ Principais Arquivos

- `src/src-gram9/TradutorIR.cpp` : implementação principal da tradução AST -> IR.
- `src/src-gram9/TradutorIR.hpp` : interface do tradutor.
- `src/src-gram9/tree/` : contém os nós do IR (`Exp`, `Stm`, `ExpCall`, `ExpName`, `StmMove`, `StmExp`, ...).
- `ins/` : coleciona entradas de teste em formato `.tokens`.

---

## 🧭 Como Funciona (resumo do fluxo)

1. O parser gera a árvore de parse a partir dos tokens.
2. A camada de extração constrói a AST de alto nível (`Funcao`, `Comando`, `Expressao`).
3. `TradutorIR` visita a AST e para cada nó produz uma sub-árvore IR correspondente:
   - `ExpressaoVariavel` pode virar `ExpMem` (acesso via frame), `ExpTemp` ou — se possuir argumentos — uma `ExpCall` cujo `nome_funcao` é um `ExpName`.
   - `ComandoAtribuicao` vira `StmMove(destino, origem)`.
   - `ComandoRetorno` é traduzido para `StmExp(exp_retorno)` para avaliação em linha.
4. A IR é impressa de forma legível para validação no terminal.
