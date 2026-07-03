Análise da sua gramática e estruturação do plano detalhado para o **Laboratório 04**.

---

## 🔍 1. Verificação da Gramática

A boa notícia é que **sua gramática já está perfeitamente preparada para o Lab 04 e NÃO precisa de alterações!** 

O enunciado exige que chamadas de funções possam fazer parte de expressões (ex: `x := 10 + calcular(a, b);`) e aceitem listas de expressões como parâmetros. Vamos rastrear como a sua gramática já faz isso nativamente:

1. Uma **`Expressao`** se reduz até chegar em **`ExpUnaria`**, que por sua vez se reduz a um **`Fator`**.
2. O **`Fator`** possui a derivação direta: `Fator -> Acesso`.
3. E a regra de **`Acesso`** contém a chamada de função:
`Acesso -> Acesso ABRE_PARENTESES ListaExp FECHA_PARENTESES`
4. A regra `ListaExp` se desdobra em `ListaExpNotEmpty`, que aceita múltiplos elementos separados por vírgula (`ListaExpNotEmpty VIRGULA Expressao`).

Portanto, uma chamada de função na sua linguagem já é tratada sintaticamente como um `Fator` dentro de qualquer expressão aritmética ou lógica. Você pode manter o seu Parser exatamente como está.

---

## 🗺️ 2. Entendendo o Layout do Frame (Pilha)

Antes de programar, precisamos visualizar como a memória de uma função organizada por este laboratório deve ficar. O Frame Pointer (FP) é a nossa referência zero.

* **Parâmetros (Acima do FP):** Ficam no frame de quem chamou. O primeiro em `FP + 8`, o segundo em `FP + 16`. Eles **não** contam no tamanho do frame atual.


* 
**Organização Interna:** Os primeiros 40 bytes abaixo do FP (`FP` até `FP - 40`) são reservados para controle do sistema (endereço de retorno, etc.).


* 
**Variáveis Locais no Frame:** Começam estritamente em `FP - 40`, depois `FP - 48`, etc.



---

## 📋 3. Plano de Implementação Passo a Passo

Vamos dividir o Lab 04 em **4 Passos**. Faça um por um e use os *Checkpoints de Validação* antes de avançar.

### Passo 1: Criar as Classes de Estrutura de Dados

Você precisa criar as classes que vão guardar as estatísticas do frame e os modos de acesso de cada variável.

1. 
**Criar `FrameFuncao.hpp**`: Uma classe simples para armazenar o resumo da função.


```cpp
class FrameFuncao {
public:
    int tamanho_frame;          [cite_start]// Total de bytes na pilha (40 + 8 * n_variaveis_no_frame) [cite: 12, 24]
    int n_param_entrada;       [cite_start]// Quantidade de parâmetros [cite: 13, 14]
    int n_maximo_param_saida;  [cite_start]// Maior nº de argumentos passados em uma chamada interna [cite: 14, 15]
    [cite_start]int n_pseudo_registradores;// Qtde de variáveis que viraram Temp [cite: 16, 17]
    int n_variaveis_no_frame;  [cite_start]// Qtde de variáveis que foram para a memória [cite: 17, 19]
};

```


2. 
**Criar `FrameAcesso.hpp**`: A classe base e suas derivadas (`NoFrame` e `Temp`).


```cpp
class FrameAcesso { public: virtual ~FrameAcesso() {} };

class FrameAcesso_NoFrame : public FrameAcesso {
public:
    int posicao_no_frame; [cite_start]// Guardará +8, +16 (params) ou -40, -48 (locais) [cite: 26, 29, 37]
    FrameAcesso_NoFrame(int pos) : posicao_no_frame(pos) {}
};

class FrameAcesso_Temp : public FrameAcesso {
public:
    int id; [cite_start]// ID incremental iniciando em 1 [cite: 38]
    FrameAcesso_Temp(int i) : id(i) {}
};

```


3. 
**Alterar `ExpressaoVariavel.hpp**`: Adicione o campo `FrameAcesso* acesso_frame;`.



