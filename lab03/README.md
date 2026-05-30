# Laboratório 3: Interpretador e Analisador Semântico

## 📌 O que é?

O Laboratório 3 é a etapa do nosso compilador onde a linguagem (um subconjunto inspirado em **Ada**) ganha vida. Este projeto atua como um **Interpretador e Analisador Semântico**. Ele não apenas valida se o código fonte está sintaticamente correto, mas constrói uma Árvore de Sintaxe Abstrata (AST), valida regras de tipagem estáticas/dinâmicas e executa os comandos instrução por instrução.

Nesta fase, implementamos recursos avançados como:

- **Controle de Escopo e *Shadowing*:** suporte a blocos internos aninhados (`declare ... begin ... end;`) onde variáveis locais podem "sombrear" parâmetros ou variáveis de escopos mais externos.
- **Análise Semântica:** checagem rigorosa de tipos durante atribuições, avaliações de expressões (como operadores aritméticos e booleanos) e retornos de funções.
- **Execução Real:** cálculo de expressões, manipulação de estado na memória e retorno do valor final processado pela função.

---

## 🛠️ Como Compilar

O projeto utiliza um `Makefile` para facilitar o processo de build. O código fonte está estruturado em C++11 (ou superior).

Para compilar o projeto e gerar o executável `compilador`, abra o terminal na raiz do projeto e execute:

```bash
make
```

Para limpar os arquivos objetos `.o` e o executável gerados, utilize:

```bash
make clean
```

## 🚀 Como Executar

O interpretador espera receber uma árvore de tokens via entrada padrão (stdin), além dos arquivos de definição da gramática e da tabela LR(1). Você também pode passar parâmetros iniciais para a função executada através de um arquivo de texto.

### Execução padrão

Use a regra `run` do Make para compilar e executar em um único passo, redirecionando o seu arquivo de teste `.tokens`:

```bash
make run < ins/seu_teste.tokens
```

### Executando com parâmetros

Se sua função Ada recebe parâmetros (por exemplo, `function soma(a : Integer; b : Integer)`), você pode passar um arquivo de texto contendo os valores (um valor por linha). Para facilitar, use process substitution no Linux:

```bash
make run PARAMS=<(echo -e "2\n5") < ins/0-retorna_soma_int.tokens
```

Alternativa: crie um arquivo `meus_params.txt`, coloque os números um abaixo do outro e rode:

```bash
make run PARAMS="meus_params.txt" < ins/0-retorna_soma_int.tokens
```

## ⚙️ Como Funciona o Pipeline

A execução do interpretador segue um fluxo bem definido:

- **Parsing LR(1):** o parser lê a gramática (`.site`), a tabela de estados (`tabela_lr1.conf`) e a entrada de tokens, validando a sintaxe e construindo a `Arvore_parse`.
- **Extração da AST:** a classe `Funcao` navega na árvore de parse, extraindo os parâmetros da função, buscando blocos de variáveis locais (incluindo declaradas internamente) e montando a lista de comandos a serem executados.
- **Montagem do Escopo (Memória):** o `Analisador` cria uma `TabelaSimbolos` de execução, alocando os parâmetros e inicializando a tabela para suportar hierarquia de escopos (pai/filho).
- **Execução e Análise Semântica:** o laço principal chama o método `executa()` de cada comando. Durante esse processo:
  - lados direitos de expressões são avaliados de forma recursiva;
  - validações de tipo ocorrem (por exemplo, impedir soma de `Boolean` com `Integer`, impedir que `not` seja aplicado a inteiros);
  - se ocorrer uma violação de tipagem ou uso de variável não inicializada, o interpretador aborta com um `std::runtime_error` claro e legível.
- **Retorno:** o valor final calculado pelo comando `return` é validado contra a assinatura original da função e impresso no terminal.

## 🏗️ Arquitetura

A arquitetura orientada a objetos foi desenhada para isolar responsabilidades e facilitar a travessia (Visitor / Interpreter pattern implícito):

- **TabelaSimbolos:** coração do controle de memória. Foi aprimorada com um ponteiro de autorreferência (`pai`) para suportar aninhamento. O método `busca()` sobe a hierarquia de escopos automaticamente. As travas de tipo em atribuições são feitas diretamente aqui para evitar estados corrompidos.
- **Funcao:** gerencia seu próprio escopo. O método `extrai_funcao` varre recursivamente a árvore lidando com nós dinâmicos (`DeclVar`, `ListaIds`, `Acesso`), instanciando e registrando variáveis de blocos internos antes da execução.
- **Comando e Derivados:** classes como `ComandoAtribuicao` e `ComandoReturn` possuem travas semânticas. Elas verificam a propriedade `.tipo` do `ValorLiteral` antes de consolidar ações.
- **Analisador:** controla o ciclo de vida. Executa os comandos, captura a exceção de tempo de execução (`try-catch`) e garante que a limpeza de memória ocorra adequadamente.
