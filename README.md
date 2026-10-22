# CardGame

Jogo estilo CardGame de labirinto


## 📋 Pré-requisitos

### (a) Ambiente de Desenvolvimento (Versão)
O trabalho foi desenvolvido e homologado em ambiente **Windows**. 
*O código faz uso de bibliotecas específicas do sistema operacional, como `<windows.h>` (para a função `Sleep`) e `<conio.h>` (para captura assíncrona de teclas via `_getch()`).*


## 📦 Implentações (M1 para M2)
| Função | Assinatura na M1 (Antiga) | Assinatura na M2 (Atual) | Motivo da Mudança |
| :--- | :--- | :--- | :--- |
| **Desenhar** | `void desenharMapa(int m[20][20])` | `void desenharMapa(const Jogo &j)` | Agora o desenho acessa `j.n` (tamanho dinâmico) e `j.orientacao` (para as portas lógicas) diretamente da struct, sem quebrar o limite de 20. |
| **Movimento** | `void moverJogador(int m[20][20], int &lin, int &col, char x)` | `void moverJogador(Jogo &j, char x)` | A struct já carrega as coordenadas do jogador, a matriz e o atributo `movimentos`, que agora é incrementado internamente de forma automática. |
| **Rotação** | `void rotacionarMapa(int m[20][20], int &lin, int &col, bool dir)` | `void rotacionarMapa(Jogo &j, bool paraDireita)` | Simplificou a atualização simultânea da matriz, das coordenadas do jogador, da `j.orientacao` e das `j.rotacoes`. |
| **Gravidade** | `void gravidade(int m[20][20])` | `void gravidade(Jogo &j)` | Substituição dos limites fixos (18, 19, 20) pelos limites dinâmicos matemáticos (`j.n - 1`, `j.n - 2`), suportando mapas de qualquer tamanho. |
| **Carregar** | `bool CarregarJogo(int num, int m[20][20])` | `bool CarregarJogo(int numero_mapa, Jogo &j)` | A função agora preenche o título, o tamanho `n` e zera os contadores logo na inicialização do estado do jogo. |


## 🛠️ Construído com

* [C++] - Linguagem base (uso de `<fstream>` e `<sstream>`).
* [MinGW / GCC] - Compilador para ambiente Windows.
* [Windows API] - Uso de `<windows.h>` e `<conio.h>` para input e pausas.
* [VS Code] - IDE de desenvolvimento.


## ✒️ Autores

* Heitor Becker Piazera 
* Alexandre Valdemarca Valesan

Você também pode ver a lista de todos os [colaboradores](https://github.com/usuario/projeto/colaboradores) que participaram deste projeto.


