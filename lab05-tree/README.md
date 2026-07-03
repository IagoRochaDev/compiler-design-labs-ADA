# Laboratório 4: Geração de Layout de Frame e Análise de Escape

## 📌 O que é?

O Laboratório 4 é o momento em que nosso compilador deixa de se preocupar apenas com a execução interpretada e começa a se preparar para a geração de código real (assembly). O objetivo deste projeto é varrer a Árvore de Sintaxe Abstrata (AST) para planejar a distribuição de memória da função, gerando a estrutura `FrameFuncao`.

Nesta fase, implementamos recursos cruciais para a compilação:

* **Expansão da Gramática:** suporte a chamadas de funções aninhadas dentro de expressões matemáticas, passando múltiplos argumentos.
* **Análise de Escape:** um algoritmo inteligente que decide o destino de uma variável. Se ela for passada como argumento para outra função, ela "escapa" para a memória (Frame). Se for apenas de uso interno, fica armazenada em Pseudo-Registradores.
* **Cálculo de Layout de Memória:** definição matemática dos *offsets* (deslocamentos) relativos ao *Frame Pointer* (FP). Parâmetros ganham posições positivas (`+8`, `+16`), e variáveis locais no frame ganham posições negativas a partir da área de organização (`-40`, `-48`).
* **Anotação Espacial na AST:** cada uso de variável e cada lado esquerdo de atribuição na árvore recebe um objeto de acesso, vinculando a sintaxe diretamente ao seu endereço físico planejado.

---

## 🛠️ Como Compilar

O projeto utiliza um `Makefile` para facilitar o processo de build. O código fonte está estruturado em C++11 (ou superior).

Para compilar o projeto e gerar o executável, abra o terminal na raiz do projeto e execute:

```bash
make
```

Para limpar os arquivos objetos `.o` e o executável gerados, utilize:

```bash
make clean
```

---

## 🚀 Como Executar

O programa continua recebendo a árvore de tokens via entrada padrão (stdin), além dos arquivos da gramática e tabela LR(1). A principal diferença é que agora o foco da saída no terminal é o relatório detalhado do mapa de memória.

### Execução padrão

Use a regra `run` do Make para compilar e testar redirecionando o seu arquivo de teste:

```bash
make run < ins/teste_lab04.tokens
```

A saída exibirá o **Resumo do Frame**, detalhando o tamanho total em bytes, a quantidade de parâmetros processados e quais variáveis foram parar na memória ou em registradores.

---

## ⚙️ Como Funciona o Pipeline

A execução agora possui etapas focadas no planejamento de memória:

* **Parsing e Construção da AST:** o parser valida a nova gramática estendida e a classe `Funcao` extrai a árvore, que agora suporta estruturas como `verificar(x + 1)`.
* **Passo 2 - Análise de Escape:** o gerador varre todos os comandos buscando chamadas de função. Ele intercepta os argumentos repassados, extrai os nomes das variáveis associadas e as insere em uma lista de "variáveis no frame", além de calcular qual chamada exige o maior número de parâmetros de saída.
* **Passo 3 - Cálculo do Layout:** * Parâmetros recebem `FrameAcessoNoFrame` subindo a partir de `+8`.
* Variáveis marcadas na análise de escape recebem `FrameAcessoNoFrame` descendo a partir de `-40`.
* As demais variáveis recebem `FrameAcessoTemp` com IDs sequenciais (pseudo-registradores).
* O `tamanho_frame` final é cravado na fórmula: `40 + (8 * n_variaveis_no_frame)`.


* **Passo 4 - Linkagem Memória-Árvore:** uma última varredura na AST injeta os ponteiros de acesso recém-calculados dentro das instâncias de `ExpressaoVariavel` e `ComandoAtribuicao`, deixando a árvore totalmente pronta para a futura tradução para assembly.

---

## 🏗️ Arquitetura

Novas classes e estruturas foram adicionadas para gerenciar a abstração da máquina:

* **FrameFuncao:** o maestro do laboratório. Ele armazena as estatísticas do frame (`tamanho_frame`, `n_param_entrada`, etc.) e mantém um dicionário (`std::map`) garantindo que a mesma variável no código sempre aponte para o mesmo endereço alocado.
* **FrameAcesso:** classe base abstrata que representa o método genérico de acesso de um dado.
* **FrameAcessoNoFrame:** herdeiro de `FrameAcesso`, armazena o *offset* (em bytes) exato em que a variável viverá na pilha de execução (Stack).
* **FrameAcessoTemp:** herdeiro de `FrameAcesso`, utilizado para variáveis leves de uso puramente local, armazenando apenas um número de identificação (`id`) para os registradores temporários da CPU.
* **Integração na AST:** as classes preexistentes `ComandoAtribuicao` e `ExpressaoVariavel` ganharam o atributo `acesso_frame`, permitindo que o interpretador ou gerador de código final saiba instantaneamente onde ler ou gravar os valores durante a execução real.