#include <iostream>
#include <termios.h>
#include <unistd.h>
using namespace std;

// ============================================================================
// FUNÇÕES DE SISTEMA E INPUT
// ============================================================================

//Lê um único caractere do teclado.

int getch(void) {
	int ch;
	struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}


//Posiciona o cursor do terminal nas coordenadas (XPos, YPos) usando sequências de escape ANSI.

void SetCursorPos(int XPos, int YPos) {
	printf("\033[%d;%dH", YPos + 1, XPos + 1);
}

/*
 * Oculta (estado = false) ou exibe (estado = true) o cursor do terminal.
 */
void alternarCursor(bool exibir) {
	if (exibir) {
		printf("\033[?25h");
	} else {
		printf("\033[?25l");
	}
}

//Limpa a tela só isso
 
void limparTela() {
	(void)system("clear");
}

// ============================================================================
// FUNÇÕES DE MANIPULAÇÃO DE MATRIZ E ESTADO DO JOGO
// ============================================================================


//Copia o conteúdo de uma matriz 20x20 de origem para uma matriz de destino.

void copiarMatriz(const int origem[20][20], int destino[20][20]) {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			destino[i][j] = origem[i][j];
		}
	}
}

//Capia para matrizes não constantes

void copiarMatriz(int origem[20][20], int destino[20][20]) {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			destino[i][j] = origem[i][j];
		}
	}
}

//Aplica a física minha maior inimiga
 
void gravidade(int m[20][20]) {
	for (int l = 18; l >= 0; l--) {
		for (int c = 0; c < 20; c++) {
			if (m[l][c] == 3) {
				int pos = l;
				while (pos < 19 && m[pos + 1][c] == 0) {
					m[pos + 1][c] = 3;
					m[pos][c] = 0;
					pos++;
				}
			}
		}
	}
}


//Desenha a matriz do jogo no console
 
void desenharMapa(int m[20][20], int contador) {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			switch (m[i][j]) {
				case 0:
					cout << " ";
					break;
				case 1:
					printf("\033[0;44m ");
					printf("\033[0m");
					break;
				case 2:
					cout << "@";
					break;
				case 3:
					cout << "O";
					break;
				case 4:
					cout << "A";
					break;
				case 5:
					cout << "S";
					break;
				case 6:
					cout << (contador % 2 == 1 ? ":" : "=");
					break;
				case 7:
					cout << (contador % 2 == 1 ? "|" : ";");
					break;
				default:
					cout << " ";
			}
		}
		cout << endl;
	}
}

//Localiza o pivete / personagem
 
void encontrarJogador(int m[20][20], int &pli, int &pco) {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			if (m[i][j] == 2) {
				pli = i;
				pco = j;
				return;
			}
		}
	}
}


//Tenta mover o jogador em uma determinada direção (deltaLinha, deltaColuna).
//Atualiza as coordenadas (pli, pco) e guarda o elemento que ficou abaixo do jogador.

void moverJogador(int m[20][20], int &pli, int &pco, int &item_em_baixo, int deltaLinha, int deltaColuna) {
	m[pli][pco] = item_em_baixo;
	int novaLinha = pli + deltaLinha;
	int novaColuna = pco + deltaColuna;

// Bloqueia movimento
	if (m[novaLinha][novaColuna] == 1 || m[novaLinha][novaColuna] == 3) {
		m[pli][pco] = 2;
	} else {
		pli = novaLinha;
		pco = novaColuna;
		item_em_baixo = m[pli][pco];
		m[pli][pco] = 2;
	}
}

//Roda roda jequiti / gira a matrix

void rotacionarMapa(int m[20][20], int &pli, int &pco, int &item_em_baixo, int &contador, bool paraDireita) {
	if (item_em_baixo != 4) return; // Só rotaciona se estiver sobre uma alavanca

	int transposta[20][20];
	m[pli][pco] = item_em_baixo;

	for (int l = 0; l < 20; l++) {
		for (int c = 0; c < 20; c++) {
			if (paraDireita) {
				transposta[c][19 - l] = m[l][c];
			} else {
				transposta[19 - c][l] = m[l][c];
			}
		}
	}

	copiarMatriz(transposta, m);

	int old_pli = pli;
	if (paraDireita) {
		pli = pco;
		pco = 19 - old_pli;
	} else {
		pli = 19 - pco;
		pco = old_pli;
	}

	item_em_baixo = m[pli][pco];
	m[pli][pco] = 2;
	contador++;
	gravidade(m);
}


//Checa se o jogador alcançou o final.