**⚙️ Checkpoint 1:** O projeto deve compilar normalmente após a adição dessas classes. Nada muda na execução por enquanto.

---

### Passo 2: Analisar a Regra de Escape (Quem vai para o Frame?)

Antes de calcular as posições, precisamos descobrir quais variáveis internas "escapam" para a memória. A regra diz: se a variável aparece dentro de uma expressão que é passada como argumento para outra função, ela vai para o Frame; caso contrário, vai para Registrador (Temp).

1. Implemente uma varredura na AST da função.
2. Ao encontrar um nó de chamada de função (`Acesso` com lista de expressões):
* Entre em cada `Expressao` da lista de argumentos.
* Colete o nome de todas as variáveis (`ExpressaoVariavel`) que aparecem ali.
* Marque essas variáveis em uma lista ou tabela temporária como `deve_ir_para_frame = true`.





**⚙️ Checkpoint 2:** No método de debug da sua classe `Funcao`, mande imprimir a lista de variáveis que você detectou que "escaparam".

* 
*Teste:* Para `resultado := not x;`, `x` NÃO deve escapar.


* 
*Teste:* Para `sucesso := verificar(x + 1);`, `x` DEVE constar como "escapou para o frame".



---

### Passo 3: Calcular o Layout do Frame (`FrameFuncao`)

Agora que você sabe quem vai para onde, percorra as declarações da função para preencher o objeto `FrameFuncao` e gerar os endereços.

1. **Parâmetros de entrada:** Percorra a lista de parâmetros. Cada um ganha um `FrameAcesso_NoFrame` com posições `+8`, `+16`, `+24`, etc. Incremente `n_param_entrada`. Eles não somam no `tamanho_frame` atual.


2. **Variáveis Locais:** Percorra as variáveis declaradas (inclusive em blocos `DECLARE` internos).
* **Se foi marcada para ir para o Frame (Passo 2):** Crie um `FrameAcesso_NoFrame`. A primeira ganha a posição `-40`, a próxima `-48`, `-56`, etc. Incremente `n_variaveis_no_frame`.


* **Se NÃO foi marcada:** Crie um `FrameAcesso_Temp`. O `id` começa em 1 e vai incrementando sequencialmente (`id=1`, `id=2`, ...) na ordem em que aparecem. Incremente `n_pseudo_registradores`.




3. **Análise de Saída:** Varra os comandos procurando chamadas de funções feitas por ela. Veja qual chamada passa o maior número de argumentos e salve esse número em `n_maximo_param_saida`.


4. 
**Cálculo Final:** Defina `tamanho_frame = 40 + (8 * n_variaveis_no_frame)`.



**⚙️ Checkpoint 3:** Crie uma função de impressão para o `FrameFuncao`. Ao rodar um caso de teste, o console deve exibir com precisão a contagem de variáveis, registradores e o tamanho final calculado em bytes.

---

### Passo 4: Decorar a AST (Vincular Usos à Declaração)

O PDF exige que múltiplos usos da mesma variável apontem para o mesmo objeto de acesso ou equivalente.

1. Na sua `TabelaSimbolos`, quando uma variável for declarada, além de guardar o tipo dela, guarde também o ponteiro do `FrameAcesso` criado para ela no Passo 3.
2. Durante a execução ou uma passada de decoração semântica na árvore, toda vez que encontrar um nó `ExpressaoVariavel`, faça uma busca na `TabelaSimbolos` pelo nome dela e associe o campo `this->acesso_frame = var_declarada->acesso_frame;`.



**⚙️ Checkpoint 4 (Final):** Atualize o método `debug_com_tab` de `ExpressaoVariavel` para que ao imprimir o nome da variável, ele imprima ao lado o local onde ela está alocada.
Exemplo de saída esperada no debug da árvore:

* 
`VAR[x (NoFrame: -40)]` 


* 
`VAR[resultado (Temp: id 1)]` 



---

Podemos começar a codificar o **Passo 1**? Se preferir, compartilhe a assinatura da sua classe `Funcao` ou do seu gerenciador de declarações para mapearmos onde o objeto `FrameFuncao` será instanciado.