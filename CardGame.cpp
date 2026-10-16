#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <string>
#include <conio.h> // Biblioteca exclusiva do Windows
using namespace std;

const int TAM = 20; 

struct Posicao {
 int lin, col;
};


struct Jogo {
 int cenario[TAM][TAM]; 
 int n; 
 int orientacao; 
 Posicao jogador;
 int movimentos, rotacoes;
 int numeroMapa;
 string titulo;
};

bool CarregarJogo(int numero_mapa, Jogo &j){
	string NomeMapa = "mapa" + to_string(numero_mapa) + ".csv";
	ifstream arq(NomeMapa);
	if(!arq.is_open()){
		return false;
	}
	string frase;
	getline(arq, frase);
	j.n = stoi(frase);
	getline(arq, frase);
	j.titulo = frase;
	for(int i = 0;i<j.n;i++){
	getline(arq, frase);
	stringstream ss(frase);
	string celula;
		int c = 0;

		while(getline(ss, celula, ',')){
			
			j.cenario[i][c] = stoi(celula);
			if(j.cenario[i][c] == 2){
				j.jogador.lin = i;
				j.jogador.col = c;
			}
			c++;
		}
	}
	arq.close();
	return true;
}


// Lê um único caractere do teclado no Windows
int getch() {
    return _getch();
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

//Aplica a física minha maior inimiga
 
void gravidade(Jogo &j) {
	for (int l = j.n - 2; l >= 0; l--) {
		for (int c = 0; c < j.n; c++) {
			if (j.cenario[l][c] == 3) {
				int pos = l;
				while (pos < 19 && j.cenario[pos + 1][c] == 0) {
					j.cenario[pos + 1][c] = 3;
					j.cenario[pos][c] = 0;
					pos++;
				}
			}
		}
	}
}


//Desenha a matriz do jogo no console
 
void desenharMapa(const Jogo &j) {
	for (int i = 0; i < j.n; i++) {
		for (int c = 0; c < j.n; c++) {
			switch (j.cenario[i][c]) {
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
					if(j.rotacoes % 2 == 1){
					    cout<<":";
					}
					if(j.rotacoes % 2 == 0){
					    cout<<"=";
					}
					break;
				case 7:
				    if(j.rotacoes % 2 == 1){
					    cout<<"|";
					}
					if(j.rotacoes % 2 == 0){
					    cout<<";";
					}
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

void moverJogador(Jogo &j, char x) {

    j.movimentos++;
	int deltaLinha = 0;
	int deltaColuna = 0;
	
    if(x == 'w'){deltaLinha = -1; deltaColuna = 0;}
	if(x == 'a'){deltaLinha = 0; deltaColuna = -1;}
	if(x == 's'){deltaLinha = 1; deltaColuna = 0;}
	if(x == 'd'){deltaLinha = 0; deltaColuna = 1;}

	j.cenario[j.jogador.lin][j.jogador.col] = j.item_em_baixo;
	int novaLinha = j.jogador.lin + deltaLinha;
	int novaColuna = j.jogador.col + deltaColuna;

	if(novaLinha < 0 || novaLinha >= j.n || novaColuna < 0 || novaColuna >= j.n){
		j.cenario[j.jogador.lin][j.jogador.col] = 2;
		j.movimentos--;
		return;
	}

	int destino = j.cenario[novaLinha][novaColuna];

	bool portaFechada = false;


	if (destino == 6) {
		portaFechada = (j.rotacoes % 2 == 0); 
	}
	
	else if (destino == 7) {
		portaFechada = (j.rotacoes % 2 == 1); 
	}

	// Bloqueia movimento se for parede (1), bloco (3) ou porta fechada
	if (destino == 1 || destino == 3 || portaFechada) {
		j.cenario[j.jogador.lin][j.jogador.col] = 2;
		j.movimentos--;
	} else {
		j.jogador.lin = novaLinha;
		j.jogador.col = novaColuna;
		j.item_em_baixo = j.cenario[j.jogador.lin][j.jogador.col];
		j.cenario[j.jogador.lin][j.jogador.col] = 2;
	}
}

//Roda roda jequiti / gira a matrix

void rotacionarMapa(Jogo &j, bool paraDireita) {
	if (j.item_em_baixo != 4) return; // Só rotaciona se estiver sobre uma alavanca
	int transposta[20][20];
	j.cenario[j.jogador.lin][j.jogador.col] = j.item_em_baixo;

	for (int l = 0; l < 20; l++) {
		for (int c = 0; c < 20; c++) {
			if (paraDireita) {
				transposta[c][19 - l] = j.cenario[l][c];
			} else {
				transposta[19 - c][l] = j.cenario[l][c];
			}
		}
	}
	if(paraDireita){
	    j.orientacao = j.orientacao + 1;
	}else{
	    j.orientacao = j.orientacao - 1;
	    if (j.orientacao < 0) {
	        j.orientacao = 3;
	    }
	}

	//(Copia a matriz de volta)
	for (int l = 0; l < j.n; l++) {
		for (int c = 0; c < j.n; c++) {
			j.cenario[l][c] = transposta[l][c];
		}
	}

	int old_pli = j.jogador.lin;
	if (paraDireita) {
		j.jogador.lin = j.jogador.col;
		j.jogador.col = 19 - old_pli;
	} else {
		j.jogador.lin = 19 - j.jogador.col;
		j.jogador.col = old_pli;
	}

	j.item_em_baixo = j.cenario[j.jogador.lin][j.jogador.col];
	j.cenario[j.jogador.lin][j.jogador.col] = 2;
	j.rotacoes++;
	gravidade(j);
}


//Checa se o jogador alcançou o final.

bool verificarVitoria(const Jogo &j) {
	if (j.item_em_baixo == 5) {
		return true;
	}else
	return false;
}


//LOOP DE JOGO

//Loop genérico de execução do jogo. Utilizado tanto para novas partidas quanto para partidas continuadas.
 
void executarJogo(Jogo &j) {
	alternarCursor(false);
	printf("\033[H\033[J"); // Limpa a tela
	char x;
	
	while (true) {
		SetCursorPos(0, 0);
		desenharMapa(j);
		
		cout<< "Quantidade de rotações: " << j.rotacoes<<endl;
		cout<<"Quantidade de movimentos: "<< j.movimentos<<endl;
		if(j.orientacao == 0){
		    cout << "A orientação atual é: " << j.orientacao * 90<< "°" <<"\033[K";
		}else if(j.orientacao == 1){
		    cout << "A orientação atual é: " << j.orientacao * 90<< "°" <<"\033[K";
		}else if(j.orientacao == 2){
		    cout << "A orientação atual é: " << j.orientacao * 90<< "°" <<"\033[K";
		}else if(j.orientacao == 3){
		    cout << "A orientação atual é: " << j.orientacao * 90<< "°" <<"\033[K";
		}
		

		if (verificarVitoria(j)){
			limparTela();
			alternarCursor(true);
			cout << "========================================\n";
			cout << "        PARABENS! VOCE VENCEU!          \n";
			cout << "========================================\n\n";
			cout << "Pressione qualquer tecla para voltar ao menu...";
			getch();
			limparTela();
			break; // Sai do loop e volta para o menu
		}

		x = getch();

		// ESC: Retorna ao menu
		if (x == 27) {
			limparTela();
			alternarCursor(true);
			break;
		}

		// R/r: Reinicia o nível
		if (x == 'r' || x == 'R') {
			CarregarJogo(j.numeroMapa, j);
			continue;
		}

		switch (x) {
			case 'w': moverJogador(j, x); break;
			case 's': moverJogador(j, x);  break;
			case 'a': moverJogador(j, x); break;
			case 'd': moverJogador(j, x);  break;
			case 'e': rotacionarMapa(j, true);  break; // Rotação Direita
			case 'q': rotacionarMapa(j, false); break; // Rotação Esquerda
		}
	}
}


// ============================================================================
// MENU PRINCIPAL E SELEÇÃO DE FASES
// ============================================================================

//Novo jogo menu:
 
void menuSelecaoMapa() {
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
	if(escolhaMapa >= 1 && escolhaMapa <= 3){
	Jogo meuJogo;
	CarregarJogo(escolhaMapa, meuJogo);
	executarJogo(meuJogo);
	}else{
		limparTela();
		cout<<"essa opção é inválida, por favor insira uma opção válida entre 1 a 4!!"<<endl;
		cout << "Pressione qualquer tecla para continuar...";
		getch();
		limparTela();
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
	int escolha_menu1;

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
			menuSelecaoMapa();
		} else if (escolha_menu1 == 2) {
				cout << "Nenhum jogo foi iniciado" << endl;
			}
		else if (escolha_menu1 == 3) {
			exibirSobre();
		} else if (escolha_menu1 == 4) {
			return 0;
		}
	}
}