bool verificarVitoria(int item_em_baixo) {
	if (item_em_baixo == 5) {
		limparTela();
		alternarCursor(true);
		cout << "========================================\n";
		cout << "        PARABENS! VOCE VENCEU!          \n";
		cout << "========================================\n\n";
		cout << "Pressione qualquer tecla para voltar ao menu...";
		getch();
		limparTela();
		return true;
	}
	return false;
}


//LOOP DE JOGO

//Loop genérico de execução do jogo. Utilizado tanto para novas partidas quanto para partidas continuadas.
 
void executarJogo(int m[20][20], const int mapa_inicial[20][20], int &pli, int &pco) {
	alternarCursor(false);
	printf("\033[H\033[J"); // Limpa a tela

	char x;
	int contador = 0;
	int item_em_baixo = 0;

	while (true) {
		SetCursorPos(0, 0);
		desenharMapa(m, contador);

		cout << "texto depois da matriz\n";
		cout << "Quantidade de rotações:" << contador;

		if (verificarVitoria(item_em_baixo)) break;

		x = getch();

		// ESC: Retorna ao menu
		if (x == 27) {
			limparTela();
			alternarCursor(true);
			break;
		}

		// R/r: Reinicia o nível
		if (x == 'r' || x == 'R') {
			copiarMatriz(mapa_inicial, m);
			encontrarJogador(m, pli, pco);
			contador = 0;
			item_em_baixo = 0;
			continue;
		}

		switch (x) {
			case 'w': moverJogador(m, pli, pco, item_em_baixo, -1, 0); break;
			case 's': moverJogador(m, pli, pco, item_em_baixo, 1, 0);  break;
			case 'a': moverJogador(m, pli, pco, item_em_baixo, 0, -1); break;
			case 'd': moverJogador(m, pli, pco, item_em_baixo, 0, 1);  break;
			case 'e': rotacionarMapa(m, pli, pco, item_em_baixo, contador, true);  break; // Rotação Direita
			case 'q': rotacionarMapa(m, pli, pco, item_em_baixo, contador, false); break; // Rotação Esquerda
		}
	}
}

//Mapa 1

void mapa1(bool &jogado, int matriz_continuada[20][20]) {
	const int mapa_inicial[20][20] = {
		{1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 4, 0, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 6, 1, 6, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 3, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 1, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 0, 1, 0, 1, 1, 1, 3, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 7, 1, 7, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 5, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1},
		{1, 0, 4, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 4, 0, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	};

	int pli = 6, pco = 6;
	int m[20][20];
	jogado = true;

	copiarMatriz(mapa_inicial, m);
	executarJogo(m, mapa_inicial, pli, pco);
	copiarMatriz(m, matriz_continuada);
}


//Mapa 2
 
void mapa2(bool &jogado, int matriz_continuada[20][20]) {
	const int mapa_inicial[20][20] = {
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 2, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 4, 1},
		{1, 0, 3, 0, 1, 0, 4, 0, 0, 1, 0, 3, 0, 0, 1, 0, 1, 1, 0, 1},
		{1, 0, 0, 0, 6, 0, 0, 0, 0, 7, 0, 0, 0, 0, 6, 0, 1, 5, 0, 1},
		{1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 4, 0, 1, 0, 0, 1, 0, 4, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1},
		{1, 0, 0, 0, 1, 6, 1, 1, 1, 1, 7, 1, 1, 1, 6, 1, 0, 0, 0, 1},
		{1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 1},
		{1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1},
		{1, 0, 0, 0, 1, 7, 1, 1, 1, 1, 6, 1, 1, 1, 7, 1, 0, 0, 0, 1},
		{1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 4, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 4, 0, 1, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
		{1, 0, 0, 0, 7, 0, 0, 0, 0, 6, 0, 0, 0, 0, 7, 0, 0, 0, 0, 1},
		{1, 0, 1, 1, 1, 0, 0, 3, 0, 1, 0, 0, 4, 0, 1, 0, 3, 0, 0, 1},
		{1, 4, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	};

	int pli = 1, pco = 1;
	int m[20][20];
	jogado = true;

	copiarMatriz(mapa_inicial, m);
	executarJogo(m, mapa_inicial, pli, pco);
	copiarMatriz(m, matriz_continuada);
}

//Mapa 3

void mapa3(bool &jogado, int matriz_continuada[20][20]) {
	const int mapa_inicial[20][20] = {
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 0, 4, 1, 0, 0, 0, 0, 1},
		{1, 0, 2, 0, 0, 1, 0, 0, 0, 3, 0, 0, 0, 0, 1, 0, 0, 5, 0, 1},
		{1, 0, 0, 0, 0, 1, 1, 1, 6, 1, 1, 7, 1, 1, 1, 0, 0, 0, 0, 1},
		{1, 1, 6, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 7, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 3, 0, 0, 0, 0, 1, 4, 0, 0, 4, 1, 0, 0, 0, 0, 3, 0, 1},
		{1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1},
		{1, 0, 0, 0, 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
		{1, 1, 7, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 6, 1, 1},
		{1, 1, 6, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 7, 1, 1},
		{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1, 0, 0, 0, 1},
		{1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1},
		{1, 0, 3, 0, 0, 0, 0, 1, 4, 0, 0, 4, 1, 0, 0, 0, 0, 3, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 7, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 6, 1, 1},
		{1, 0, 0, 0, 0, 1, 1, 1, 7, 1, 1, 6, 1, 1, 1, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 3, 0, 0, 0, 1, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 0, 4, 1, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	};

	int pli = 2, pco = 2;
	int m[20][20];
	jogado = true;

	copiarMatriz(mapa_inicial, m);
	executarJogo(m, mapa_inicial, pli, pco);
	copiarMatriz(m, matriz_continuada);
}

void jogo_continuado(int matriz_continuada[20][20]) {
	const int mapa_inicial[20][20] = {
		{1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 4, 0, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 6, 1, 6, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 3, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 1, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 0, 1, 0, 1, 1, 1, 3, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 7, 1, 7, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 5, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1},
		{1, 0, 4, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 4, 0, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	};

	int pli = 0, pco = 0;
	encontrarJogador(matriz_continuada, pli, pco);
	executarJogo(matriz_continuada, mapa_inicial, pli, pco);
}

// ============================================================================
// MENU PRINCIPAL E SELEÇÃO DE FASES
// ============================================================================

//Novo jogo menu:
 
void menuSelecaoMapa(bool &jogado, int matriz_continuada[20][20]) {
	limparTela();
	int escolhaMapa;
	cout << "========================================\n";
	cout << "           SELECAO DE MAPAS             \n";
	cout << "========================================\n";
	cout << "1 - Mapa 1 (Original)\n";
	cout << "2 - Mapa 2 (Corredores em Cruz)\n";
	cout << "3 - Mapa 3 (Labirinto Periférico)\n";
	cout << "4 - Voltar ao Menu Principal\n";
	cout << "Escolha uma opcao: ";
	cin >> escolhaMapa;
	cin.ignore(10000, '\n');

	limparTela();
	if (escolhaMapa == 1) {
		mapa1(jogado, matriz_continuada);
	} else if (escolhaMapa == 2) {
		mapa2(jogado, matriz_continuada);
	} else if (escolhaMapa == 3) {
		mapa3(jogado, matriz_continuada);
	}
}

void exibirSobre() {
	limparTela();
	cout << "Este jogo foi desenvolvido por Heitor Becker Piazera e Alexandre V Valesan na disciplina de algoritmos 2 lecionada por Thiago Felski\n\n";
	cout << "Como jogar?\n";
	cout << "O objetivo do jogo é chegar a saida (s)\n";
	cout << "As alavancas (A) servem como um meio de rotação do mapa tanto para direita ou para esquerda\n";
	cout << "Para acionar a alavanca, basta se posicionar em cima dela e apertar\n";
	cout << "Q - girar para esquerda\n";
	cout << "E - girar para direita\n";
	cout << "Os blocos (O) tem sistema de gravidade, ou seja, quando o mapa gira eles mudam de posição de acordo com a rotação\n";
	cout << "De acordo com a rotação dos mapa as portas se abrirão e outras fecharão\n";
	cout << "Mais comandos\n";
	cout << "R - reset do mapa\n";
	cout << "Esc - pause o jogo\n\n";
	cout << "Digite qualquer número para voltar ao menu\n";
	int voltar;
	cin >> voltar;
	limparTela();
}

int main() {
	bool jogado = false;
	int escolha_menu1;
	int matriz_continuada[20][20];

	while (true) {
		cout << "CARDGAME" << endl;
		cout << "Pressione o número indicado para:" << endl;
		cout << "1 - Novo jogo" << endl;
		cout << "2 - Continuar" << endl;
		cout << "3 - Sobre" << endl;
		cout << "4 - Fim" << endl;
		cin >> escolha_menu1;
		cin.ignore(10000, '\n');

		if (escolha_menu1 == 1) {
			menuSelecaoMapa(jogado, matriz_continuada);
		} else if (escolha_menu1 == 2) {
			if (!jogado) {
				cout << "Nenhum jogo foi iniciado" << endl;
			} else {
				jogo_continuado(matriz_continuada);
			}
		} else if (escolha_menu1 == 3) {
			exibirSobre();
		} else if (escolha_menu1 == 4) {
			return 0;
		}
	}
}
